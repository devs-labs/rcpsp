INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  ${VLE_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS})

LINK_DIRECTORIES(
  ${VLE_LIBRARY_DIRS}
  ${Boost_LIBRARY_DIRS})

ADD_LIBRARY(ActivityPilot MODULE ActivityPilot.cpp)
TARGET_LINK_LIBRARIES(ActivityPilot ${VLE_LIBRARIES})
INSTALL(TARGETS ActivityPilot
  RUNTIME DESTINATION plugins/simulator
  LIBRARY DESTINATION plugins/simulator)

ADD_LIBRARY(ActivityScheduler MODULE ActivityScheduler.cpp)
TARGET_LINK_LIBRARIES(ActivityScheduler ${VLE_LIBRARIES} rcpsp-data)
INSTALL(TARGETS ActivityScheduler
  RUNTIME DESTINATION plugins/simulator
  LIBRARY DESTINATION plugins/simulator)

ADD_LIBRARY(Assignment MODULE Assignment.cpp)
TARGET_LINK_LIBRARIES(Assignment ${VLE_LIBRARIES} rcpsp-data)
INSTALL(TARGETS Assignment
  RUNTIME DESTINATION plugins/simulator
  LIBRARY DESTINATION plugins/simulator)

ADD_LIBRARY(Dispatcher MODULE Dispatcher.cpp)
TARGET_LINK_LIBRARIES(Dispatcher ${VLE_LIBRARIES} rcpsp-data)
INSTALL(TARGETS Dispatcher
  RUNTIME DESTINATION plugins/simulator
  LIBRARY DESTINATION plugins/simulator)

ADD_LIBRARY(Pool MODULE Pool.cpp)
TARGET_LINK_LIBRARIES(Pool ${VLE_LIBRARIES} rcpsp-data)
INSTALL(TARGETS Pool
  RUNTIME DESTINATION plugins/simulator
  LIBRARY DESTINATION plugins/simulator)

ADD_LIBRARY(Processor MODULE Processor.cpp)
TARGET_LINK_LIBRARIES(Processor ${VLE_LIBRARIES} rcpsp-data)
INSTALL(TARGETS Processor
  RUNTIME DESTINATION plugins/simulator
  LIBRARY DESTINATION plugins/simulator)

ADD_LIBRARY(StepScheduler MODULE StepScheduler.cpp)
TARGET_LINK_LIBRARIES(StepScheduler ${VLE_LIBRARIES} StepScheduler-devs
  rcpsp-data)
INSTALL(TARGETS StepScheduler
  RUNTIME DESTINATION plugins/simulator
  LIBRARY DESTINATION plugins/simulator)

ADD_SUBDIRECTORY(constructor)
ADD_SUBDIRECTORY(data)
ADD_SUBDIRECTORY(devs)
ADD_SUBDIRECTORY(utils)