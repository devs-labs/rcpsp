/**
 * @file Problem.hpp
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

#ifndef __PROBLEM__HPP
#define __PROBLEM_HPP 1

#include <string>
#include <vector>

#include <vle/value/Value.hpp>

namespace rcpsp {

typedef std::vector < std::string > resources_t;
typedef std::pair < std::string, resources_t > pool_t;
typedef std::map < std::string, pool_t > pools_t;

class Pools
{
public:
    Pools()
    { }

    Pools(const vle::value::Value* value)
    {
        const vle::value::Map* pools =
            dynamic_cast < const vle::value::Map* >(value);

        for (vle::value::Map::const_iterator it = pools->begin();
             it != pools->end(); ++it) {
            const vle::value::Set* pool =
                dynamic_cast < const vle::value::Set* >(it->second);
            const vle::value::Set* resources =
                dynamic_cast < const vle::value::Set* >(pool->get(1));
            resources_t resources_;

            for (unsigned int i = 0; i < resources->size(); ++i) {
                resources_.push_back(vle::value::toString(resources->get(i)));
            }
            mPools[it->first] = pool_t(vle::value::toString(pool->get(0)),
                                       resources_);
        }
    }

    const pools_t& pools() const
    { return mPools; }

    vle::value::Value* toValue() const
    {
        vle::value::Map* value = new vle::value::Map;

        for (pools_t::const_iterator it = mPools.begin(); it != mPools.end();
             ++it) {
            vle::value::Set* pool = new vle::value::Set();
            vle::value::Set* resources = new vle::value::Set();

            for (resources_t::const_iterator itr = it->second.second.begin();
                 itr != it->second.second.end(); ++itr) {
                resources->add(new vle::value::String(*itr));
            }
            pool->add(new vle::value::String(it->second.first));
            pool->add(resources);
            value->add(it->first, pool);
        }
        return value;
    }

private:
    pools_t mPools;
};

typedef std::map < std::string, Pools > locations_t;

class Locations
{
public:
    Locations()
    { }

    Locations(const vle::value::Value* value)
    {
        const vle::value::Map* locations =
            dynamic_cast < const vle::value::Map* >(value);

        for (vle::value::Map::const_iterator it = locations->begin();
             it != locations->end(); ++it) {
            mLocations[it->first] = Pools(it->second);
        }
    }

    const locations_t& locations() const
    { return mLocations; }

    vle::value::Value* toValue() const
    {
        vle::value::Map* value = new vle::value::Map;

        for (locations_t::const_iterator it = mLocations.begin();
             it != mLocations.end(); ++it) {
            value->add(it->first, it->second.toValue());
        }
        return value;
    }

private:
    locations_t mLocations;
};

} // namespace rcpsp

#endif
