#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE "math"
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <cmath>
#include <boost/scoped_ptr.hpp>

#include "dsp_acoustics/zcr.h"
#include "dsp_acoustics/xcorr.h"
#include "dsp_acoustics/GraphUtils.h"


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

void generateSignal(std::vector<float>& sig, double resolution = 1, unsigned int offset = 0)
{
    for(unsigned int i = 0; i < sig.size(); i++)
    {
        sig[i] = (float)sin((double)(i + offset) / (resolution*M_PI));
    }
};

//test cross correlation
BOOST_AUTO_TEST_CASE(test_xcorr)
{
    boost::scoped_ptr<dsp::Xcorr> xcorr(new dsp::Xcorr(100));
    std::vector<float> signal_1(100, 0.0f);
    std::vector<float> signal_2(100, 0.0f);
    
    // one peak, gap of 20
    signal_1[50] = 1.0f;
    signal_2[30] = 1.0f;
    int gap = xcorr->calculate(signal_1.data(), signal_2.data());
    BOOST_CHECK_EQUAL(gap,20);
    BOOST_CHECK_EQUAL(1.0,xcorr->getCorrelation());
    signal_1[50] = 0.0f;
    signal_2[30] = 0.0f;
    
    // continuous signal, gap of 2
    generateSignal(signal_1, 0.5);
    generateSignal(signal_2, 0.5, 2);
    gap = xcorr->calculate(signal_1.data(), signal_2.data());
    BOOST_CHECK_EQUAL(gap,2);
    
    // continuous signal, gap of 5
    generateSignal(signal_2, 0.5, 5);
    gap = xcorr->calculate(signal_1.data(), signal_2.data());
    BOOST_CHECK_EQUAL(gap,5);
    
    // continuous signal, gap of 6 is too high, so it should select -4
    generateSignal(signal_2, 0.5, 6);
    gap = xcorr->calculate(signal_1.data(), signal_2.data());
    BOOST_CHECK_EQUAL(gap,-4);
    
    generateSignal(signal_1, 1);
    std::vector<float> signal_3(100, 0.0f);
    std::vector<float> signal_4(100, 0.0f);
   
    // non-continuous signal, gap of 8
    unsigned offset = 8;
    for(unsigned i = 0; i < 20; i++)
    {
        signal_3[30+i]=signal_1[i];
        signal_4[30+offset+i]=signal_1[i];
    }
    gap = xcorr->calculate(signal_4.data(), signal_3.data());
    BOOST_CHECK_EQUAL(gap,8);

    // non-continuous signal, gap of 20
    signal_4.clear();
    signal_4.resize(100, 0.0);
    offset = 20;
    for(unsigned i = 0; i < 20; i++)
    {
        signal_4[30+offset+i]=signal_1[i];
    }
    gap = xcorr->calculate(signal_4.data(), signal_3.data());
    BOOST_CHECK_EQUAL(gap,20);
    
    // non-continuous signal, gap of 50
    signal_4.clear();
    signal_4.resize(100, 0.0);
    offset = 50;
    for(unsigned i = 0; i < 20; i++)
    {
        signal_4[30+offset+i]=signal_1[i];
    }
    gap = xcorr->calculate(signal_4.data(), signal_3.data());
    BOOST_CHECK_EQUAL(gap,50);
}