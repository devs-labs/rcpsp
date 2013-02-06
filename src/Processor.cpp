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

#include <devs/Processor.hpp>

#include <vle/utils/Trace.hpp>

namespace rcpsp {

class Processor : public devs::Processor
{
public:
    Processor(const vle::devs::DynamicsInit& init,
              const vle::devs::InitEventList& events) :
        devs::Processor(init, events)
    { }

    vle::devs::Time computeRemainingTime(const vle::devs::Time& time) const
    {
        vle::devs::Time min = vle::devs::infinity;

        for(Activities::const_iterator it = mRunningActivities.begin();
            it != mRunningActivities.end(); ++it) {
            vle::devs::Time rt = (*it)->remainingTime(time);

            if (rt < min) {
                min = rt;
            }
        }
        return min;
    }

    virtual void done(const vle::devs::Time& /* time */)
    { mDoneActivities.clear(); }

    virtual void finish(const vle::devs::Time& time)
    {
        Activities::iterator it = mRunningActivities.begin();

        while (it != mRunningActivities.end()) {
            if ((*it)->done(time)) {

                TraceModel(vle::fmt(" [%1%:%2%] at %3% -> %4% finishs") %
                           getModel().getParentName() % getModelName() %
                           time % (*it)->name());

                (*it)->finish(time);
                mDoneActivities.push_back(*it);
                mRunningActivities.erase(it);
                it = mRunningActivities.begin();
            } else {
                ++it;
            }
        }
    }

    virtual bool idle() const
    { return mRunningActivities.empty(); }

    virtual void start(const vle::devs::Time& time, Activity* a)
    {
        TraceModel(vle::fmt(" [%1%:%2%] at %3% -> %4% starts") %
                   getModel().getParentName() % getModelName() %
                   time % a->name());

        a->start(time);
        mRunningActivities.push_back(a);
    }

};

} // namespace rcpsp

DECLARE_DYNAMICS(rcpsp::Processor);
