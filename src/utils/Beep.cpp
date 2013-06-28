/**
 * @file Beep.cpp
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

#include <vle/devs/Dynamics.hpp>

namespace rcpsp {

class Beep : public vle::devs::Dynamics
{
public:
    Beep(const vle::devs::DynamicsInit& init,
         const vle::devs::InitEventList& events) :
        vle::devs::Dynamics(init, events)
    {
    }

    vle::devs::Time init(const vle::devs::Time& /* time */)
    { return 0.0; }

    vle::devs::Time timeAdvance() const
    { return vle::devs::infinity; }

    void output(const vle::devs::Time& /* time */,
                vle::devs::ExternalEventList& output) const
    { output.push_back(buildEvent("out")); }
};

} // namespace rcpsp

DECLARE_DYNAMICS(rcpsp::Beep);
