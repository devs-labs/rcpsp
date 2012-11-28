/**
 * @file Constructor.cpp
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

#include <fstream>

namespace rcpsp {

class Constructor : public vle::devs::Executive
{
public:
    Constructor(const vle::devs::ExecutiveInit& init,
                const vle::devs::InitEventList& events) :
	vle::devs::Executive(init, events),
        mLocations(events.get("locations"))
    { }

    virtual ~Constructor() { }

    void createLocation(const std::string& name, const Pools& pools)
    {
        // condition
        conditions().get("cond_step_scheduler").setValueToPort(
            "location", vle::value::String(name));
        conditions().get("cond_pool_constructor").setValueToPort(
            "pools", *pools.toValue());

        // create models
        createModelFromClass("Location", name);

        // ports
        addOutputPort("scheduler", name);

        // connections
        addConnection("scheduler", name, name, "schedule");
        addConnection(name, "done", "scheduler", "done");
    }

    void createNetwork()
    {
        for (locations_t::const_iterator it = mLocations.locations().begin();
             it != mLocations.locations().end(); ++it) {
            for (locations_t::const_iterator it2 =
                     mLocations.locations().begin();
                 it2 != mLocations.locations().end(); ++it2) {
                if (it->first != it2->first) {
                    addConnection(it->first, "schedule",
                                  it2->first, "schedule");
                }
            }
        }
    }

    vle::devs::Time init(const vle::devs::Time& /* time */)
    {
        for (locations_t::const_iterator it = mLocations.locations().begin();
             it != mLocations.locations().end(); ++it) {
            createLocation(it->first, it->second);
        }
        createNetwork();
        return 0;
    }

    void output(const vle::devs::Time& /* time */,
                vle::devs::ExternalEventList& output) const
    {
        output.addEvent(buildEvent("start"));
    }

    vle::devs::Time timeAdvance() const
    {
        return vle::devs::Time::infinity;
    }

    void internalTransition(const vle::devs::Time& /* time */)
    {
        std::ofstream file("output.vpz");
        dump(file, "experiment");
    }

private:
    Locations mLocations;
};

} // namespace rcpsp

DECLARE_NAMED_EXECUTIVE(Constructor, rcpsp::Constructor);
