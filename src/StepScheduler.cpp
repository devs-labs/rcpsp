/**
 * @file StepScheduler.cpp
 * @author The VLE Development Team
 * See the AUTHORS or Authors.txt file
 */

/*
 * Copyright (C) 2012 ULCO http://www.univ-littoral.fr
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>

#include <vle/devs/Dynamics.hpp>

#include <Activities.hpp>

namespace rcpsp {

class StepScheduler : public vle::devs::Dynamics
{
public:
    StepScheduler(const vle::devs::DynamicsInit& init,
                  const vle::devs::InitEventList& events) :
        vle::devs::Dynamics(init, events)
    {
        mLocation = vle::value::toString(events.get("location"));
    }

    Activity* select() const
    {
        if (mWaitingActivities.empty()) {
            return 0;
        } else {
            return mWaitingActivities.front();
        }
    }

    vle::devs::Time init(const vle::devs::Time& /* time */)
    {
        mRunningActivity = 0;
        mPhase = WAIT;
        return vle::devs::Time::infinity;
    }

    void output(const vle::devs::Time& time,
                vle::devs::ExternalEventList& output) const
    {
        if (mPhase == SEND_DEMAND) {
            if (not mWaitingActivities.empty()) {
                Activity* a = select();
                ResourceConstraints rc = a->buildResourceConstraints();

                if (not rc.empty()) {
                    vle::devs::ExternalEvent* ee =
                        new vle::devs::ExternalEvent("demand");

                    std::cout << "[" << getModel().getParentName()
                              << ":" << getModelName()
                              << "] at " << time
                              << " - " << a->name()
                              << " - demand" << std::endl;

                    ee << vle::devs::attribute("resources", rc.toValue());
                    output.addEvent(ee);
                }
            }
        } else if (mPhase == SEND_PROCESS) {
            if (mRunningActivity)
            {
                vle::devs::ExternalEvent* ee =
                    new vle::devs::ExternalEvent("process");

                std::cout << "[" << getModel().getParentName()
                          << ":" << getModelName()
                          << "] at " << time
                          << " - " << mRunningActivity->name()
                          << " - process" << std::endl;

                ee << vle::devs::attribute("activity",
                                           mRunningActivity->toValue());
                output.addEvent(ee);
            }
        } else if (mPhase == SEND_SCHEDULE) {
            for(Activities::const_iterator it = mSchedulingActivities.begin();
                it != mSchedulingActivities.end(); ++it) {
                {
                    vle::devs::ExternalEvent* ee =
                        new vle::devs::ExternalEvent("schedule");

                    ee << vle::devs::attribute("location",
                                               (*it)->location().name());
                    ee << vle::devs::attribute("activity", (*it)->toValue());
                    output.addEvent(ee);
                }
            }
        } else if (mPhase == SEND_DONE) {
            for(Activities::const_iterator it = mDoneActivities.begin();
                it != mDoneActivities.end(); ++it) {

                std::cout << "[" << getModel().getParentName()
                          << ":" << getModelName()
                          << "] - " << (*it)->name()
                          << " - done" << std::endl;

                {
                    vle::devs::ExternalEvent* ee =
                        new vle::devs::ExternalEvent("done");

                    ee << vle::devs::attribute("activity", (*it)->toValue());
                    output.addEvent(ee);
                }
            }
        } else if (mPhase == SEND_RELEASE) {
            for(Activities::const_iterator it = mReleasedActivities.begin();
                it != mReleasedActivities.end(); ++it) {

                std::cout << "[" << getModel().getParentName()
                          << ":" << getModelName()
                          << "] - " << (*it)->name()
                          << " - release"
                          << " => "
                          << *(*it)->allocatedResources()
                          << std::endl;

                Resources* releasedResources = (*it)->releasedResources();

                if (not releasedResources->empty()) {
                    vle::devs::ExternalEvent* ee =
                        new vle::devs::ExternalEvent("release");

                    ee << vle::devs::attribute(
                        "resources", releasedResources->toValue());
                    output.addEvent(ee);
                }
            }
        }
    }

    vle::devs::Time timeAdvance() const
    {
        if (mPhase == SEND_DEMAND or mPhase == SEND_DONE or
            mPhase == SEND_PROCESS or mPhase == SEND_RELEASE or
            mPhase == SEND_SCHEDULE) {
            return 0;
        } else {
            return vle::devs::Time::infinity;
        }
    }

    void internalTransition(const vle::devs::Time& time)
    {
        if (mPhase == SEND_DEMAND) {
            Activity* a = select();

            if (a->buildResourceConstraints().empty()) {
                mRunningActivity = a;

                ActivityFIFO::iterator it = std::find(
                    mWaitingActivities.begin(),
                    mWaitingActivities.end(), a);

                mWaitingActivities.erase(it);
                mPhase = SEND_PROCESS;
            } else {
                mPhase = WAIT;
            }
        } else if (mPhase == SEND_DONE) {
            mDoneActivities.clear();
            if (mWaitingActivities.empty()) {
                mPhase = WAIT;
            } else {
                mPhase = SEND_DEMAND;
            }
        } else if (mPhase == SEND_PROCESS) {
            if (mRunningActivity) {
                delete mRunningActivity;
                mRunningActivity = 0;
            }
            if (mWaitingActivities.empty()) {
                mPhase = WAIT;
            } else {
                mPhase = SEND_DEMAND;
            }
        } else if (mPhase == SEND_RELEASE) {
            while (not mReleasedActivities.empty()) {
                Activity* a = mReleasedActivities.back();

                a->release();
                if (a->end()) {
                    mDoneActivities.push_back(a);
                    mPhase = SEND_DONE;
                } else {
                    if (mLocation == a->location().name()) {
                        mWaitingActivities.push_back(a);
                        mPhase = SEND_DEMAND;
                    } else {
                        mSchedulingActivities.push_back(a);
                        mPhase = SEND_SCHEDULE;
                    }
                }
                mReleasedActivities.pop_back();
            }
        } else if (mPhase == SEND_SCHEDULE) {
            mSchedulingActivities.clear();
            if (mWaitingActivities.empty()) {
                mPhase = WAIT;
            } else {
                mPhase = SEND_DEMAND;
            }
        }
    }

    void externalTransition(
        const vle::devs::ExternalEventList& events,
        const vle::devs::Time& time)
    {
        vle::devs::ExternalEventList::const_iterator it = events.begin();

        while (it != events.end()) {
            if ((*it)->onPort("schedule")) {
                if ((*it)->getStringAttributeValue("location") == mLocation) {
                    Activity* a =
                        Activity::build((*it)->getAttributeValue("activity"));

                    std::cout << "[" << getModel().getParentName()
                              << ":" << getModelName()
                              << "] at " << time
                              << " - schedule = " << a->current()->name()
                              << "/" << a->name() << std::endl;

                    mWaitingActivities.push_back(a);
                    mPhase = SEND_DEMAND;
                }
            } else if ((*it)->onPort("assign")) {
                Resources* r = Resources::build(
                    (*it)->getAttributeValue("resources"));
                Activity* a = select();

                a->assign(r);

                std::cout << "[" << getModel().getParentName()
                          << ":" << getModelName()
                          << "] - assign = " << r->size()
                          << " -> " << a->name()
                          << " -> ";
                if (a->allocatedResources()) {
                    std::cout << *a->allocatedResources();
                } else {
                    std::cout << "{}";

                }
                std::cout << std::endl;

                if (a->checkResourceConstraint()) {
                    mRunningActivity = a;

                    ActivityFIFO::iterator it = std::find(
                        mWaitingActivities.begin(),
                        mWaitingActivities.end(), a);

                    mWaitingActivities.erase(it);
                    mPhase = SEND_PROCESS;
                }
            } else if ((*it)->onPort("done")) {
                Activity* a =
                    Activity::build((*it)->getAttributeValue("activity"));

                mReleasedActivities.push_back(a);
                mPhase = SEND_RELEASE;
            }
            ++it;
        }
    }

    virtual vle::value::Value* observation(
        const vle::devs::ObservationEvent& event) const
    {
        if (event.onPort("waiting")) {
            vle::value::Set* list = new vle::value::Set;

            for(ActivityFIFO::const_iterator it =  mWaitingActivities.begin();
                it != mWaitingActivities.end(); ++it) {
                list->add(new vle::value::String((*it)->name()));
            }
            return list;
        }
        return 0;
    }

private:
    enum Phase { WAIT, SEND_DEMAND, SEND_DONE, SEND_PROCESS, SEND_RELEASE,
                 SEND_SCHEDULE };

    std::string mLocation;

    Phase mPhase;
    ActivityFIFO mWaitingActivities;
    Activity* mRunningActivity;
    Activities mDoneActivities;
    Activities mReleasedActivities;
    Activities mSchedulingActivities;
};

} // namespace rcpsp

DECLARE_NAMED_DYNAMICS(StepScheduler, rcpsp::StepScheduler);
