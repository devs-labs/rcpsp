/**
 * @file Resources.hpp
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

#ifndef __RESOURCES_HPP
#define __RESOURCES_HPP 1

#include <string>
#include <vector>

#include <vle/devs/ExternalEvent.hpp>
#include <vle/value/Value.hpp>

#include <data/Resource.hpp>

namespace rcpsp {

class Resources : public std::vector < Resource* >
{
public:
    Resources()
    { }

    Resources(const vle::value::Value* value)
    {
        const vle::value::Set* set =
            dynamic_cast < const vle::value::Set* >(value);

        for (unsigned int i = 0; i < set->size(); ++i) {
            push_back(new Resource(set->get(i)));
        }
    }

    virtual ~Resources()
    { }

    void add(const Resources& r)
    {
        for (Resources::const_iterator itr = r.begin(); itr != r.end();
             ++itr) {
            push_back(*itr);
        }
    }

    static Resources* build(const vle::value::Value& value)
    { return new Resources(&value); }

    static const vle::value::Value& get(const vle::devs::ExternalEvent* ee)
    { return ee->getAttributeValue("resources"); }

    vle::value::Value* toValue() const
    {
        vle::value::Set* value = new vle::value::Set;

        for (const_iterator it = begin(); it != end(); ++it) {
            value->add((*it)->toValue());
        }
        return value;
    }
};

std::ostream& operator<<(std::ostream& o, const Resources& r);

class ResourceTypes : public std::map < std::string, unsigned int >
{
public:
    ResourceTypes()
    { }

    ResourceTypes(const vle::value::Value* value)
    {
        const vle::value::Map* map =
            dynamic_cast < const vle::value::Map* >(value);

        for (vle::value::Map::const_iterator it = map->begin();
             it != map->end(); ++it) {
            insert(std::make_pair(it->first,
                                  vle::value::toInteger(it->second)));
        }
    }

    static ResourceTypes* build(const vle::value::Value& value)
    { return new ResourceTypes(&value); }

    static const vle::value::Value& get(const vle::devs::ExternalEvent* ee)
    { return ee->getAttributeValue("resources"); }

    vle::value::Value* toValue() const
    {
        vle::value::Map* value = new vle::value::Map;

        for (const_iterator it = begin(); it != end(); ++it) {
            value->add(it->first, new vle::value::Integer(it->second));
        }
        return value;
    }
};

} // namespace rcpsp

#endif
