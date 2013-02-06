/**
 * @file ResourceConstraint.hpp
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

#ifndef __RESOURCE_CONTRAINT_HPP
#define __RESOURCE_CONTRAINT_HPP 1

#include <string>
#include <vle/devs/Time.hpp>
#include <vle/value/Value.hpp>

#include <data/Resource.hpp>

namespace rcpsp {

class ResourceConstraint
{
public:
    ResourceConstraint(const std::string& type, int quantity, bool same) :
        mType(type), mQuantity(quantity), mSame(same)
    { }

    ResourceConstraint(const vle::value::Value* value)
    {
        const vle::value::Set* set =
            dynamic_cast < const vle::value::Set* >(value);

        mType = vle::value::toString(set->get(0));
        mQuantity = vle::value::toInteger(set->get(1));
        if (set->size() == 3) {
            mSame = vle::value::toBoolean(set->get(2));
        } else {
            mSame = false;
        }
    }

    ResourceConstraint(const ResourceConstraint& rc) :
        mType(rc.mType),
        mQuantity(rc.mQuantity),
        mSame(rc.mSame)
    { }

    unsigned int quantity() const
    { return mQuantity; }

    virtual const ResourceConstraint& operator=(const ResourceConstraint& rc)
    {
        mType = rc.mType;
        mQuantity = rc.mQuantity;
        return *this;
    }

    vle::value::Value* toValue() const
    {
        vle::value::Set* value = new vle::value::Set;

        value->add(new vle::value::String(mType));
        value->add(new vle::value::Integer(mQuantity));
        value->add(new vle::value::Boolean(mSame));
        return value;
    }

    bool same() const
    { return mSame; }

    const std::string& type() const
    { return mType; }

private:
    std::string mType;
    unsigned int mQuantity;
    bool mSame;
};

} // namespace rcpsp

#endif
