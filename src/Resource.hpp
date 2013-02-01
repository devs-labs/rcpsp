/**
 * @file Resource.hpp
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

#ifndef __RESOURCE_HPP
#define __RESOURCE_HPP 1

#include <string>
#include <vector>

#include <vle/value/Set.hpp>
#include <vle/value/Value.hpp>

#include <Planning.hpp>

namespace rcpsp {

class Resource
{
public:
    Resource(const std::string& name, const std::string& type) :
        mName(name), mType(type)
    { }

    Resource(const vle::value::Value* value)
    {
        const vle::value::Set* set =
            dynamic_cast < const vle::value::Set* >(value);

        mName = vle::value::toString(set->get(0));
        mType = vle::value::toString(set->get(1));
    }

    const std::string& name() const
    { return mName; }

    vle::value::Value* toValue() const
    {
        vle::value::Set* value = new vle::value::Set;

        value->add(new vle::value::String(mName));
        value->add(new vle::value::String(mType));
        return value;
    }

    const std::string& type() const
    { return mType; }

private:
    friend std::ostream& operator<<(std::ostream& o, const Resource& r);

    std::string mName;
    std::string mType;
};

std::ostream& operator<<(std::ostream& o, const Resource& r);

} // namespace rcpsp

#endif
