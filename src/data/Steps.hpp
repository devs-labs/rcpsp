/**
 * @file Steps.hpp
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

#ifndef __STEPS_HPP
#define __STEPS_HPP 1

#include <data/Step.hpp>

namespace rcpsp {

class Steps : public std::vector < Step* >
{
public:
    Steps()
    { }

    Steps(const Steps& s) : std::vector < Step* >(s)
    {
        for(const_iterator it = s.begin(); it != s.end(); ++it)
            push_back(new Step(**it));
    }

    Steps(const vle::value::Value* value)
    {
        const vle::value::Set* set =
            dynamic_cast < const vle::value::Set* >(value);

        for (unsigned int i = 0; i < set->size(); ++i) {
            push_back(new Step(set->get(i)));
        }
    }

    virtual ~Steps()
    { for(iterator it = begin(); it != end(); ++it) delete *it; }

    Steps::iterator find(const std::string& name);

    vle::value::Value* toValue() const
    {
        vle::value::Set* value = new vle::value::Set;

        for (const_iterator it = begin(); it != end(); ++it) {
            value->add((*it)->toValue());
        }
        return value;
    }
};

std::ostream& operator<<(std::ostream& o, const Steps& s);

} // namespace rcpsp

#endif
