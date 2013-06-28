/**
 * @file Activity.cpp
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

#include <data/Activity.hpp>

namespace rcpsp {

Activity::Activity(const vle::value::Value* value)
{
    const vle::value::Set* set = dynamic_cast < const vle::value::Set* >(value);

    mName = vle::value::toString(set->get(0));
    mSteps = new Steps(set->get(1));
    mTemporalConstraints = TemporalConstraints(set->get(2));
    if (set->size() > 3) {
        if (vle::value::toSet(set->get(3)).size() == 0) {
            mAllocatedResources = 0;
        } else {
            mAllocatedResources = new Resources(set->get(3));
        }
        if (set->size() > 4) {
            std::string value = vle::value::toString(set->get(4));

            if (value == "DONE") {
                mStepIterator = mSteps->end();
            } else {
                mStepIterator = mSteps->find(value);
            }
        }
    } else {
        mAllocatedResources = 0;
        mStepIterator = mSteps->begin();
    }
}

void Activity::assign(Resources* r)
{
    if (mAllocatedResources) {
        for (Resources::const_iterator it = r->begin(); it != r->end(); ++it) {
            mAllocatedResources->push_back(*it);
        }
        delete r;
    } else {
        mAllocatedResources = r;
    }
}

ResourceConstraints Activity::buildResourceConstraints() const
{
    if (mStepIterator != mSteps->end()) {
        return (*mStepIterator)->buildResourceConstraints(mAllocatedResources);
    } else {
        return ResourceConstraints();
    }
}

bool Activity::checkResourceConstraint() const
{
    if (mStepIterator != mSteps->end()) {
        return (*mStepIterator)->checkResourceConstraint(*mAllocatedResources);
    } else {
        return false;
    }
}

bool Activity::done(const vle::devs::Time& time) const
{
    if (not mSteps->empty() and mStepIterator != mSteps->end()) {
        return (*mStepIterator)->done(time);
    } else {
        return true;
    }
}

void Activity::finish(const vle::devs::Time& time)
{
    if (not mSteps->empty() and mStepIterator != mSteps->end()) {
        (*mStepIterator)->finish(time);
        ++mStepIterator;
    } else {
        //TODO exception
    }
}

void Activity::release()
{
    if (end()) {
        mAllocatedResources = 0;
    } else {
        Resources::iterator it = mAllocatedResources->begin();

        while (it != mAllocatedResources->end()) {
            if (not (*mStepIterator)->needAgain((*it)->type())) {
                mAllocatedResources->erase(it);
                it = mAllocatedResources->begin();
            } else {
                ++it;
            }
        }
    }
}

Resources* Activity::releasedResources() const
{
    if (end()) {
        return mAllocatedResources;
    } else {
        Resources* resources = new Resources;

        for (Resources::const_iterator it = mAllocatedResources->begin();
             it != mAllocatedResources->end(); ++it) {
            if (not (*mStepIterator)->needAgain((*it)->type())) {
                resources->push_back(*it);
            }
        }
        return resources;
    }
}

vle::devs::Time Activity::remainingTime(const vle::devs::Time& time) const
{
    if (not mSteps->empty() and mStepIterator != mSteps->end()) {
        return (*mStepIterator)->remainingTime(time);
    } else {
        return 0;
    }
}

const ResourceConstraints& Activity::resourceConstraints() const
{
    if (not mSteps->empty() and mStepIterator != mSteps->end()) {
        return (*mStepIterator)->resourceConstraints();
    } else {
        //TODO exception
    }
}

void Activity::start(const vle::devs::Time& time)
{
    if (not mSteps->empty() and mStepIterator != mSteps->end()) {
        (*mStepIterator)->start(time);
    } else {
        //TODO exception
    }
}

bool Activity::starting(const vle::devs::Time& time) const
{
    if (not mSteps->empty() and mStepIterator != mSteps->end()) {
        return (*mStepIterator)->starting(time);
    } else {
        return false;
    }
}

vle::value::Value* Activity::toValue() const
{
    vle::value::Set* value = new vle::value::Set();

    value->add(new vle::value::String(mName));
    value->add(mSteps->toValue());
    value->add(mTemporalConstraints.toValue());
    if (mAllocatedResources) {
        value->add(mAllocatedResources->toValue());
    } else {
        value->add(new vle::value::Set);
    }
    if (not mSteps->empty() and mStepIterator != mSteps->end()) {
        value->add(new vle::value::String((*mStepIterator)->name()));
    } else {
        value->add(new vle::value::String("DONE"));
    }
    return value;
}

void Activity::wait(const vle::devs::Time& time)
{
    if (not mSteps->empty() and mStepIterator != mSteps->end()) {
        (*mStepIterator)->wait(time);
    } else {
        //TODO exception
    }
}

std::ostream& operator<<(std::ostream& o, const Activity& a)
{
    o << "[ " << a.mName << " , " << *a.mSteps;
    if (not a.end()) {
        o << " < " << (*a.mStepIterator)->name() << " >";
    }
    o << " , " << a.mTemporalConstraints << " ]";
    if (a.mAllocatedResources) {
        o << ", " << *a.mAllocatedResources;
    }
    return o;
}

} // namespace rcpsp
