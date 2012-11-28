/**
 * @file ResourceConstraints.cpp
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

#include <ResourceConstraints.hpp>

namespace rcpsp {

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

} // namespace rcpsp
