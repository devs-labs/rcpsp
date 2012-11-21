/**
 * @file test/test.cpp
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

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE package_test
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <Activity.hpp>

using namespace rcpsp;

BOOST_AUTO_TEST_CASE(test_activity)
{
    Activity activity("A1",
                      TemporalConstraints(TemporalConstraints::NONE,
                                          vle::devs::Time::negativeInfinity,
                                          vle::devs::Time::infinity,
                                          vle::devs::Time::negativeInfinity,
                                          vle::devs::Time::infinity));
}

BOOST_AUTO_TEST_CASE(test_step)
{
    Location L1("L1");
    Location L2("L2");
    Location L3("L3");

    ResourcePool Pool1("R1");
    Pool1.add(new Resource("R1", "R1_1"));
    Pool1.add(new Resource("R1", "R1_2"));
    Pool1.add(new Resource("R1", "R1_3"));

    ResourcePool Pool2("R2");
    Pool2.add(new Resource("R2", "R2_1"));
    Pool2.add(new Resource("R2", "R2_2"));
    Pool2.add(new Resource("R2", "R2_3"));

    ResourcePool Pool3("R3");
    Pool3.add(new Resource("R3", "R3_1"));
    Pool3.add(new Resource("R3", "R3_2"));
    Pool3.add(new Resource("R3", "R3_3"));

    ResourceConstraints resourceConstraints;

    resourceConstraints.push_back(ResourceConstraint(Pool1.name(), 1));
    resourceConstraints.push_back(ResourceConstraint(Pool2.name(), 1));

    Step step("a1_1", 10, L1, resourceConstraints,
              TemporalConstraints(TemporalConstraints::NONE,
                                  vle::devs::Time::negativeInfinity,
                                  vle::devs::Time::infinity,
                                  vle::devs::Time::negativeInfinity,
                                  vle::devs::Time::infinity));
}
