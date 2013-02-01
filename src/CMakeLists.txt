INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  ${VLE_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS})

LINK_DIRECTORIES(
  ${VLE_LIBRARY_DIRS}
  ${Boost_LIBRARY_DIRS})

DeclareDevsDynamics(ActivityPilot "ActivityPilot.cpp")
DeclareDevsDynamics(ActivityScheduler "ActivityScheduler.cpp;Activities.hpp;Activities.cpp;Activity.hpp;Activity.cpp;Location.hpp;Planning.hpp;PrecedencesGraph.hpp;Resource.hpp;Resource.cpp;ResourceConstraint.hpp;ResourceConstraints.hpp;ResourceConstraints.cpp;Resources.hpp;Resources.cpp;Step.hpp;Step.cpp;Steps.hpp;Steps.cpp;TemporalConstraints.hpp;TemporalConstraints.cpp")
DeclareDevsDynamics(Assignment "Assignment.cpp;Activities.hpp;Activities.cpp;Activity.hpp;Activity.cpp;Location.hpp;Planning.hpp;PrecedencesGraph.hpp;Resource.hpp;Resource.cpp;ResourceConstraint.hpp;ResourceConstraints.hpp;ResourceConstraints.cpp;Resources.hpp;Resources.cpp;Step.hpp;Step.cpp;Steps.hpp;Steps.cpp;TemporalConstraints.hpp;TemporalConstraints.cpp")
DeclareDevsDynamics(Beep "Beep.cpp")
DeclareDevsDynamics(Constructor "Constructor.cpp;Activities.hpp;Activities.cpp;Activity.hpp;Activity.cpp;Location.hpp;Planning.hpp;PrecedencesGraph.hpp;Resource.hpp;Resource.cpp;ResourceConstraint.hpp;ResourceConstraints.hpp;ResourceConstraints.cpp;Resources.hpp;Resources.cpp;Step.hpp;Step.cpp;Steps.hpp;Steps.cpp;TemporalConstraints.hpp;TemporalConstraints.cpp")
DeclareDevsDynamics(Dispatcher "Dispatcher.cpp;Activities.hpp;Activities.cpp;Activity.hpp;Activity.cpp;Location.hpp;Planning.hpp;PrecedencesGraph.hpp;Resource.hpp;Resource.cpp;ResourceConstraint.hpp;ResourceConstraints.hpp;ResourceConstraints.cpp;Resources.hpp;Resources.cpp;Step.hpp;Step.cpp;Steps.hpp;Steps.cpp;TemporalConstraints.hpp;TemporalConstraints.cpp")
DeclareDevsDynamics(PoolConstructor "PoolConstructor.cpp;Activities.hpp;Activities.cpp;Activity.hpp;Activity.cpp;Location.hpp;Planning.hpp;PrecedencesGraph.hpp;Resource.hpp;Resource.cpp;ResourceConstraint.hpp;ResourceConstraints.hpp;ResourceConstraints.cpp;Resources.hpp;Resources.cpp;Step.hpp;Step.cpp;Steps.hpp;Steps.cpp;TemporalConstraints.hpp;TemporalConstraints.cpp")
DeclareDevsDynamics(Pool "Pool.cpp;Activities.hpp;Activities.cpp;Activity.hpp;Activity.cpp;Location.hpp;Planning.hpp;PrecedencesGraph.hpp;Resource.hpp;Resource.cpp;ResourceConstraint.hpp;ResourceConstraints.hpp;ResourceConstraints.cpp;Resources.hpp;Resources.cpp;Step.hpp;Step.cpp;Steps.hpp;Steps.cpp;TemporalConstraints.hpp;TemporalConstraints.cpp")
DeclareDevsDynamics(Processor "Processor.cpp;Activities.hpp;Activities.cpp;Activity.hpp;Activity.cpp;Location.hpp;Planning.hpp;PrecedencesGraph.hpp;Resource.hpp;Resource.cpp;ResourceConstraint.hpp;ResourceConstraints.hpp;ResourceConstraints.cpp;Resources.hpp;Resources.cpp;Step.hpp;Step.cpp;Steps.hpp;Steps.cpp;TemporalConstraints.hpp;TemporalConstraints.cpp")
DeclareDevsDynamics(StepScheduler "StepScheduler.cpp;Activities.hpp;Activities.cpp;Activity.hpp;Activity.cpp;Location.hpp;Planning.hpp;PrecedencesGraph.hpp;Resource.hpp;Resource.cpp;ResourceConstraint.hpp;ResourceConstraints.hpp;ResourceConstraints.cpp;Resources.hpp;Resources.cpp;Step.hpp;Step.cpp;Steps.hpp;Steps.cpp;TemporalConstraints.hpp;TemporalConstraints.cpp")