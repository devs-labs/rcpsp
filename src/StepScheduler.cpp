/**
 * @file StepScheduler.cpp
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

#include <devs/StepScheduler.hpp>
#include <policy/FIFOPolicy.hpp>

namespace rcpsp {

    class StepScheduler : public devs::StepScheduler
    {
    public:
        StepScheduler(const vle::devs::DynamicsInit& init,
                      const vle::devs::InitEventList& events) :
            devs::StepScheduler(init, events)
        {
            mPolicy = new FIFOPolicy(mWaitingActivities);
        }

        virtual void add(Activity* a)
        { mPolicy->add(a); }

        virtual bool another() const
        { return mPolicy->another(); }

        virtual bool demand() const
        { return mPolicy->demand(); }

        virtual bool empty() const
        { return mWaitingActivities.empty(); }

        virtual bool next()
        { return mPolicy->next(); }

        virtual void remove(Activity* a)
        {
            WaitingActivities::iterator it =
                std::find(mWaitingActivities.begin(),
                          mWaitingActivities.end(), a);

            mWaitingActivities.erase(it);
        }

        virtual void reset()
        { mPolicy->reset(); }

        virtual Activity* select() const
        { return mPolicy->select(); }

        virtual vle::value::Value* observe() const
        { return mWaitingActivities.toValue(); }

    private:
        WaitingActivities mWaitingActivities;
        StepSchedulingPolicy* mPolicy;
    };

} // namespace rcpsp

DECLARE_DYNAMICS(rcpsp::StepScheduler);
