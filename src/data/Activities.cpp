/**
 * @file Activities.cpp
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

#include <iostream>

#include <data/Activities.hpp>

namespace rcpsp {

vle::devs::Time Activities::next(const vle::devs::Time& time) const
{
    vle::devs::Time min = vle::devs::infinity;

    for (Activities::const_iterator it = begin(); it != end(); ++it) {
        if (min > (*it)->temporalConstraints().earlyStartTime()) {
            min = (*it)->temporalConstraints().earlyStartTime();
        }
    }
    return min - time;
}

void Activities::removeStartingActivities()
{
    for (Activities::const_iterator it = mStartingActivities.begin();
         it != mStartingActivities.end(); ++it) {
        for (Activities::iterator it2 = begin(); it2 != end(); ++it2) {
            if (*it == *it2) {
                erase(it2);
                break;
            }
        }
    }
    mStartingActivities.clear();
}

void Activities::starting(const vle::devs::Time& time)
{
    for (Activities::const_iterator it = begin(); it != end(); ++it) {
        if ((*it)->starting(time)) {
            (*it)->wait(time);
            mStartingActivities.push_back(*it);
        }
    }
}

std::ostream& operator<<(std::ostream& o, const Activities& a)
{
    o << "{ ";
    for (Activities::const_iterator it = a.begin(); it != a.end(); ++it) {
        o << **it << " ";
    }
    o << "}";
    return o;
}

} // namespace rcpsp
