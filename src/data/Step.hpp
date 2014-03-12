/**
 * @file Step.hpp
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

#ifndef __STEP_HPP
#define __STEP_HPP 1

#include <ostream>
#include <string>

#include <vle/value/Value.hpp>

#include <data/Location.hpp>
#include <data/ResourceConstraints.hpp>
#include <data/TemporalConstraints.hpp>

namespace rcpsp {

class Step
{
public:
    Step(const std::string& name,
         const vle::devs::Time& duration,
         const Location& location,
         const ResourceConstraints& resourceConstraints,
         const TemporalConstraints& temporalConstraints) :
        mName(name),
        mDuration(duration),
        mLocation(location),
        mResourceConstraints(resourceConstraints),
        mTemporalConstraints(temporalConstraints),
        mWaiting(false),
        mRunning(false),
        mDone(false),
        mStartDate(vle::devs::infinity),
        mFinishDate(vle::devs::infinity)
    { }

    Step(const vle::value::Value* value)
    {
        const vle::value::Set* set =
            dynamic_cast < const vle::value::Set* >(value);

        mName = vle::value::toString(set->get(0));
        mDuration = (vle::devs::Time)vle::value::toDouble(set->get(1));
        mLocation = Location(set->get(2));
        mResourceConstraints = ResourceConstraints(set->get(3));
        mTemporalConstraints = TemporalConstraints(set->get(4));

        mWaiting = false;
        mRunning = false;
        mDone = false;
        mStartDate = vle::devs::infinity;
        mFinishDate = vle::devs::infinity;
    }

    virtual ~Step()
    { }

    ResourceConstraints buildResourceConstraints(const Resources* r) const
    { return mResourceConstraints.buildResourceConstraints(r); }

    bool checkResourceConstraint(const Resources& r) const
    { return mResourceConstraints.checkResourceConstraint(r); }

    bool done(const vle::devs::Time& time) const
    { return mDuration == time - mStartDate; }

    const vle::devs::Time& duration() const
    { return mDuration; }

    void finish(const vle::devs::Time& time)
    {
        mFinishDate = time;
        mRunning = false;
        mDone = true;
    }

    const Location& location() const
    { return mLocation; }

    const std::string& name() const
    { return mName; }

    bool needAgain(const std::string& type) const
    { return mResourceConstraints.needAgain(type); }

    virtual bool operator==(const Step& s) const
    { return mName == s.mName; }

    virtual bool operator==(const std::string& name) const
    { return mName == name; }

    vle::devs::Time remainingTime(const vle::devs::Time& time) const
    { return mDuration - (time - mStartDate); }

    const ResourceConstraints& resourceConstraints() const
    { return mResourceConstraints; }

    void start(const vle::devs::Time& time)
    {
        mStartDate = time;
        mWaiting = false;
        mRunning = true;
    }

    bool starting(const vle::devs::Time& time) const
    { return mTemporalConstraints.starting(time); }

    const TemporalConstraints& temporalConstraints() const
    { return mTemporalConstraints; }

    vle::value::Value* toValue() const
    {
        vle::value::Set* value = new vle::value::Set;

        value->add(new vle::value::String(mName));
        value->add(new vle::value::Double(mDuration));
        value->add(mLocation.toValue());
        value->add(mResourceConstraints.toValue());
        value->add(mTemporalConstraints.toValue());
        return value;
    }

    void wait(const vle::devs::Time& time)
    {
        mWaitDate = time;
        mWaiting = true;
    }

private:
    friend std::ostream& operator<<(std::ostream& o, const Step& s);

    std::string mName;
    vle::devs::Time mDuration;
    Location mLocation;
    ResourceConstraints mResourceConstraints;
    TemporalConstraints mTemporalConstraints;

    // state
    bool mWaiting;
    bool mRunning;
    bool mDone;

    vle::devs::Time mWaitDate;
    vle::devs::Time mStartDate;
    vle::devs::Time mFinishDate;
};

std::ostream& operator<<(std::ostream& o, const Step& s);

} // namespace rcpsp

#endif
