/**
 * @file PoolConstructor.cpp
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

#include <vle/devs/Executive.hpp>

#include <Problem.hpp>

namespace rcpsp {

class PoolConstructor : public vle::devs::Executive
{
public:
    PoolConstructor(const vle::devs::ExecutiveInit& init,
                    const vle::devs::InitEventList& events) :
	vle::devs::Executive(init, events),
        mPools(events.get("pools"))
    {
    }

    virtual ~PoolConstructor() { }

    void createPool(const std::string& name, const pool_t& pool)
    {
        // condition
        vle::value::Set* value = new vle::value::Set;

        value->add(new vle::value::String(name));
        value->add(new vle::value::String(pool.first));

        vle::value::Set* resources = new vle::value::Set();

        for (resources_t::const_iterator itr = pool.second.begin();
             itr != pool.second.end(); ++itr) {
            vle::value::Set* resource = new vle::value::Set();

            resource->add(new vle::value::String(*itr));
            resource->add(new vle::value::String(pool.first));
            resources->add(resource);
        }
        value->add(resources);

        conditions().get("cond_pool").setValueToPort(
            "pool", *value);

        // create models
        createModelFromClass("Pool", name);

        // connections
        addConnection("assignment", "assign", name, "assign");
        addConnection("assignment", "demand", name, "demand");
        addConnection("assignment", "release", name, "release");

        addConnection(name, "available", "assignment", "available");
        addConnection(name, "assign", coupledmodelName(), "assign");
    }

    vle::devs::Time init(const vle::devs::Time& /* time */)
    {
        for (pools_t::const_iterator it = mPools.pools().begin();
             it != mPools.pools().end(); ++it) {
            createPool(it->first, it->second);
        }
        return vle::devs::infinity;
    }

private:
    Pools mPools;
};

} // namespace rcpsp

DECLARE_EXECUTIVE(rcpsp::PoolConstructor);
