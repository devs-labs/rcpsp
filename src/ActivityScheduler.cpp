/**
 * @file ActivityScheduler.cpp
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

#include <Activity.hpp>
#include <PrecedencesGraph.hpp>

namespace rcpsp {

class ActivityScheduler : public vle::devs::Dynamics
{
public:
    ActivityScheduler(const vle::devs::DynamicsInit& init,
                      const vle::devs::InitEventList& events) :
        vle::devs::Dynamics(init, events),
        mActivities(events.get("activities"))
    {
    }

    vle::devs::Time init(const vle::devs::Time& /* time */)
    {
        mPhase = INIT;
        return vle::devs::infinity;
    }

    void output(const vle::devs::Time& /* time */,
                vle::devs::ExternalEventList& output) const
    {
        if (mPhase == SEND) {
            const Activities::result_t& activities =
                mActivities.startingActivities();

            for(Activities::result_t::const_iterator it = activities.begin();
                it != activities.end(); ++it) {
                vle::devs::ExternalEvent* ee =
                    new vle::devs::ExternalEvent((*it)->location().name());

                ee << vle::devs::attribute("location",
                                           (*it)->location().name());
                ee << vle::devs::attribute("activity", (*it)->toValue());
                output.addEvent(ee);
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
            mPhase = WAIT;
        }
    }

    void externalTransition(
        const vle::devs::ExternalEventList& events,
        const vle::devs::Time& time)
    {
        vle::devs::ExternalEventList::const_iterator it = events.begin();

        while (it != events.end()) {
            if ((*it)->onPort("start")) {
                if (mPhase == INIT) {
                    mActivities.starting(time);
                    if (mActivities.startingActivities().empty()) {
                        mPhase = WAIT;
                    } else {
                        mPhase = SEND;
                    }
                }
            } else if ((*it)->onPort("done")) {
                Activity* a =
                    Activity::build((*it)->getAttributeValue("activity"));

                TraceModel(vle::fmt(" [%1%:%2%] at %3% -> %4% DONE") %
                           getModel().getParentName() % getModelName() %
                           time % a->name());

                mDoneActivities.push_back(a);
            }
            ++it;
        }
    }

private:
    enum Phase { INIT, WAIT, SEND };

    Phase mPhase;

    Activities mActivities;
    Activities mDoneActivities;
    PrecedencesGraph mPrecedencesGraph;
};

} // namespace rcpsp

DECLARE_DYNAMICS(rcpsp::ActivityScheduler);
