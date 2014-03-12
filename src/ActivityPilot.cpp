/**
 * @file ActivityPilot.cpp
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

#include <vle/devs/Dynamics.hpp>

namespace rcpsp {

    class ActivityPilot : public vle::devs::Dynamics
    {
    public:
        ActivityPilot(const vle::devs::DynamicsInit& init,
                      const vle::devs::InitEventList& events) :
            vle::devs::Dynamics(init, events)
        {
        }

        vle::devs::Time init(const vle::devs::Time& /* time */)
        {
            return vle::devs::infinity;
        }

        void output(const vle::devs::Time& /* time */,
                    vle::devs::ExternalEventList& output) const
        {
        }

        vle::devs::Time timeAdvance() const
        {
            return vle::devs::infinity;
        }

        void internalTransition(const vle::devs::Time& /* time */)
        {
        }

        void externalTransition(
            const vle::devs::ExternalEventList& events,
            const vle::devs::Time& time)
        {
            vle::devs::ExternalEventList::const_iterator it = events.begin();

            while (it != events.end()) {
                if ((*it)->onPort("start")) {
                }
                ++it;
            }
        }

        virtual vle::value::Value* observation(
            const vle::devs::ObservationEvent& event) const
        {
            return 0;
        }

    private:
        enum Phase { INIT, RUN, DONE };

        Phase mPhase;
    };

} // namespace rcpsp

DECLARE_DYNAMICS(rcpsp::ActivityPilot);
