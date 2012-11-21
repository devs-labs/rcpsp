/**
 * @file Resource.hpp
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

#ifndef __RESOURCE_HPP
#define __RESOURCE_HPP 1

#include <string>
#include <vector>

#include <vle/value.hpp>

#include <Planning.hpp>

namespace rcpsp {

class Resource
{
public:
    Resource(const std::string& name, const std::string& type) :
        mName(name), mType(type)
    { }

    Resource(const vle::value::Value* value)
    {
        const vle::value::Set* set =
            dynamic_cast < const vle::value::Set* >(value);

        mName = vle::value::toString(set->get(0));
        mType = vle::value::toString(set->get(1));
    }

    const std::string& name() const
    { return mName; }

    vle::value::Value* toValue() const
    {
        vle::value::Set* value = new vle::value::Set;

        value->add(new vle::value::String(mName));
        value->add(new vle::value::String(mType));
        return value;
    }

    const std::string& type() const
    { return mType; }

private:
    friend std::ostream& operator<<(std::ostream& o, const Resource& r);

    std::string mName;
    std::string mType;
};

std::ostream& operator<<(std::ostream& o, const Resource& r);

class Resources : public std::vector < Resource* >
{
public:
    Resources()
    { }

    Resources(const vle::value::Value* value)
    {
        const vle::value::Set* set =
            dynamic_cast < const vle::value::Set* >(value);

        for (unsigned int i = 0; i < set->size(); ++i) {
            push_back(new Resource(set->get(i)));
        }
    }

    virtual ~Resources()
    { for(iterator it = begin(); it != end(); ++it) delete *it; }

    void add(const Resources& r)
    {
        for (Resources::const_iterator itr = r.begin(); itr != r.end();
             ++itr) {
            push_back(*itr);
        }
    }

    static Resources* build(const vle::value::Value& value)
    { return new Resources(&value); }

    vle::value::Value* toValue() const
    {
        vle::value::Set* value = new vle::value::Set;

        for (const_iterator it = begin(); it != end(); ++it) {
            value->add((*it)->toValue());
        }
        return value;
    }
};

std::ostream& operator<<(std::ostream& o, const Resources& r);

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
