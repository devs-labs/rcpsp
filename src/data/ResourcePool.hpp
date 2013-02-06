/**
 * @file ResourcePool.hpp
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

#ifndef __RESOURCE_POOL_HPP
#define __RESOURCE_POOL_HPP 1

#include <string>
#include <vector>

#include <vle/value/Set.hpp>
#include <vle/value/Value.hpp>

#include <data/Resources.hpp>

namespace rcpsp {

class ResourcePool
{
public:
    ResourcePool(const std::string& name) :
        mName(name)
    { }

    ResourcePool(const vle::value::Value* value)
    {
        const vle::value::Set* set =
            dynamic_cast < const vle::value::Set* >(value);

        mName = vle::value::toString(set->get(0));
        mType = vle::value::toString(set->get(1));
        mResources = new Resources(set->get(2));
        //TODO: mPlannings
    }

    virtual ~ResourcePool()
    { delete mResources; }

    void add(Resource* resource)
    { mResources->push_back(resource); }

    Resources* assign(int n)
    {
        Resources* r = new Resources;

        for(int i = 0; i < n; ++i) {
            r->push_back(mResources->back());
            mResources->pop_back();
        }
        return r;
    }

    //TODO: to include planning constraint
    Resources* available() const
    { return mResources; }

    const std::string& name() const
    { return mName; }

    int quantity() const
    { return mResources->size(); }

    void release(Resources* r)
    {
        for(Resources::const_iterator it = r->begin(); it != r->end(); ++it) {
            if (mType == (*it)->type()) {
                mResources->push_back(*it);
            }
        }
    }

    vle::value::Value* toValue() const
    {
        vle::value::Set* value = new vle::value::Set;

        value->add(new vle::value::String(mName));
        value->add(new vle::value::String(mType));
        value->add(mResources->toValue());
        //TODO: mPlannings
        return value;
    }

    const std::string& type() const
    { return mType; }

private:
    std::string mName;
    std::string mType;
    Resources* mResources;
    Plannings mPlannings;
};

} // namespace rcpsp

#endif
