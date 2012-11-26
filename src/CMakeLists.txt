INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  ${VLE_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS})

LINK_DIRECTORIES(
  ${VLE_LIBRARY_DIRS}
  ${Boost_LIBRARY_DIRS})

ADD_LIBRARY(rcpsp SHARED Activities.hpp Activities.cpp Activity.hpp
  Activity.cpp ActivityScheduler.cpp Assignment.cpp Location.hpp
  Planning.hpp Pool.cpp PrecedenceConstraint.hpp PrecedencesGraph.hpp
  Processor.cpp Resource.hpp Resource.cpp Resources.hpp Resources.cpp
  ResourceConstraint.hpp ResourceConstraints.hpp ResourcePool.hpp
  TemporalConstraints.hpp TemporalConstraints.cpp Step.hpp Step.cpp
  StepScheduler.cpp Steps.hpp Steps.cpp)

TARGET_LINK_LIBRARIES(rcpsp
  ${VLE_LIBRARIES}
  ${Boost_LIBRARIES})

INSTALL(TARGETS rcpsp
  RUNTIME DESTINATION lib
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
