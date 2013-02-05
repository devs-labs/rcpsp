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

#include <vle/utils/Trace.hpp>

#include <ResourcePool.hpp>

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
        mAvailable = false;
        mDeliveredResources = 0;
        mPhase = WAIT;
        return vle::devs::infinity;
    }

    void output(const vle::devs::Time& time,
                vle::devs::ExternalEventList& output) const
    {
        if (mPhase == SEND_AVAILABLE) {
            vle::devs::ExternalEvent* ee =
                new vle::devs::ExternalEvent("available");

            TraceModel(vle::fmt(" [%1%:%2%] at %3% -> %4% send available: %5%")
                       % getModel().getParentName() % getModelName() %
                       time % mPool.type() % mAvailableNumber);

            ee << vle::devs::attribute("available", mAvailable);
            ee << vle::devs::attribute("number", mAvailableNumber);
            output.addEvent(ee);
        } else if (mPhase == SEND_ASSIGN) {
            vle::devs::ExternalEvent* ee =
                new vle::devs::ExternalEvent("assign");

            TraceModel(vle::fmt(" [%1%:%2%] at %3% -> %4% send assign: %5%")
                       % getModel().getParentName() % getModelName() %
                       time % mPool.type() % mDeliveredResources->size());

            ee << vle::devs::attribute("resources",
                                       mDeliveredResources->toValue());
            output.addEvent(ee);
        }
    }

    vle::devs::Time timeAdvance() const
    {
        if (mPhase == SEND_ASSIGN or mPhase == SEND_AVAILABLE) {
            return 0;
        } else {
            return vle::devs::infinity;
        }
    }

    void internalTransition(const vle::devs::Time& /* time */)
    {
        if (mPhase == SEND_AVAILABLE) {
            mAvailable = false;
            mAvailableNumber = 0;
            mPhase = WAIT;
        } else if (mPhase == SEND_ASSIGN) {
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
                if (mPool.type() == (*it)->getStringAttributeValue("type")) {
                    int quantity = (*it)->getIntegerAttributeValue("quantity");

                    TraceModel(vle::fmt(
                                   " [%1%:%2%] at %3% -> %4% assign: %5% / %6%")
                               % getModel().getParentName() % getModelName() %
                               time % mPool.type() % quantity %
                               mPool.quantity());

                    mDeliveredResources = mPool.assign(quantity);
                    mPhase = SEND_ASSIGN;
                }
            } else if ((*it)->onPort("demand")) {
                if (mPool.type() == (*it)->getStringAttributeValue("type")) {
                    int quantity = (*it)->getIntegerAttributeValue("quantity");

                    TraceModel(vle::fmt(
                                   " [%1%:%2%] at %3% -> %4% demand: %5% / %6%")
                               % getModel().getParentName() % getModelName() %
                               time % mPool.type() % quantity %
                               mPool.quantity());

                    if (quantity <= mPool.quantity()) {
                        mAvailable = true;
                        mAvailableNumber = quantity;
                    } else {
                        mAvailable = false;
                        mAvailableNumber = 0;
                    }
                    mPhase = SEND_AVAILABLE;
                }
            } else if ((*it)->onPort("release")) {
                Resources* r = Resources::build(
                    (*it)->getAttributeValue("resources"));

                TraceModel(vle::fmt(
                               " [%1%:%2%] at %3% -> %4% release: %5%")
                           % getModel().getParentName() % getModelName() %
                           time % mPool.type() % *r);

                mPool.release(r);
                r->clear();
                delete r;
                mPhase = WAIT;
            }
            ++it;
        }
    }

    virtual vle::value::Value* observation(
        const vle::devs::ObservationEvent& event) const
    {
        if (event.onPort("available_resources")) {
            vle::value::Set* list = new vle::value::Set;

            for (Resources::const_iterator it = mPool.available()->begin();
                 it != mPool.available()->end(); ++it) {
               list->add(new vle::value::String((*it)->name()));
            }
           return list;
        }
        return 0;
    }

private:
    enum Phase { WAIT, SEND_ASSIGN, SEND_AVAILABLE };

    Phase mPhase;
    ResourcePool mPool;
    bool mAvailable;
    int mAvailableNumber;
    Resources* mDeliveredResources;
};

} // namespace rcpsp

DECLARE_DYNAMICS(rcpsp::Pool);
