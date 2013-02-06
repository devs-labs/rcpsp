/**
 * @file PrecedenceConstraint.hpp
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

#ifndef __PRECEDENCE_CONTRAINTS_HPP
#define __PRECEDENCE_CONTRAINTS_HPP 1

#include <string>

#include <vle/devs/Time.hpp>

#include <data/Activities.hpp>

namespace rcpsp {

class PrecedenceConstraint
{
public:
    enum Type {
        SS, /**< Start to Start relationship. */
        FS, /**< Finish to Start relationship. */
        SF, /**< Start to Finish relationship. */
        FF /**< Finish to Finish relationship. */
    };

    PrecedenceConstraint(Activities::const_iterator& first,
                         Activities::const_iterator& second,
                         Type type,
                         const vle::devs::Time& mintimelag,
                         const vle::devs::Time& maxtimelag) :
        mFirst(first), mSecond(second), mType(type),
        mMinTimelag(mintimelag), mMaxTimelag(maxtimelag)
    { }

    bool isSS() const { return mType == SS; }
    bool isFS() const { return mType == FS; }
    bool isSF() const { return mType == SF; }
    bool isFF() const { return mType == FF; }

    const Type& type() const { return mType; }

    const vle::devs::Time& minTimelag() const { return mMinTimelag; }
    const vle::devs::Time& maxTimelag() const { return mMaxTimelag; }

private:
    Activities::const_iterator mFirst;
    Activities::const_iterator mSecond;
    Type mType;
    vle::devs::Time mMinTimelag;
    vle::devs::Time mMaxTimelag;
};

typedef std::vector < PrecedenceConstraint > PrecedenceConstraints;

} // namespace rcpsp

#endif
