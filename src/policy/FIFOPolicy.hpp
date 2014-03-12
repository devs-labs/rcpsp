/**
 * @file FIFOPolicy.hpp
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

#ifndef __FIFO_POLICY_HPP
#define __FIFO_POLICY_HPP 1

#include <policy/StepSchedulingPolicy.hpp>

namespace rcpsp {

class FIFOPolicy : public StepSchedulingPolicy
{
public:
    FIFOPolicy(WaitingActivities& waitingActivities) :
    StepSchedulingPolicy(waitingActivities), mSelectedActivity(0)
    { }

    virtual void add(Activity* a)
    { mWaitingActivities.push_back(a); }

    virtual bool another() const
    { return true; }

    virtual bool demand() const
    { return false; }

    virtual bool next()
    {
        if (mSelectedActivity == 0) {
            mSelectedActivityIt = mWaitingActivities.begin();
        }
        ++mSelectedActivityIt;
        if (mSelectedActivityIt != mWaitingActivities.end()) {
            mSelectedActivity = *mSelectedActivityIt;
            return true;
        } else {
            mSelectedActivity = 0;
            return false;
        }
    }

    virtual void reset()
    { mSelectedActivity = 0; }

    virtual Activity* select() const
    {
        return mSelectedActivity == 0 ? mWaitingActivities.front() :
            mSelectedActivity;
    }

private:
    WaitingActivities::iterator mSelectedActivityIt;
    Activity* mSelectedActivity;
};

} // namespace rcpsp

#endif
