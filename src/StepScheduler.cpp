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

#include <devs/StepScheduler.hpp>

namespace rcpsp {

class ActivityFIFO : public std::list < Activity* >
{
public:
    ActivityFIFO()
    { }
};

class StepScheduler : public devs::StepScheduler
{
public:
    StepScheduler(const vle::devs::DynamicsInit& init,
                  const vle::devs::InitEventList& events) :
        devs::StepScheduler(init, events)
    { }

    virtual void add(Activity* a)
    {
        mWaitingActivities.push_back(a);
    }

    virtual bool empty() const
    { return mWaitingActivities.empty(); }

    virtual void remove(Activity* a)
    {
        ActivityFIFO::iterator it = std::find(
            mWaitingActivities.begin(),
            mWaitingActivities.end(), a);

        mWaitingActivities.erase(it);
    }

    virtual Activity* select() const
    { return empty() ? 0 : mWaitingActivities.front(); }

    virtual vle::value::Value* observe() const
    {
        vle::value::Set* list = new vle::value::Set;

        for(ActivityFIFO::const_iterator it =  mWaitingActivities.begin();
            it != mWaitingActivities.end(); ++it) {
            list->add(new vle::value::String((*it)->name()));
        }
        return list;
    }

private:
    ActivityFIFO mWaitingActivities;
};

} // namespace rcpsp

DECLARE_DYNAMICS(rcpsp::StepScheduler);
