/**
 * @file PrecedencesGraph.hpp
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

#ifndef __PRECEDENCE_GRAPH_HPP
#define __PRECEDENCE_GRAPH_HPP 1

#include <data/PrecedenceConstraint.hpp>

namespace rcpsp {

class PrecedencesGraph
{
public:
    PrecedencesGraph()
    { }

    void add(Activities::const_iterator& first,
             Activities::const_iterator& second,
             PrecedenceConstraint::Type type,
             const vle::devs::Time& mintimelag,
             const vle::devs::Time& maxtimelag)
    {
        mPrecedenceContraints.push_back(
            PrecedenceConstraint(first, second, type, mintimelag, maxtimelag));
    }

private:
    PrecedenceConstraints mPrecedenceContraints;
};

} // namespace rcpsp

#endif
