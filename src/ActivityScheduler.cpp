/**
 * @file ActivityScheduler.cpp
 * @author The VLE Development Team
 * See the AUTHORS or Authors.txt file
 */

/*
 * Copyright (C) 2012-2014 ULCO http://www.univ-littoral.fr
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

#include <data/Activity.hpp>
#include <data/PrecedencesGraph.hpp>

#include <iostream>

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

        vle::devs::Time init(const vle::devs::Time& time)
        {
            mPhase = INIT;
            mLastTime = time;
            mSigma = 0;
            return vle::devs::infinity;
        }

        void output(const vle::devs::Time& /* time */,
                    vle::devs::ExternalEventList& output) const
        {
            if (mPhase == SEND) {
                const Activities::result_t& activities =
                    mActivities.startingActivities();

                for(Activities::result_t::const_iterator it =
                        activities.begin();
                    it != activities.end(); ++it) {
                    vle::devs::ExternalEvent* ee =
                        new vle::devs::ExternalEvent((*it)->location().name());

                    ee << vle::devs::attribute("location",
                                               (*it)->location().name());
                    ee << vle::devs::attribute("activity", (*it)->toValue());
                    output.push_back(ee);
                }
            }
        }

        vle::devs::Time timeAdvance() const
        {
            if (mPhase == SEND) {
                return 0;
            } else if (mPhase == WAIT) {
                return mSigma;
            } else {
                return vle::devs::infinity;
            }
        }

        void internalTransition(const vle::devs::Time& time)
        {
            if (mPhase == SEND) {
                const Activities::result_t& activities =
                    mActivities.startingActivities();

                for(Activities::result_t::const_iterator it =
                        activities.begin();
                    it != activities.end(); ++it) {
                    mRunningActivities.push_back(*it);
                }
                mActivities.removeStartingActivities();
                if (mActivities.empty()) {
                    mPhase = DONE;
                } else {
                    mSigma = mActivities.next(time);
                    mLastTime = time;
                    mPhase = WAIT;
                }
            } else if (mPhase == WAIT) {
                mActivities.starting(time);
                mPhase = SEND;
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
                    Activity* a = Activity::build(Activity::get(*it));

                    TraceModel(vle::fmt(" [%1%:%2%] at %3% -> %4% DONE") %
                               getModel().getParentName() % getModelName() %
                               time % a->name());

                    mDoneActivities.push_back(a);
                }
                ++it;
            }
            if (mSigma > 0) {
                mSigma -= (time - mLastTime);
                mLastTime = time;
            }
        }

        void confluentTransitions(
            const vle::devs::Time& time,
            const vle::devs::ExternalEventList& /* events */)
        {
            TraceModel(vle::fmt(" [%1%:%2%] at %3% -> confluent !") %
                       getModel().getParentName() % getModelName() %
                       time);
        }

    private:
        enum Phase { DONE, INIT, WAIT, SEND };

        Phase mPhase;
        vle::devs::Time mLastTime;
        vle::devs::Time mSigma;

        Activities mActivities;
        Activities mRunningActivities;
        Activities mDoneActivities;
        PrecedencesGraph mPrecedencesGraph;
    };

} // namespace rcpsp

DECLARE_DYNAMICS(rcpsp::ActivityScheduler);
