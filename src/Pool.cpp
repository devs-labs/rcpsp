/**
 * @file Pool.cpp
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

#include <Resource.hpp>

namespace rcpsp {

class Pool : public vle::devs::Dynamics
{
public:
    Pool(const vle::devs::DynamicsInit& init,
         const vle::devs::InitEventList& events) :
        vle::devs::Dynamics(init, events),
        mPool(events.get("pool"))
    {
    }

    vle::devs::Time init(const vle::devs::Time& /* time */)
    {
        mDeliveredResources = 0;
        mPhase = WAIT;
        return vle::devs::Time::infinity;
    }

    void output(const vle::devs::Time& /* time */,
                vle::devs::ExternalEventList& output) const
    {
        if (mPhase == SEND) {
            if (mDeliveredResources) { // TODO: ???
                vle::devs::ExternalEvent* ee =
                    new vle::devs::ExternalEvent("available");

                ee << vle::devs::attribute("resources",
                                           mDeliveredResources->toValue());
                output.addEvent(ee);
            }
        }
    }

    vle::devs::Time timeAdvance() const
    {
        if (mPhase == SEND) {
            return 0;
        } else {
            return vle::devs::Time::infinity;
        }
    }

    void internalTransition(const vle::devs::Time& /* time */)
    {
        if (mPhase == SEND) {
            delete mDeliveredResources;
            mDeliveredResources = 0;
            mPhase = WAIT;
        }
    }

    void externalTransition(
        const vle::devs::ExternalEventList& events,
        const vle::devs::Time& time)
    {
        vle::devs::ExternalEventList::const_iterator it = events.begin();

        while (it != events.end()) {
            if ((*it)->onPort("assign")) {

                // TODO

            } else if ((*it)->onPort("demand")) {
                if (mPool.type() == (*it)->getStringAttributeValue("type")) {
                    int quantity = (*it)->getIntegerAttributeValue("quantity");

                    std::cout << "[" << getModel().getParentName()
                              << ":" << getModelName()
                              << "] - " << mPool.type()
                              << ": demand = " << quantity
                              << "/" << mPool.quantity() << std::endl;

                    if (quantity <= mPool.quantity()) {
                        mDeliveredResources = mPool.assign(quantity);
                    }
                    mPhase = SEND;
                }
            } else if ((*it)->onPort("release")) {
                Resources* r = Resources::build(
                    (*it)->getAttributeValue("resources"));

                std::cout << "[" << getModel().getParentName()
                          << ":" << getModelName()
                          << "] - " << mPool.type()
                          << ": release = " << *r << std::endl;

                mPool.release(r);
            }
            ++it;
        }
    }

private:
    enum Phase { WAIT, SEND };

    Phase mPhase;
    ResourcePool mPool;
    Resources* mDeliveredResources;
};

} // namespace rcpsp

DECLARE_NAMED_DYNAMICS(Pool, rcpsp::Pool);
