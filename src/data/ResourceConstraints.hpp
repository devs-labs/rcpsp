/**
 * @file ResourceConstraints.hpp
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

#ifndef __RESOURCE_CONTRAINTS_HPP
#define __RESOURCE_CONTRAINTS_HPP 1

#include <string>
#include <vle/devs/Time.hpp>
#include <vle/value/Set.hpp>
#include <vle/value/Value.hpp>

#include <data/ResourceConstraint.hpp>
#include <data/Resources.hpp>

namespace rcpsp {

class ResourceConstraints : public std::vector < ResourceConstraint >
{
public:
    ResourceConstraints()
    { }

    ResourceConstraints(const vle::value::Value* value)
    {
        const vle::value::Set* set =
            dynamic_cast < const vle::value::Set* >(value);

        for (unsigned int i = 0; i < set->size(); ++i) {
            push_back(ResourceConstraint(set->get(i)));
        }
    }

    static ResourceConstraints* build(const vle::value::Value& value)
    { return new ResourceConstraints(&value); }

    ResourceConstraints buildResourceConstraints(const Resources* r) const;

    bool checkResourceConstraint(const Resources& r) const;

    unsigned int quantity(const std::string& type) const
    {
        const_iterator it = begin();
        bool found = false;

        while (it != end() and not found) {
            if (it->type() == type) {
                found = true;
            } else {
                ++it;
            }
        }
        if (found) {
            return it->quantity();
        } else {
            return 0;
        }
    }

    unsigned int quantity() const
    {
        unsigned int n = 0;

        for (const_iterator it = begin(); it != end(); ++it) {
            n += it->quantity();
        }
        return n;
    }

    bool needAgain(const std::string& type) const;

    vle::value::Value* toValue() const
    {
        vle::value::Set* value = new vle::value::Set;

        for (const_iterator it = begin(); it != end(); ++it) {
            value->add(it->toValue());
        }
        return value;
    }

};

} // namespace rcpsp

#endif
