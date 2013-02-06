INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  ${VLE_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS})

LINK_DIRECTORIES(
  ${VLE_LIBRARY_DIRS}
  ${Boost_LIBRARY_DIRS})

ADD_LIBRARY(rcpsp-data STATIC Activities.cpp Planning.hpp Resource.cpp
  Steps.cpp Activities.hpp PrecedenceConstraint.hpp Resource.hpp Steps.hpp
  Activity.cpp PrecedencesGraph.hpp ResourcePool.hpp TemporalConstraints.cpp
  Activity.hpp Problem.hpp Resources.cpp TemporalConstraints.hpp
  ResourceConstraint.hpp Resources.hpp ResourceConstraints.cpp Step.cpp
  Location.hpp ResourceConstraints.hpp Step.hpp)

TARGET_LINK_LIBRARIES(rcpsp-data ${VLE_LIBRARIES} ${Boost_LIBRARIES})
