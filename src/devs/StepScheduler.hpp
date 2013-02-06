/**
 * @file StepScheduler.hpp
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

#include <vle/utils/Trace.hpp>

#include <data/Activities.hpp>

namespace rcpsp { namespace devs {

class StepScheduler : public vle::devs::Dynamics
{
public:
    StepScheduler(const vle::devs::DynamicsInit& init,
                  const vle::devs::InitEventList& events);

    virtual void add(Activity* a) =0;
    virtual bool empty() const =0;
    virtual void remove(Activity* a) =0;
    virtual Activity* select() const =0;
    virtual vle::value::Value* observe() const =0;

    virtual vle::devs::Time init(const vle::devs::Time& /* time */);
    virtual void output(const vle::devs::Time& time,
                        vle::devs::ExternalEventList& output) const;
    virtual vle::devs::Time timeAdvance() const;
    virtual void internalTransition(const vle::devs::Time& /* time */);
    virtual void externalTransition(
        const vle::devs::ExternalEventList& events,
        const vle::devs::Time& time);
    virtual vle::value::Value* observation(
        const vle::devs::ObservationEvent& event) const;

private:
    enum Phase { WAIT_SCHEDULE, WAIT_ASSIGN, WAIT_RESOURCE, SEND_DEMAND,
                 SEND_DONE, SEND_PROCESS, SEND_RELEASE, SEND_SCHEDULE };

    std::string mLocation;

    Phase mPhase;
    Activity* mRunningActivity;
    Activities mDoneActivities;
    Activities mReleasedActivities;
    Activities mSchedulingActivities;
};

} // namespace devs
} // namespace rcpsp
