/**
 * @file devs/Processor.hpp
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

#include <data/Activities.hpp>

namespace rcpsp { namespace devs {

class Processor : public vle::devs::Dynamics
{
public:
    Processor(const vle::devs::DynamicsInit& init,
              const vle::devs::InitEventList& events);

    virtual vle::devs::Time computeRemainingTime(
        const vle::devs::Time& time) const =0;
    virtual void done(const vle::devs::Time& time) =0;
    virtual void finish(const vle::devs::Time& time) =0;
    virtual bool idle() const =0;
    virtual void start(const vle::devs::Time& time, Activity* a) =0;

    virtual vle::devs::Time init(const vle::devs::Time& /* time */);
    virtual void output(const vle::devs::Time& /* time */,
                        vle::devs::ExternalEventList& output) const;
    virtual vle::devs::Time timeAdvance() const;
    virtual void internalTransition(const vle::devs::Time& time);
    virtual void externalTransition(
        const vle::devs::ExternalEventList& events,
        const vle::devs::Time& time);
    virtual void confluentTransitions(
        const vle::devs::Time& time,
        const vle::devs::ExternalEventList& events);
    virtual vle::value::Value* observation(
        const vle::devs::ObservationEvent& event) const;

protected:
    Activities mRunningActivities;
    Activities mDoneActivities;

private:
    enum Phase { IDLE, RUNNING, DONE };

    Phase mPhase;
    vle::devs::Time mSigma;
};

} } // namespace devs rcpsp
