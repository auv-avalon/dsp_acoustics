#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE "math"
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <cmath>
#include "../src/zcr.h"
#include "../src/xcorr.h"

//test zero crossing rate
BOOST_AUTO_TEST_CASE(test_zcr)
{
    std::vector<float> signal;
    float value = -1.0f;
    for(unsigned i = 0; i < 100; i++)
    {
        signal.push_back(value);
        value *= -1.0f;
        value += copysign((float)i, value);
    }
    
    unsigned zcr = dsp::zeroCrossingRate<float>(signal.data(), 2);
    BOOST_CHECK_EQUAL(zcr,1);
    zcr = dsp::zeroCrossingRate<float>(signal.data(), 10);
    BOOST_CHECK_EQUAL(zcr,9);
    zcr = dsp::zeroCrossingRate<float>(signal.data(), 100);
    BOOST_CHECK_EQUAL(zcr,99);
    zcr = dsp::zeroCrossingRate<float>(signal.data(), 1);
    BOOST_CHECK_EQUAL(zcr,0);
    zcr = dsp::zeroCrossingRate<float>(signal.data(), 0);
    BOOST_CHECK_EQUAL(zcr,0);
    
    signal.clear();
    signal.push_back(-1.0);
    signal.push_back(0.0);
    signal.push_back(1.0);
    zcr = dsp::zeroCrossingRate<float>(signal.data(), 3);
    BOOST_CHECK_EQUAL(zcr,1);
    
    signal.clear();
    signal.push_back(1.0);
    signal.push_back(0.0);
    signal.push_back(0.0);
    signal.push_back(0.0);
    signal.push_back(-1.0);
    zcr = dsp::zeroCrossingRate<float>(signal.data(), 5);
    BOOST_CHECK_EQUAL(zcr,1);
    
    signal.clear();
    signal.push_back(-1.0);
    signal.push_back(0.0);
    signal.push_back(-1.0);
    signal.push_back(1.0);
    signal.push_back(0.0);
    signal.push_back(1.0);
    zcr = dsp::zeroCrossingRate<float>(signal.data(), 6);
    BOOST_CHECK_EQUAL(zcr,1);
}