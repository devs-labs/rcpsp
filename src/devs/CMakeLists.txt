INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  ${VLE_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS})

LINK_DIRECTORIES(
  ${VLE_LIBRARY_DIRS}
  ${Boost_LIBRARY_DIRS})

ADD_LIBRARY(StepScheduler-devs STATIC StepScheduler.hpp StepScheduler.cpp)
TARGET_LINK_LIBRARIES(StepScheduler-devs ${VLE_LIBRARIES} ${Boost_LIBRARIES}
  rcpsp-data)

ADD_LIBRARY(Processor-devs STATIC Processor.hpp Processor.cpp)
TARGET_LINK_LIBRARIES(Processor-devs ${VLE_LIBRARIES} ${Boost_LIBRARIES}
  rcpsp-data)