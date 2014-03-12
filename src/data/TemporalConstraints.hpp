/**
 * @file TemporalConstraints.hpp
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

#ifndef __TEMPORAL_CONTRAINTS_HPP
#define __TEMPORAL_CONTRAINTS_HPP 1

#include <string>

#include <vle/devs/Time.hpp>
#include <vle/value/Double.hpp>
#include <vle/value/Set.hpp>
#include <vle/value/Value.hpp>

namespace rcpsp {

class TemporalConstraints
{
public:
    enum Type {
        NONE = 0,
        ES = 1,        /**< The early start time (ES). */
        LS = 1 << 1,   /**< The late start time (LS) */
        EF = 1 << 2,   /**< The early finish time (EF). */
        LF = 1 << 3    /**< The late finish time (LF) */
    };

    TemporalConstraints()
    { }

    TemporalConstraints(Type type,
                        const vle::devs::Time& earlystarttime,
                        const vle::devs::Time& latestarttime,
                        const vle::devs::Time& earlyfinishtime,
                        const vle::devs::Time& latefinishtime) :
        mType(type), mEarlyStartTime(earlystarttime),
        mLateStartTime(latestarttime),
        mEarlyFinishTime(earlyfinishtime),
        mLateFinishTime(latefinishtime)
    { }

    TemporalConstraints(const TemporalConstraints& tc) :
        mType(tc.mType),
        mEarlyStartTime(tc.mEarlyStartTime),
        mLateStartTime(tc.mLateStartTime),
        mEarlyFinishTime(tc.mEarlyFinishTime),
        mLateFinishTime(tc.mLateFinishTime)
    { }

    TemporalConstraints(const vle::value::Value* value)
    {
        const vle::value::Set* set =
            dynamic_cast < const vle::value::Set* >(value);

        mType = (Type)vle::value::toInteger(set->get(0));
        mEarlyStartTime = (vle::devs::Time)vle::value::toDouble(set->get(1));
        mLateStartTime = (vle::devs::Time)vle::value::toDouble(set->get(2));
        mEarlyFinishTime = (vle::devs::Time)vle::value::toDouble(set->get(3));
        mLateFinishTime = (vle::devs::Time)vle::value::toDouble(set->get(4));
    }

    bool starting(const vle::devs::Time& time) const
    {
        if (mType & (ES | LS)) {
            return mEarlyStartTime <= time and time <= mLateStartTime;
        }
        return true;
    }

    bool isES() const { return mType & ES; }
    bool isLS() const { return mType & LS; }
    bool isEF() const { return mType & EF; }
    bool isLF() const { return mType & LF; }

    const Type& type() const { return mType; }

    const vle::devs::Time& earlyStartTime() const { return mEarlyStartTime; }
    const vle::devs::Time& lateStartTime() const { return mLateStartTime; }
    const vle::devs::Time& earlyFinishTime() const { return mEarlyFinishTime; }
    const vle::devs::Time& lateFinishTime() const { return mLateFinishTime; }

    vle::value::Value* toValue() const
    {
        vle::value::Set* value = new vle::value::Set;

        value->add(new vle::value::Integer(mType));
        value->add(new vle::value::Double(mEarlyStartTime));
        value->add(new vle::value::Double(mLateStartTime));
        value->add(new vle::value::Double(mEarlyFinishTime));
        value->add(new vle::value::Double(mLateFinishTime));
        return value;
    }

private:
    friend std::ostream& operator<<(std::ostream& o,
                                    const TemporalConstraints& tc);

    Type mType;
    vle::devs::Time mEarlyStartTime;
    vle::devs::Time mLateStartTime;
    vle::devs::Time mEarlyFinishTime;
    vle::devs::Time mLateFinishTime;
};

std::ostream& operator<<(std::ostream& o, const TemporalConstraints& tc);

} // namespace rcpsp

#endif
