/**
 * @file Assignment.cpp
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

#include <Resources.hpp>
#include <ResourceConstraints.hpp>

namespace rcpsp {

class Assignment : public vle::devs::Dynamics
{
public:
    Assignment(const vle::devs::DynamicsInit& init,
              const vle::devs::InitEventList& events) :
        vle::devs::Dynamics(init, events)
    {
    }

    vle::devs::Time init(const vle::devs::Time& /* time */)
    {
        mResourceConstraints = 0;
        mDeliveredResources = 0;
        mReleasedResources = 0;
        mPhase = WAIT;
        return vle::devs::Time::infinity;
    }

    void output(const vle::devs::Time& /* time */,
                vle::devs::ExternalEventList& output) const
    {
        if (mPhase == SEND_ASSIGN) {
            vle::devs::ExternalEvent* ee =
                new vle::devs::ExternalEvent("assign");

            ee << vle::devs::attribute("resources",
                                       mDeliveredResources->toValue());
            output.addEvent(ee);
        } else if (mPhase == SEND_DEMAND) {
            for (ResourceConstraints::const_iterator it =
                     mResourceConstraints->begin();
                 it != mResourceConstraints->end(); ++it) {
                vle::devs::ExternalEvent* ee =
                    new vle::devs::ExternalEvent("demand");

                 std::cout << "[" << getModel().getParentName()
                           << ":" << getModelName()
                           << "] - demand => "
                           << it->type() << " = " << it->quantity()
                           << std::endl;

               ee << vle::devs::attribute("type", it->type());
                ee << vle::devs::attribute("quantity", (int)it->quantity());
                output.addEvent(ee);
            }
        } else if (mPhase == SEND_RELEASE) {
            vle::devs::ExternalEvent* ee =
                new vle::devs::ExternalEvent("release");

            ee << vle::devs::attribute("resources",
                                       mReleasedResources->toValue());
            output.addEvent(ee);
        }
    }

    vle::devs::Time timeAdvance() const
    {
        if (mPhase == SEND_ASSIGN or mPhase == SEND_DEMAND or
            mPhase == SEND_RELEASE) {
            return 0;
        } else {
            return vle::devs::Time::infinity;
        }
    }

    void internalTransition(const vle::devs::Time& /* time */)
    {
        if (mPhase == SEND_ASSIGN) {
            delete mDeliveredResources;
            mDeliveredResources = 0;
            delete mResourceConstraints;
            mResourceConstraints = 0;
            mPhase = WAIT;
        } else if (mPhase == SEND_DEMAND) {
            mPhase = WAIT;
        } else if (mPhase == SEND_RELEASE) {
            delete mReleasedResources;
            mReleasedResources = 0;
            mPhase = WAIT;
        }
    }

    void externalTransition(
        const vle::devs::ExternalEventList& events,
        const vle::devs::Time& time)
    {
        vle::devs::ExternalEventList::const_iterator it = events.begin();

        while (it != events.end()) {
            if ((*it)->onPort("available")) {
                Resources* r = Resources::build(
                    (*it)->getAttributeValue("resources"));

                if (mDeliveredResources) {
                    mDeliveredResources->add(*r);
                    r->clear();
                    delete r;
                } else {
                    mDeliveredResources = r;
                }

                std::cout << "[" << getModel().getParentName()
                          << ":" << getModelName()
                          << "] - assign = " << mDeliveredResources->size()
                          << "/" << mResourceConstraints->quantity()
                          << std::endl;

                if (mDeliveredResources->size() ==
                    mResourceConstraints->quantity()) {
                    mPhase = SEND_ASSIGN;
                }
            } else if ((*it)->onPort("demand")) {
                mResourceConstraints =
                    ResourceConstraints::build(
                        (*it)->getAttributeValue("resources"));

                std::cout << "[" << getModel().getParentName()
                          << ":" << getModelName()
                          << "] - demand = " << mResourceConstraints->quantity()
                          << std::endl;

                mPhase = SEND_DEMAND;
            } else if ((*it)->onPort("release")) {
                mReleasedResources = Resources::build(
                    (*it)->getAttributeValue("resources"));

                std::cout << "[" << getModel().getParentName()
                          << ":" << getModelName()
                          << "] - release = "
                          << *mReleasedResources << std::endl;

                mPhase = SEND_RELEASE;
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
    enum Phase { WAIT, SEND_DEMAND, SEND_ASSIGN, SEND_RELEASE };

    Phase mPhase;
    ResourceConstraints* mResourceConstraints;
    Resources* mDeliveredResources;
    Resources* mReleasedResources;
};

} // namespace rcpsp

DECLARE_NAMED_DYNAMICS(Assignment, rcpsp::Assignment);
