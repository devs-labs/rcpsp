/**
 * @file ResourceConstraints.cpp
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

#include <data/ResourceConstraints.hpp>

#include <map>

namespace rcpsp {

ResourceConstraints ResourceConstraints::buildResourceConstraints(
    const Resources* r) const
{
    ResourceConstraints rc;
    std::map < std::string, int > resourceQuantity;

    for (const_iterator it = begin(); it != end(); ++it) {
        resourceQuantity[it->type()] = it->quantity();
    }

    if (r) {
        for (Resources::const_iterator it = r->begin(); it != r->end(); ++it) {
            --resourceQuantity[(*it)->type()];
        }
    }

    for (std::map < std::string, int >::const_iterator it =
             resourceQuantity.begin(); it != resourceQuantity.end(); ++it) {
        if (it->second > 0) {
            rc.push_back(ResourceConstraint(it->first, it->second, false));
        }
    }
    return rc;
}

bool ResourceConstraints::checkResourceConstraint(const Resources& r) const
{
    bool ok = true;
    std::map < std::string, int > resourceQuantity;

    for (const_iterator it = begin(); it != end(); ++it) {
        resourceQuantity[it->type()] = it->quantity();
    }
    for (Resources::const_iterator it = r.begin(); it != r.end(); ++it) {
        --resourceQuantity[(*it)->type()];
    }
    for (std::map < std::string, int >::const_iterator it =
             resourceQuantity.begin(); it != resourceQuantity.end() and ok;
         ++it) {
        ok = it->second == 0;
    }
    return ok;
}

bool ResourceConstraints::needAgain(const std::string& type) const
{
    bool yes = false;

    for (const_iterator it = begin(); it != end() and not yes; ++it) {
        yes = it->type() == type and it->same();
    }
    return yes;
}

} // namespace rcpsp
