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
    StepSchedulingPolicy(waitingActivities)
    { }

    virtual void add(Activity* a)
    { mWaitingActivities.push_back(a); }

    virtual bool another() const
    { return false; }

    virtual void next()
    { }

    virtual Activity* select() const
    { return mWaitingActivities.empty() ? 0 : mWaitingActivities.front(); }
};

} // namespace rcpsp

#endif
