/**
 * @file Transport.cpp
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

namespace rcpsp {

class Transport : public vle::devs::Dynamics
{
public:
    Transport(const vle::devs::DynamicsInit& init,
              const vle::devs::InitEventList& events) :
        vle::devs::Dynamics(init, events),
        mLocation(vle::value::toString(events.get("location")))
    {
    }

    vle::devs::Time init(const vle::devs::Time& /* time */)
    {
        mPhase = WAIT;
        return vle::devs::infinity;
    }

    void output(const vle::devs::Time& /* time */,
                vle::devs::ExternalEventList& output) const
    {
        if (mPhase == SEND) {
            for(Activities::result_t::const_iterator it = mActivities.begin();
                it != mActivities.end(); ++it) {
                vle::devs::ExternalEvent* ee =
                    new vle::devs::ExternalEvent("out");

                ee << vle::devs::attribute("location",
                                           (*it)->location().name());
                ee << vle::devs::attribute("activity", (*it)->toValue());
                output.push_back(ee);
            }
        }
    }

    vle::devs::Time timeAdvance() const
    {
        if (mPhase == SEND) return 0;
        else return vle::devs::infinity;
    }

    void internalTransition(const vle::devs::Time& /* time */)
    {
        if (mPhase == SEND) {
            mActivities.clear();
            mPhase = WAIT;
        }
    }

    void externalTransition(
        const vle::devs::ExternalEventList& events,
        const vle::devs::Time& time)
    {
        vle::devs::ExternalEventList::const_iterator it = events.begin();

        while (it != events.end()) {
            if ((*it)->onPort("in")) {
                if (Location::get(*it) == mLocation) {
                    Activity* a = Activity::build(Activity::get(*it));

                    TraceModel(
                        vle::fmt(" [%1%:%2%] at %3% -> in = %4%/%5%") %
                        getModel().getParentName() % getModelName() %
                        time % a->current()->name() % a->name());

                    mActivities.push_back(a);
                    mPhase = SEND;
                }
            }
            ++it;
        }
    }

private:
    enum Phase { WAIT, SEND };

    // parameters
    std::string mLocation;

    // state
    Phase mPhase;
    Activities mActivities;
};

} // namespace rcpsp

DECLARE_DYNAMICS(rcpsp::Transport);
