/**
 * @file StepSchedulingPolicy.hpp
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

#ifndef __STEP_SCHEDULING_POLICY_HPP
#define __STEP_SCHEDULING_POLICY_HPP 1

namespace rcpsp {

#include <data/Activities.hpp>

class StepSchedulingPolicy
{
public:
    StepSchedulingPolicy(WaitingActivities& waitingActivities) :
    mWaitingActivities(waitingActivities)
    { }

    virtual void add(Activity* a) =0;
    virtual bool another() const =0;
    virtual bool demand() const =0;
    virtual void next() =0;
    virtual Activity* select() const =0;

protected:
    WaitingActivities& mWaitingActivities;
};

} // namespace rcpsp

#endif
