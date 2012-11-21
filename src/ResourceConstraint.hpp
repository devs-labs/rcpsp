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

#ifndef __RESOURCE_CONTRAINTS_HPP
#define __RESOURCE_CONTRAINTS_HPP 1

#include <string>
#include <vle/devs/Time.hpp>
#include <vle/value.hpp>

#include <Resource.hpp>

namespace rcpsp {

class ResourceConstraint
{
public:
    ResourceConstraint(const std::string& type, int quantity) :
        mType(type), mQuantity(quantity)
    { }

    ResourceConstraint(const vle::value::Value* value)
    {
        const vle::value::Set* set =
            dynamic_cast < const vle::value::Set* >(value);

        mType = vle::value::toString(set->get(0));
        mQuantity = vle::value::toInteger(set->get(1));
    }

    ResourceConstraint(const ResourceConstraint& rc) :
        mType(rc.mType),
        mQuantity(rc.mQuantity)
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
        return value;
    }

    const std::string& type() const
    { return mType; }

private:
    std::string mType;
    unsigned int mQuantity;
};

class ResourceConstraints : public std::vector < ResourceConstraint >
{
public:
    ResourceConstraints()
    { }

    ResourceConstraints(const vle::value::Value* value)
    {
        const vle::value::Set* set =
            dynamic_cast < const vle::value::Set* >(value);

        for (unsigned int i = 0; i < set->size(); ++i) {
            push_back(ResourceConstraint(set->get(i)));
        }
    }

    static ResourceConstraints* build(const vle::value::Value& value)
    { return new ResourceConstraints(&value); }

    unsigned int quantity() const
    {
        unsigned int n = 0;

        for (const_iterator it = begin(); it != end(); ++it) {
            n += it->quantity();
        }
        return n;
    }

    vle::value::Value* toValue() const
    {
        vle::value::Set* value = new vle::value::Set;

        for (const_iterator it = begin(); it != end(); ++it) {
            value->add(it->toValue());
        }
        return value;
    }

};

} // namespace rcpsp

#endif
