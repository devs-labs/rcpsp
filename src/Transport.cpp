/**
 * @file Transport.cpp
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
#include <data/Problem.hpp>

namespace rcpsp {

    class Transport : public vle::devs::Dynamics
    {
        typedef std::vector < Activity* > Activities;
        typedef std::vector < vle::devs::Time > Dates;

    public:
        Transport(const vle::devs::DynamicsInit& init,
                  const vle::devs::InitEventList& events) :
            vle::devs::Dynamics(init, events),
            mLocation(vle::value::toString(events.get("location"))),
            mDurations(events.get("durations"))
        {
        }

        vle::devs::Time init(const vle::devs::Time& /* time */)
        {
            mSigma = vle::devs::infinity;
            mLastTime = 0;
            return vle::devs::infinity;
        }

        void output(const vle::devs::Time& time,
                    vle::devs::ExternalEventList& output) const
        {
            unsigned int i = 0;

            for(Activities::const_iterator it = mActivities.begin();
                it != mActivities.end(); ++it, ++i) {

                if (time == mOutDates[i]) {
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

            TraceModel(
                vle::fmt(" [%1%:%2%] -> ta = %3% ; n = %4%") %
                getModel().getParentName() % getModelName() %
                mSigma % mOutDates.size());

            return mSigma;
        }

        void internalTransition(const vle::devs::Time& time)
        {
            TraceModel(
                vle::fmt(" [%1%:%2%] at %3% -> internalTransition") %
                getModel().getParentName() % getModelName() %
                time);

            Activities::iterator ita = mActivities.begin();
            Dates::iterator itd = mOutDates.begin();

            while (ita != mActivities.end()) {
                if (*itd == time) {
                    mOutDates.erase(itd);
                    mActivities.erase(ita);
                    ita = mActivities.begin();
                    itd = mOutDates.begin();
                } else {
                    ++ita;
                    ++itd;
                }
            }
            if (mOutDates.empty()) {
                mSigma = vle::devs::infinity;
            } else {
                double min = *min_element(mOutDates.begin(), mOutDates.end());

                mSigma = min - time;
            }
            mLastTime = time;
        }

        void externalTransition(
            const vle::devs::ExternalEventList& events,
            const vle::devs::Time& time)
        {
            vle::devs::ExternalEventList::const_iterator it = events.begin();

            TraceModel(
                vle::fmt(" [%1%:%2%] at %3% -> externalTransition") %
                getModel().getParentName() % getModelName() %
                time);

            while (it != events.end()) {
                if ((*it)->onPort("in")) {
                    if (Location::get(*it) == mLocation) {
                        Activity* a = Activity::build(Activity::get(*it));
                        vle::devs::Time outDate;

                        TraceModel(
                            vle::fmt(" [%1%:%2%] at %3% -> in = %4%/%5%") %
                            getModel().getParentName() % getModelName() %
                            time % a->current()->name() % a->name());

                        if (a->begin()) {
                            outDate = time;
                        } else {
                            std::string previousLocation =
                                (*it)->getStringAttributeValue("previous");

                            outDate = time + mDurations[previousLocation];

                            TraceModel(
                                vle::fmt(
                                    " [%1%:%2%] at %3% -> previous = %4%") %
                                getModel().getParentName() % getModelName() %
                                time % previousLocation);
                            TraceModel(
                                vle::fmt(
                                    " [%1%:%2%] at %3% -> duration = %4%") %
                                getModel().getParentName() % getModelName() %
                                time % mDurations[previousLocation]);
                            TraceModel(
                                vle::fmt(" [%1%:%2%] at %3% -> date = %4%") %
                                getModel().getParentName() % getModelName() %
                                time % outDate);

                        }

                        if (mOutDates.empty()) {
                            mSigma = outDate - time;
                        } else {
                            mSigma -= time - mLastTime;
                            if (outDate < time + mSigma) {
                                mSigma = outDate - time;
                            }
                        }
                        mOutDates.push_back(outDate);
                        mLastTime = time;

                        TraceModel(
                            vle::fmt(" [%1%:%2%] at %3% -> next date = %4%") %
                            getModel().getParentName() % getModelName() %
                            time % (time + mSigma));

                        mActivities.push_back(a);
                    } else {
                        if (not mOutDates.empty()) {
                            mSigma -= time - mLastTime;
                            mLastTime = time;
                        }
                    }

                }
                ++it;
            }
        }

        vle::value::Value* observation(
            const vle::devs::ObservationEvent& event) const
        {
            if (event.onPort("transport")) {
                vle::value::Set* value = new vle::value::Set;

                for(Activities::const_iterator it = mActivities.begin();
                    it != mActivities.end(); ++it) {
                    value->add(new vle::value::String((*it)->name()));
                }
                return value;
            }
            return 0;
        }

    private:
        // parameters
        std::string mLocation;
        Durations mDurations;

        // state
        vle::devs::Time mSigma;
        vle::devs::Time mLastTime;
        Activities mActivities;
        Dates mOutDates;
    };

} // namespace rcpsp

DECLARE_DYNAMICS(rcpsp::Transport);
