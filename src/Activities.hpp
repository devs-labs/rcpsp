/**
 * @file Activities.hpp
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

#ifndef __ACTIVITIES_HPP
#define __ACTIVITIES_HPP 1

#include <ostream>
#include <list>
#include <vector>

#include <Activity.hpp>

namespace rcpsp {

class Activities : public std::vector < Activity* >
{
public:
    typedef std::vector < Activity* > result_t;

    Activities()
    { }

    Activities(const Activities& a) : std::vector < Activity* >(a)
    {
        for(const_iterator it = a.begin(); it != a.end(); ++it)
            push_back(new Activity(**it));
    }

    Activities(const vle::value::Value* value)
    {
        const vle::value::Set* set =
            dynamic_cast < const vle::value::Set* >(value);

        for (unsigned int i = 0; i < set->size(); ++i) {
            push_back(new Activity(set->get(i)));
        }
    }

    virtual ~Activities()
    { for(iterator it = begin(); it != end(); ++it) delete *it; }

    void clear()
    {
        for(iterator it = begin(); it != end(); ++it) delete *it;
        std::vector < Activity* >::clear();
    }

    void removeStartingActivities();

    void starting(const vle::devs::Time& time);

    const result_t& startingActivities() const
    { return mStartingActivities; }

    vle::value::Value* toValue() const
    {
        vle::value::Set* value = new vle::value::Set;

        for (const_iterator it = begin(); it != end(); ++it) {
            value->add((*it)->toValue());
        }
        return value;
    }

private:
    friend std::ostream& operator<<(std::ostream& o, const Activities& a);

    result_t mStartingActivities;
};

class ActivityFIFO : public std::list < Activity* >
{
public:
    ActivityFIFO()
    { }
};

} // namespace rcpsp

#endif
