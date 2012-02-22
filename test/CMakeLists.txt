
rock_testsuite(test_suite test_suite.cpp 
    DEPS_PKGCONFIG gsl)

rock_testsuite(test_sonar_beam_to test_SonarBeamToSonarScan.cpp
    DEPS_PKGCONFIG base-types)
#TARGET_LINK_LIBRARIES(math_testsuite ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES} ${Boost_SYSTEM_LIBRARIES} ${GSL_LIBRARIES} )

