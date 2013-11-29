/**
 * @file devs/StepScheduler.cpp
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

#include <devs/StepScheduler.hpp>

#include <vle/utils/Trace.hpp>

namespace rcpsp { namespace devs {

StepScheduler::StepScheduler(const vle::devs::DynamicsInit& init,
                             const vle::devs::InitEventList& events) :
    vle::devs::Dynamics(init, events),
    mLocation(vle::value::toString(events.get("location")))
{ }

vle::devs::Time StepScheduler::init(const vle::devs::Time& /* time */)
{
    mRunningActivity = 0;
    mUnavailableResources = 0;
    mPhase = WAIT_SCHEDULE;
    return vle::devs::infinity;
}

void StepScheduler::output(const vle::devs::Time& time,
                           vle::devs::ExternalEventList& output) const
{
    if (mPhase == SEND_DEMAND) {
        if (not empty()) { // est ce utile ???
            Activity* a = select();
            ResourceConstraints rc = a->buildResourceConstraints();

            if (not rc.empty()) {
                vle::devs::ExternalEvent* ee =
                    new vle::devs::ExternalEvent("demand");

                TraceModel(vle::fmt(" [%1%:%2%] at %3% -> %4% - demand") %
                           getModel().getParentName() % getModelName() %
                           time % a->name());

                ee << vle::devs::attribute("resources", rc.toValue());
                output.push_back(ee);
            }
        }
    } else if (mPhase == SEND_PROCESS) {
        if (mRunningActivity) { // est ce utile ???
            vle::devs::ExternalEvent* ee =
                new vle::devs::ExternalEvent("process");

            TraceModel(vle::fmt(" [%1%:%2%] at %3% -> %4% - process") %
                       getModel().getParentName() % getModelName() %
                       time % mRunningActivity->name());

            ee << vle::devs::attribute("activity",
                                       mRunningActivity->toValue());
            output.push_back(ee);
        }
    } else if (mPhase == SEND_SCHEDULE) {
        for(Activities::const_iterator it = mSchedulingActivities.begin();
            it != mSchedulingActivities.end(); ++it) {
            vle::devs::ExternalEvent* ee =
                new vle::devs::ExternalEvent("schedule");

            ee << vle::devs::attribute("previous", mLocation);
            ee << vle::devs::attribute("location",
                                       (*it)->location().name());
            ee << vle::devs::attribute("activity", (*it)->toValue());
            output.push_back(ee);
        }
    } else if (mPhase == SEND_DONE) {
        for(Activities::const_iterator it = mDoneActivities.begin();
            it != mDoneActivities.end(); ++it) {

            TraceModel(vle::fmt(" [%1%:%2%] at %3% -> %4% - done") %
                       getModel().getParentName() % getModelName() %
                       time % (*it)->name());

            vle::devs::ExternalEvent* ee =
                new vle::devs::ExternalEvent("done");

            ee << vle::devs::attribute("activity", (*it)->toValue());
            output.push_back(ee);
        }
    } else if (mPhase == SEND_RELEASE) {
        for(Activities::const_iterator it = mReleasedActivities.begin();
            it != mReleasedActivities.end(); ++it) {

            TraceModel(vle::fmt(" [%1%:%2%] at %3% -> %4% - release => %5%")
                       % getModel().getParentName() % getModelName() %
                       time % (*it)->name() % *(*it)->allocatedResources());

            Resources* releasedResources = (*it)->releasedResources();

            if (not releasedResources->empty()) {
                vle::devs::ExternalEvent* ee =
                    new vle::devs::ExternalEvent("release");

		TraceModel(vle::fmt(" [%1%:%2%] at %3% -> %4% - release => %5%")
			   % getModel().getParentName() % getModelName() %
			   time % (*it)->name() % *releasedResources);

                ee << vle::devs::attribute(
                    "resources", releasedResources->toValue());
                output.push_back(ee);
            }
        }
    } else if (mPhase == SEND_OUT_DEMAND) {
        vle::devs::ExternalEvent* ee =
            new vle::devs::ExternalEvent("out_demand");

        ee << vle::devs::attribute("resources",
                                   mUnavailableResources->toValue());
        output.push_back(ee);
    }
}

vle::devs::Time StepScheduler::timeAdvance() const
{
    if (mPhase == SEND_DEMAND or mPhase == SEND_DONE or
        mPhase == SEND_OUT_DEMAND or mPhase == SEND_PROCESS or
        mPhase == SEND_RELEASE or mPhase == SEND_SCHEDULE) {
        return 0;
    } else {
        return vle::devs::infinity;
    }
}

