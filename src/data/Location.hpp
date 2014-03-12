/**
 * @file Location.hpp
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

#ifndef __LOCATION_HPP
#define __LOCATION_HPP 1

#include <string>
#include <vector>

#include <vle/devs/ExternalEvent.hpp>
#include <vle/value/String.hpp>
#include <vle/value/Value.hpp>

namespace rcpsp {

class Location
{
public:
    Location(const std::string& name = "") : mName(name)
    { }

    Location(const vle::value::Value* value)
    { mName = vle::value::toString(value); }

    static const std::string& get(const vle::devs::ExternalEvent* ee)
    { return ee->getStringAttributeValue("location"); }

    const std::string& name() const
    { return mName; }

    vle::value::Value* toValue() const
    { return new vle::value::String(mName); }

private:
    std::string mName;
};

typedef std::vector < Location > Path;

} // namespace rcpsp

#endif
