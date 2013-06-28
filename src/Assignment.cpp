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

#include <vle/utils/Trace.hpp>

#include <data/Resources.hpp>
#include <data/ResourceConstraints.hpp>

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
        mAvailableResourceNumber = 0;
        mReleasedResources = 0;
        mResponseNumber = 0;
        mPhase = WAIT_DEMAND;
        return vle::devs::infinity;
    }

    void output(const vle::devs::Time& time,
                vle::devs::ExternalEventList& output) const
    {
        if (mPhase == SEND_ASSIGN) {
            for (ResourceConstraints::const_iterator it =
                     mResourceConstraints->begin();
                 it != mResourceConstraints->end(); ++it) {
                vle::devs::ExternalEvent* ee =
                    new vle::devs::ExternalEvent("assign");

                ee << vle::devs::attribute("type", it->type());
                ee << vle::devs::attribute("quantity", (int)it->quantity());
                output.push_back(ee);
            }
        } else if (mPhase == SEND_DEMAND) {
            for (ResourceConstraints::const_iterator it =
                     mResourceConstraints->begin();
                 it != mResourceConstraints->end(); ++it) {
                vle::devs::ExternalEvent* ee =
                    new vle::devs::ExternalEvent("demand");

                TraceModel(vle::fmt(" [%1%:%2%] at %3% -> demand: %4% = %5%") %
                           getModel().getParentName() % getModelName() %
                           time % it->type() % it->quantity());

                ee << vle::devs::attribute("type", it->type());
                ee << vle::devs::attribute("quantity", (int)it->quantity());
                output.push_back(ee);
            }
        } else if (mPhase == SEND_RELEASE) {
            vle::devs::ExternalEvent* ee =
                new vle::devs::ExternalEvent("release");

            ee << vle::devs::attribute("resources",
                                       mReleasedResources->toValue());
            output.push_back(ee);
        } else if (mPhase == SEND_UNAVAILABLE) {
            vle::devs::ExternalEvent* ee =
                new vle::devs::ExternalEvent("unavailable");

            ee << vle::devs::attribute("resources",
                                       mUnavailableResources.toValue());
            output.push_back(ee);
        }
    }

    vle::devs::Time timeAdvance() const
    {
        if (mPhase == SEND_ASSIGN or mPhase == SEND_DEMAND or
            mPhase == SEND_RELEASE or mPhase == SEND_UNAVAILABLE) {
            return 0;
        } else {
            return vle::devs::infinity;
        }
    }

    void internalTransition(const vle::devs::Time& /* time */)
    {
        if (mPhase == SEND_ASSIGN) {
            clearDemand();
            mPhase = WAIT_DEMAND;
        } else if (mPhase == SEND_DEMAND) {
            mPhase = WAIT_AVAILABLE;
        } else if (mPhase == SEND_RELEASE) {
            delete mReleasedResources;
            mReleasedResources = 0;
            if (mResourceConstraints != 0 and
                mResponseNumber < mResourceConstraints->size()) {
                mPhase = WAIT_AVAILABLE;
            } else {
                mPhase = WAIT_DEMAND;
            }
        } else if (mPhase == SEND_UNAVAILABLE) {
            mUnavailableResources.clear();
            mPhase = WAIT_DEMAND;
        }
    }

    void externalTransition(
        const vle::devs::ExternalEventList& events,
        const vle::devs::Time& time)
    {
        vle::devs::ExternalEventList::const_iterator it = events.begin();

        while (it != events.end()) {
            if ((*it)->onPort("available")) {
                bool available = (*it)->getBooleanAttributeValue("available");
                unsigned int number = (*it)->getIntegerAttributeValue("number");
                std::string type = (*it)->getStringAttributeValue("type");

                if (available) {
                    mAvailableResourceNumber += number;
                } else {
                    mUnavailableResources[type] =
                        mResourceConstraints->quantity(type) - number;
                }
                ++mResponseNumber;

                TraceModel(vle::fmt(" [%1%:%2%] at %3% -> available: %4% / %5%")
                           % getModel().getParentName() % getModelName() %
                           time % mAvailableResourceNumber %
                           mResourceConstraints->quantity());

                if (mAvailableResourceNumber ==
                    mResourceConstraints->quantity()) {
                    mPhase = SEND_ASSIGN;
                } else {
                    if (mResponseNumber == mResourceConstraints->size()) {
                        clearDemand();
                        mPhase = SEND_UNAVAILABLE;
                    } else {
                        mPhase = WAIT_AVAILABLE;
                    }
                }
            } else if ((*it)->onPort("demand")) {
                mResourceConstraints =
                    ResourceConstraints::build(Resources::get(*it));

                TraceModel(vle::fmt(" [%1%:%2%] at %3% -> demand: %4%") %
                           getModel().getParentName() % getModelName() %
                           time % mResourceConstraints->quantity());

                mPhase = SEND_DEMAND;
            } else if ((*it)->onPort("release")) {
                mReleasedResources = Resources::build(Resources::get(*it));

                TraceModel(vle::fmt(" [%1%:%2%] at %3% -> release: %4%") %
                           getModel().getParentName() % getModelName() %
                           time % *mReleasedResources);

                mPhase = SEND_RELEASE;
            }
            ++it;
        }
    }

    void confluentTransitions(const vle::devs::Time& time,
                              const vle::devs::ExternalEventList& /* events */)
    {
        TraceModel(vle::fmt(" [%1%:%2%] at %3% -> confluent !") %
                   getModel().getParentName() % getModelName() %
                   time);
    }

    virtual vle::value::Value* observation(
        const vle::devs::ObservationEvent& /* event */) const
    {
        return 0;
    }

private:
    void clearDemand()
    {
        mAvailableResourceNumber = 0;
        mResponseNumber = 0;
        delete mResourceConstraints;
        mResourceConstraints = 0;
    }

    enum Phase { WAIT_AVAILABLE, WAIT_DEMAND, SEND_DEMAND, SEND_ASSIGN,
                 SEND_RELEASE, SEND_UNAVAILABLE };

    Phase mPhase;
    ResourceConstraints* mResourceConstraints;
    unsigned int mResponseNumber;
    unsigned int mAvailableResourceNumber;
    Resources* mReleasedResources;
    ResourceTypes mUnavailableResources;
};

} // namespace rcpsp

DECLARE_DYNAMICS(rcpsp::Assignment);
