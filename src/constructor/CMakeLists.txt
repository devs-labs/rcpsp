INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  ${VLE_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS})

LINK_DIRECTORIES(
  ${VLE_LIBRARY_DIRS}
  ${Boost_LIBRARY_DIRS})

ADD_LIBRARY(Constructor MODULE Constructor.cpp)
TARGET_LINK_LIBRARIES(Constructor ${VLE_LIBRARIES} rcpsp-data)
INSTALL(TARGETS Constructor
  RUNTIME DESTINATION plugins/simulator
  LIBRARY DESTINATION plugins/simulator)

ADD_LIBRARY(PoolConstructor MODULE PoolConstructor.cpp)
TARGET_LINK_LIBRARIES(PoolConstructor ${VLE_LIBRARIES} rcpsp-data)
INSTALL(TARGETS PoolConstructor
  RUNTIME DESTINATION plugins/simulator
  LIBRARY DESTINATION plugins/simulator)