void StepScheduler::internalTransition(const vle::devs::Time& /* time */)
{
    if (mPhase == SEND_DEMAND) {
        Activity* a = select();

        if (a->buildResourceConstraints().empty()) {
            mRunningActivity = a;
            remove(a);
            mPhase = SEND_PROCESS;
        } else {
            mPhase = WAIT_ASSIGN;
        }
    } else if (mPhase == SEND_DONE) {
        mDoneActivities.clear();
        if (empty()) {
            mPhase = WAIT_SCHEDULE;
        } else {
            mPhase = SEND_DEMAND;
        }
    } else if (mPhase == SEND_PROCESS) {
        if (mRunningActivity) {
            const Resources& r = *mRunningActivity->allocatedResources();

            for (unsigned int i = 0; i < r.size(); ++i) {
                ResourceTypes::iterator it = mUsedResources.find(r[i]->type());

                if (it == mUsedResources.end()) {
                    mUsedResources[r[i]->type()] = 0;
                }
                ++mUsedResources[r[i]->type()];
            }

            delete mRunningActivity;
            mRunningActivity = 0;
        }
	if (not mReleasedActivities.empty()) {
            mPhase = SEND_RELEASE;
	} else {
            if (empty()) {
                mPhase = WAIT_SCHEDULE;
            } else {
                mPhase = SEND_DEMAND;
            }
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
                    add(a);
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
        if (empty()) {
            mPhase = WAIT_SCHEDULE;
        } else {
            mPhase = SEND_DEMAND;
        }
    }
}

void StepScheduler::externalTransition(
    const vle::devs::ExternalEventList& events,
    const vle::devs::Time& time)
{
    vle::devs::ExternalEventList::const_iterator it = events.begin();

    while (it != events.end()) {
        if ((*it)->onPort("schedule")) {
            if (Location::get(*it) == mLocation) {
                Activity* a = Activity::build(Activity::get(*it));

                TraceModel(
                    vle::fmt(" [%1%:%2%] at %3% -> schedule = %4%/%5%") %
                    getModel().getParentName() % getModelName() %
                    time % a->current()->name() % a->name());

                add(a);
                if (mPhase == WAIT_SCHEDULE) {
                    mPhase = SEND_DEMAND;
                } else if (mPhase == WAIT_RESOURCE) {
                    if (a->buildResourceConstraints().empty()) {
                        mRunningActivity = a;
                        remove(a);
                        mPhase = SEND_PROCESS;
                    }
		}
            }
        } else if ((*it)->onPort("assign")) {
            Resources* r = Resources::build(Resources::get(*it));
            Activity* a = select();

            a->assign(r);

            if (a->allocatedResources()) {
                TraceModel(
                    vle::fmt(
                        " [%1%:%2%] at %3% -> assign = %4% -> %5% -> %6%")
                    % getModel().getParentName() % getModelName() %
                    time % r->size() % a->name() %
                    *a->allocatedResources());
            } else {
                TraceModel(
                    vle::fmt(
                        " [%1%:%2%] at %3% -> assign = %4% -> %5% -> {}")
                    % getModel().getParentName() % getModelName() %
                    time % r->size() % a->name());
            }

            if (a->checkResourceConstraint()) {
                mRunningActivity = a;
                remove(a);
                mPhase = SEND_PROCESS;
            }
        } else if ((*it)->onPort("done")) {
            Activity* a = Activity::build(Activity::get(*it));

            mReleasedActivities.push_back(a);
	    if (mPhase == WAIT_SCHEDULE or mPhase == WAIT_RESOURCE) {
                mPhase = SEND_RELEASE;
	    }
        }  else if ((*it)->onPort("unavailable")) {
            if (demand()) {
                mUnavailableResources = ResourceTypes::build(
                    ResourceTypes::get(*it));
                mPhase = SEND_OUT_DEMAND;
            } else {
                if (another()) {
                    next();
                    mPhase = SEND_DEMAND;
                } else {
                    if (not mReleasedActivities.empty()) {
                        mPhase = SEND_RELEASE;
                    } else {
                        mPhase = WAIT_RESOURCE;
                    }
                }
            }
        }
        ++it;
    }
}

void StepScheduler::confluentTransitions(
    const vle::devs::Time& time,
    const vle::devs::ExternalEventList& events)
{
    TraceModel(vle::fmt(" [%1%:%2%] at %3% -> confluent !") %
               getModel().getParentName() % getModelName() %
               time);

    internalTransition(time);
    externalTransition(events, time);
}

vle::value::Value* StepScheduler::observation(
    const vle::devs::ObservationEvent& event) const
{
    if (event.onPort("waiting")) {
        return observe();
    }
    return 0;
}

} } // namespace devs rcpsp
