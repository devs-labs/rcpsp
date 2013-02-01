/**
 * @file Dispatcher.cpp
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

class Dispatcher : public vle::devs::Dynamics
{
public:
    Dispatcher(const vle::devs::DynamicsInit& init,
               const vle::devs::InitEventList& events) :
        vle::devs::Dynamics(init, events)
    {
    }

    vle::devs::ExternalEvent* cloneExternalEvent(
        vle::devs::ExternalEvent* event, const std::string& portName) const
    {
        vle::devs::ExternalEvent* ee = new vle::devs::ExternalEvent(
            portName);
        vle::value::Map::const_iterator it = event->getAttributes().begin();

        while (it != event->getAttributes().end()) {
            ee->putAttribute(it->first, it->second->clone());
            ++it;
        }
        return ee;
    }

    vle::devs::Time init(const vle::devs::Time& /* time */)
    {
        mPhase = IDLE;
        return vle::devs::infinity;
    }

    void output(const vle::devs::Time& /* time */,
                vle::devs::ExternalEventList& output) const
    {
        for (events::const_iterator it = mEvents.begin(); it != mEvents.end();
             ++it) {
            output.addEvent(*it);
        }
    }

    vle::devs::Time timeAdvance() const
    {
        if (mPhase == IDLE) {
            return vle::devs::infinity;
        } else {
            return 0;
        }
    }

    void internalTransition(const vle::devs::Time& /* time */)
    {
        mEvents.clear();
        mPhase = IDLE;
    }

    void externalTransition(
        const vle::devs::ExternalEventList& events,
        const vle::devs::Time& /* time */)
    {
        vle::devs::ExternalEventList::const_iterator it = events.begin();

        while (it != events.end()) {
            std::string location = (*it)->getStringAttributeValue("location");
            vle::devs::ExternalEvent* ee = cloneExternalEvent(*it, location);

            mEvents.push_back(ee);
            mPhase = SEND;
            ++it;
        }
    }

private:
    enum phase { IDLE, SEND };

    typedef std::list < vle::devs::ExternalEvent* > events;

    phase mPhase;
    events mEvents;
};

} // namespace rcpsp

DECLARE_DYNAMICS(rcpsp::Dispatcher);
