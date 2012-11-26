/**
 * @file Processor.cpp
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

#include <vle/devs/Dynamics.hpp>

#include <Activities.hpp>

namespace rcpsp {

class Processor : public vle::devs::Dynamics
{
public:
    Processor(const vle::devs::DynamicsInit& init,
              const vle::devs::InitEventList& events) :
        vle::devs::Dynamics(init, events)
    {
    }

    vle::devs::Time init(const vle::devs::Time& /* time */)
    {
        mPhase = IDLE;
        mSigma = vle::devs::Time::infinity;
        return vle::devs::Time::infinity;
    }

    void output(const vle::devs::Time& /* time */,
                vle::devs::ExternalEventList& output) const
    {
        if (mPhase == DONE) {
            for(Activities::const_iterator it = mDoneActivities.begin();
                it != mDoneActivities.end(); ++it) {
                vle::devs::ExternalEvent* ee =
                    new vle::devs::ExternalEvent("done");

                ee << vle::devs::attribute("activity", (*it)->toValue());
                output.addEvent(ee);
            }
        }
    }

    vle::devs::Time timeAdvance() const
    {
        if (mPhase == IDLE) {
            return vle::devs::Time::infinity;
        } else if (mPhase == RUNNING) {
            return mSigma;
        } else {
            return 0;
        }
    }

    void internalTransition(const vle::devs::Time& time)
    {
        if (mPhase == RUNNING) {
            Activities::iterator it = mRunningActivities.begin();

            while (it != mRunningActivities.end()) {
                if ((*it)->done(time)) {

                    std::cout << getModel().getParentName() << " -> "
                              << time << ": " << (*it)->name() << " finishs"
                              << std::endl;

                    (*it)->finish(time);
                    mDoneActivities.push_back(*it);
                    mRunningActivities.erase(it);
                    it = mRunningActivities.begin();
                } else {
                    ++it;
                }
            }
            mPhase = DONE;
        } else if (mPhase == DONE) {
            mDoneActivities.clear();
            if (mRunningActivities.empty()) {
                mPhase = IDLE;
            } else {
                computeRemainingTime(time);
                mPhase = RUNNING;
            }
        }
    }

    void externalTransition(
        const vle::devs::ExternalEventList& events,
        const vle::devs::Time& time)
    {
        vle::devs::ExternalEventList::const_iterator it = events.begin();

        while (it != events.end()) {
            if ((*it)->onPort("process")) {
                Activity* a =
                    Activity::build((*it)->getAttributeValue("activity"));

                std::cout << getModel().getParentName() << " -> "
                          << time << ": " << a->name() << " starts"
                          << std::endl;

                a->start(time);
                mRunningActivities.push_back(a);
                mPhase = RUNNING;
            }
            ++it;
        }
        computeRemainingTime(time);
    }

    virtual vle::value::Value* observation(
        const vle::devs::ObservationEvent& event) const
    {
        if (event.onPort("running_activity")) {
            vle::value::Set* list = new vle::value::Set;

            for (Activities::const_iterator it = mRunningActivities.begin();
                 it != mRunningActivities.end(); ++it) {
                list->add(new vle::value::String((*it)->name()));
            }
            return list;
        }
        if (event.onPort("running_step")) {
            vle::value::Set* list = new vle::value::Set;

            for (Activities::const_iterator it = mRunningActivities.begin();
                 it != mRunningActivities.end(); ++it) {
                list->add(new vle::value::String((*it)->current()->name()));
            }
            return list;
        }
        if (event.onPort("used_resources")) {
            vle::value::Set* list = new vle::value::Set;

            for (Activities::const_iterator it = mRunningActivities.begin();
                 it != mRunningActivities.end(); ++it) {
                const Resources* resources = (*it)->allocatedResources();

                for (Resources::const_iterator itr = resources->begin();
                     itr != resources->end(); ++itr) {
                    list->add(new vle::value::String((*itr)->name()));
                }
            }
            return list;
        }
        if (event.onPort("used_resource_types")) {
            std::map < std::string, int > resourceNumber;

            for (Activities::const_iterator it = mRunningActivities.begin();
                 it != mRunningActivities.end(); ++it) {
                const Resources* resources = (*it)->allocatedResources();

                for (Resources::const_iterator itr = resources->begin();
                     itr != resources->end(); ++itr) {
                    if (resourceNumber.find((*itr)->type()) ==
                        resourceNumber.end()) {
                        resourceNumber[(*itr)->type()] = 0;
                    }
                    resourceNumber[(*itr)->type()]++;
                }
            }

            vle::value::Set* list = new vle::value::Set;

            for (std::map < std::string, int >::const_iterator it =
                     resourceNumber.begin(); it != resourceNumber.end(); ++it) {
                vle::value::Set* value = new vle::value::Set;

                value->add(new vle::value::String(it->first));
                value->add(new vle::value::Integer(it->second));
                list->add(value);
            }
            return list;
        }
        return 0;
    }

private:
    void computeRemainingTime(const vle::devs::Time& time)
    {
        vle::devs::Time min = vle::devs::Time::infinity;

        for(Activities::const_iterator it = mRunningActivities.begin();
            it != mRunningActivities.end(); ++it) {
            vle::devs::Time rt = (*it)->remainingTime(time);

            if (rt < min) {
                min = rt;
            }
        }
        mSigma = min;
    }

    enum Phase { IDLE, RUNNING, DONE };

    Phase mPhase;
    Activities mRunningActivities;
    Activities mDoneActivities;
    vle::devs::Time mSigma;
};

} // namespace rcpsp

DECLARE_NAMED_DYNAMICS(Processor, rcpsp::Processor);
