/**
 * @file devs/Processor.hpp
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

#include <devs/Processor.hpp>

#include <vle/utils/Trace.hpp>

namespace rcpsp { namespace devs {

Processor::Processor(const vle::devs::DynamicsInit& init,
                     const vle::devs::InitEventList& events) :
    vle::devs::Dynamics(init, events)
{ }

vle::devs::Time Processor::init(const vle::devs::Time& /* time */)
{
    mPhase = IDLE;
    mSigma = vle::devs::infinity;
    return vle::devs::infinity;
}

void Processor::output(const vle::devs::Time& /* time */,
                       vle::devs::ExternalEventList& output) const
{
    if (mPhase == DONE) {
        for(Activities::const_iterator it = mDoneActivities.begin();
            it != mDoneActivities.end(); ++it) {
            vle::devs::ExternalEvent* ee =
                new vle::devs::ExternalEvent("done");

            ee << vle::devs::attribute("activity", (*it)->toValue());
            output.push_back(ee);
        }
    }
}

vle::devs::Time Processor::timeAdvance() const
{
    if (mPhase == IDLE) {
        return vle::devs::infinity;
    } else if (mPhase == RUNNING) {
        return mSigma;
    } else {
        return 0;
    }
}

void Processor::internalTransition(const vle::devs::Time& time)
{
    if (mPhase == RUNNING) {
        finish(time);
        mPhase = DONE;
    } else if (mPhase == DONE) {
        done(time);
        if (idle()) {
            mPhase = IDLE;
        } else {
            mSigma = computeRemainingTime(time);
            mPhase = RUNNING;
        }
    }
}

void Processor::externalTransition(const vle::devs::ExternalEventList& events,
                                   const vle::devs::Time& time)
{
    vle::devs::ExternalEventList::const_iterator it = events.begin();

    while (it != events.end()) {
        if ((*it)->onPort("process")) {
            Activity* a = Activity::build(Activity::get(*it));

            start(time, a);
            mPhase = RUNNING;
        }
        ++it;
    }
    mSigma = computeRemainingTime(time);
}

void Processor::confluentTransitions(
    const vle::devs::Time& time,
    const vle::devs::ExternalEventList& /* events */)
{
    TraceModel(vle::fmt(" [%1%:%2%] at %3% -> confluent !") %
               getModel().getParentName() % getModelName() %
               time);
}

vle::value::Value* Processor::observation(
    const vle::devs::ObservationEvent& event) const
{
    if (event.onPort("running_activity")) {
        return mRunningActivities.observe_activity();
    }
    if (event.onPort("running_step")) {
        return mRunningActivities.observe_step();
    }
    if (event.onPort("used_resources")) {
        return mRunningActivities.observe_used_resources();
    }
    if (event.onPort("used_resource_types")) {
        return mRunningActivities.observe_used_resource_types();
    }
    return 0;
}

} } // namespace devs rcpsp
