/**
 * @file TemporalConstraints.cpp
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

#include <data/TemporalConstraints.hpp>

namespace rcpsp {

std::ostream& operator<<(std::ostream& o, const TemporalConstraints& tc)
{
    o << "(";
    if (tc.mType == TemporalConstraints::NONE) o << "NONE ";
    if (tc.mType & TemporalConstraints::ES) o << "ES ";
    if (tc.mType & TemporalConstraints::LS) o << "LS ";
    if (tc.mType & TemporalConstraints::EF) o << "EF ";
    if (tc.mType & TemporalConstraints::LF) o << "LF ";
    o << "| ";
    o << tc.mEarlyStartTime << " | ";
    o << tc.mLateStartTime << " | ";
    o << tc.mEarlyFinishTime << " | ";
    o << tc.mLateFinishTime << ")";
    return o;
}

} // namespace rcpsp
