
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE "math"
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>
#include "../src/FIRFilter.h"
#include "../src/FitPolynom.h"
#include <algorithm>


void out(std::vector<unsigned char>::iterator iter)
{
    std::cout << (int)*iter << std::endl;
}

//test finding local min max
BOOST_AUTO_TEST_CASE(test_find_min_max)
{
    //first test
    std::vector<unsigned char> values;
    values.push_back(1);
    values.push_back(2);
    values.push_back(3);
    values.push_back(4);
    values.push_back(5);
    values.push_back(6);
    values.push_back(7);
    values.push_back(8);

    std::vector<std::vector<unsigned char>::iterator> result;
    dsp::findLocalMinMax(values.begin(),values.end(),result);
    BOOST_CHECK_EQUAL(result.size(),2);
    BOOST_CHECK_EQUAL(*result[0],1);
    BOOST_CHECK_EQUAL(*result[1],8);

    //second test
    result.clear();
    values.clear();
    values.push_back(1);
    values.push_back(2);
    values.push_back(10);
    values.push_back(8);
    values.push_back(5);
    values.push_back(10);
    values.push_back(7);
    values.push_back(8);
    values.push_back(9);

    dsp::findLocalMinMax(values.begin(),values.end(),result);
    BOOST_CHECK_EQUAL(result.size(),6);
    BOOST_CHECK_EQUAL(*result[0],1);
    BOOST_CHECK_EQUAL(*result[1],10);
    BOOST_CHECK_EQUAL(*result[2],5);
    BOOST_CHECK_EQUAL(*result[3],10);
    BOOST_CHECK_EQUAL(*result[4],7);
    BOOST_CHECK_EQUAL(*result[5],9);

    //for_each(result.begin(),result.end(),out);
}

//test moving average filter
BOOST_AUTO_TEST_CASE(test_moving_average_filter)
{
    std::vector<unsigned char> values;
    values.push_back(1);
    values.push_back(2);
    values.push_back(3);
    values.push_back(4);
    values.push_back(5);
    values.push_back(6);
    values.push_back(7);
    values.push_back(8);

    std::vector<unsigned char> result;
    result.resize(values.size());
    typedef std::vector<unsigned char>::iterator iterator;

    //this should be equivalent to window_size of 1 and should therefore change nothing 
    dsp::movingAverageFilterSym<iterator,iterator,unsigned int>(values.begin(),values.end(),result.begin(),2);
    BOOST_CHECK_EQUAL(result.size(),values.size());
    for(int i=0;i < result.size();++i)
        BOOST_CHECK_EQUAL(result[i], values[i]);

    //applying a window of 5
    dsp::movingAverageFilterSym<iterator,iterator,unsigned int>(values.begin(),values.end(),result.begin(),5);
    BOOST_CHECK_EQUAL(result.size(),values.size());
    BOOST_CHECK_EQUAL(result[0],2);
    BOOST_CHECK_EQUAL(result[1],2);
    BOOST_CHECK_EQUAL(result[2],3);
    BOOST_CHECK_EQUAL(result[3],4);
    BOOST_CHECK_EQUAL(result[4],5);
    BOOST_CHECK_EQUAL(result[5],6);
    BOOST_CHECK_EQUAL(result[6],6);
    BOOST_CHECK_EQUAL(result[7],7);

    //applying a window which is too big
    BOOST_CHECK_THROW((dsp::movingAverageFilterSym<iterator,iterator,unsigned int> (values.begin(),values.end(),result.begin(),9)),std::runtime_error);

    //applying a window which has the maximum size
    BOOST_CHECK_NO_THROW((dsp::movingAverageFilterSym<iterator,iterator,unsigned int> (values.begin(),values.end(),result.begin(),8)));

    //check simple syntax
    BOOST_CHECK_NO_THROW((dsp::movingAverageFilterSymI(values.begin(),values.end(),result.begin(),8)));
    BOOST_CHECK_NO_THROW((dsp::movingAverageFilterSymD(values.begin(),values.end(),result.begin(),8)));
}

//test if min max search is working
BOOST_AUTO_TEST_CASE(test_find_local_min_max)
{
  std::vector<float> values;
  values.push_back(0);
  values.push_back(1);
  values.push_back(2);
  values.push_back(3);
  values.push_back(4);
  values.push_back(4);
  values.push_back(5);
  values.push_back(4);
  values.push_back(3);
  values.push_back(2);
  values.push_back(1);
  values.push_back(0);
  values.push_back(0);
  std::vector<std::vector<float>::iterator > result;
  dsp::findLocalMinMax(1,1,values.begin(),values.end(),result);
  BOOST_CHECK_EQUAL(3,result.size());
  BOOST_CHECK_EQUAL(0,*result[0]);
  BOOST_CHECK_EQUAL(5,*result[1]);
  BOOST_CHECK_EQUAL(0,*result[2]);

  values.push_back(0);
  values.push_back(1);
  values.push_back(2);
  values.push_back(3);
  values.push_back(4);
  values.push_back(3);
  values.push_back(6);
  values.push_back(4);
  values.push_back(3);
  values.push_back(4);
  values.push_back(1);
  values.push_back(0);
  result.clear();
  dsp::findLocalMinMax(2,2,values.begin(),values.end(),result);
  BOOST_CHECK_EQUAL(6,result.size());
  BOOST_CHECK_EQUAL(0,*result[0]);
  BOOST_CHECK_EQUAL(5,*result[1]);
  BOOST_CHECK_EQUAL(0,*result[2]);
  BOOST_CHECK_EQUAL(0,*result[3]);
  BOOST_CHECK_EQUAL(6,*result[4]);
  BOOST_CHECK_EQUAL(0,*result[5]);
}


//test if min max search is working
BOOST_AUTO_TEST_CASE(test_find_local_min_max2)
{
  std::vector<float> values;
  values.push_back(0);
  values.push_back(3);
  values.push_back(2);
  values.push_back(3);
  values.push_back(2);
  values.push_back(5);
  values.push_back(4);
  values.push_back(10);
  values.push_back(9);
  values.push_back(10);
  values.push_back(9);
  values.push_back(11);
  values.push_back(5);
  values.push_back(7);
  values.push_back(6);
  values.push_back(7);
  values.push_back(0);
  std::vector<std::vector<float>::iterator > result;
  dsp::findLocalMinMax(1,3,values.begin(),values.end(),result);
  BOOST_CHECK_EQUAL(3,result.size());
  BOOST_CHECK_EQUAL(0,*result[0]);
  BOOST_CHECK_EQUAL(11,*result[1]);
  BOOST_CHECK_EQUAL(0,*result[2]);
}

//test if min max search is working
BOOST_AUTO_TEST_CASE(test_find_local_min_max3)
{
  std::vector<float> values;
  values.push_back(0);
  values.push_back(3);
  values.push_back(2);
  values.push_back(3);
  values.push_back(2);
  values.push_back(5);
  values.push_back(4);
  values.push_back(10);
  values.push_back(9);
  values.push_back(10);
  values.push_back(9);
  values.push_back(11);
  values.push_back(5);
  values.push_back(8);
  values.push_back(6);
  values.push_back(7);
  values.push_back(0);
  std::vector<std::vector<float>::iterator > result;
  dsp::findLocalMinMax(1,3,values.begin(),values.end(),result);
  BOOST_CHECK_EQUAL(6,result.size());
  BOOST_CHECK_EQUAL(0,*result[0]);
  BOOST_CHECK_EQUAL(11,*result[1]);
  BOOST_CHECK_EQUAL(5,*result[2]);
}

//test if min max search is working
BOOST_AUTO_TEST_CASE(test_find_local_min_max4)
{
  std::vector<float> values;
  values.push_back(0);
  values.push_back(3);
  values.push_back(2);
  values.push_back(3);
  values.push_back(2);
  values.push_back(5);
  values.push_back(4);
  values.push_back(10);
  values.push_back(9);
  values.push_back(10);
  values.push_back(9);
  values.push_back(11);
  values.push_back(5);
  values.push_back(8);
  values.push_back(6);
  values.push_back(7);
  values.push_back(0);
  values.push_back(0);
  std::vector<std::vector<float>::iterator > result;
  dsp::findLocalMinMax(4,3,values.begin(),values.end(),result);
  BOOST_CHECK_EQUAL(3,result.size());
  BOOST_CHECK_EQUAL(0,*result[0]);
  BOOST_CHECK_EQUAL(11,*result[1]);
  BOOST_CHECK_EQUAL(0,*result[2]);
}

//test if polynomial fit is working
BOOST_AUTO_TEST_CASE(test_poly_fit)
{
  std::vector<float> values;
  values.push_back(0);
  values.push_back(3);
  values.push_back(2);
  values.push_back(3);
  values.push_back(2);
  values.push_back(5);
  values.push_back(4);
  values.push_back(10);
  values.push_back(9);
  values.push_back(10);
  values.push_back(9);
  values.push_back(11);
  values.push_back(5);
  values.push_back(8);
  values.push_back(6);
  values.push_back(7);
  values.push_back(0);

  std::vector<float> result;
  double chisq=0;
  dsp::fitPolynom(3,values.begin(),values.end(),result,chisq);
  BOOST_CHECK_EQUAL(3,result.size());
  BOOST_CHECK(result[0] > -1.28 && result[0] < -1.279);
  BOOST_CHECK(result[1] > 2.09 && result[1] < 2.093);
  BOOST_CHECK(result[2] > -0.1129 && result[2] < -0.1128);

  std::cout << "polynomial fit: " << result[0] << " " << result[1] << " " << result[2] << " " << sqrt(chisq) << std::endl ;
}

//test if polynomial fit is working
BOOST_AUTO_TEST_CASE(test_find_local_min_max5)
{
  float values_[1080] = {18.0, 18.0, 17.0, 17.0, 17.0, 17.0, 17.0, 17.0, 17.0, 18.0, 18.0, 18.0, 18.0, 18.0, 18.0, 18.0, 18.0, 18.0, 18.0, 18.0, 18.0, 18.0, 18.0, 18.0, 19.0, 19.0, 19.0, 19.0, 19.0, 19.0, 19.0, 19.0, 19.0, 18.0, 19.0, 18.0, 18.0, 19.0, 19.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 19.0, 19.0, 18.0, 18.0, 18.0, 19.0, 19.0, 19.0, 19.0, 19.0, 19.0, 20.0, 19.0, 19.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 19.0, 18.0, 18.0, 17.0, 18.0, 18.0, 17.0, 17.0, 18.0, 17.0, 16.0, 16.0, 14.0, 12.0, 11.0, 10.0, 10.0, 11.0, 12.0, 14.0, 16.0, 17.0, 19.0, 21.0, 22.0, 23.0, 24.0, 24.0, 24.0, 23.0, 22.0, 21.0, 20.0, 20.0, 20.0, 19.0, 19.0, 20.0, 20.0, 19.0, 19.0, 20.0, 20.0, 20.0, 20.0, 21.0, 21.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 19.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 21.0, 22.0, 22.0, 22.0, 21.0, 20.0, 20.0, 19.0, 19.0, 20.0, 21.0, 22.0, 22.0, 22.0, 22.0, 21.0, 20.0, 20.0, 20.0, 21.0, 21.0, 21.0, 21.0, 20.0, 20.0, 19.0, 19.0, 19.0, 19.0, 19.0, 20.0, 20.0, 20.0, 21.0, 21.0, 21.0, 21.0, 21.0, 20.0, 20.0, 20.0, 19.0, 19.0, 19.0, 19.0, 18.0, 18.0, 18.0, 18.0, 17.0, 15.0, 13.0, 12.0, 10.0, 8.0, 9.0, 10.0, 11.0, 13.0, 16.0, 19.0, 21.0, 23.0, 24.0, 26.0, 26.0, 27.0, 27.0, 27.0, 26.0, 25.0, 24.0, 24.0, 23.0, 22.0, 22.0, 22.0, 22.0, 23.0, 24.0, 24.0, 24.0, 23.0, 22.0, 22.0, 23.0, 22.0, 23.0, 23.0, 22.0, 22.0, 22.0, 22.0, 22.0, 22.0, 22.0, 22.0, 22.0, 23.0, 23.0, 23.0, 23.0, 23.0, 23.0, 23.0, 23.0, 23.0, 23.0, 23.0, 22.0, 23.0, 24.0, 24.0, 24.0, 25.0, 25.0, 25.0, 26.0, 27.0, 28.0, 28.0, 29.0, 29.0, 29.0, 29.0, 29.0, 30.0, 31.0, 33.0, 35.0, 38.0, 40.0, 44.0, 48.0, 54.0, 60.0, 67.0, 73.0, 77.0, 79.0, 76.0, 70.0, 63.0, 55.0, 48.0, 43.0, 38.0, 33.0, 29.0, 26.0, 24.0, 23.0, 22.0, 22.0, 22.0, 22.0, 23.0, 23.0, 22.0, 21.0, 19.0, 17.0, 14.0, 12.0, 9.0, 8.0, 9.0, 10.0, 12.0, 16.0, 19.0, 21.0, 24.0, 26.0, 28.0, 28.0, 29.0, 29.0, 28.0, 27.0, 26.0, 25.0, 24.0, 23.0, 23.0, 23.0, 24.0, 24.0, 24.0, 24.0, 23.0, 23.0, 23.0, 23.0, 23.0, 23.0, 23.0, 23.0, 22.0, 23.0, 23.0, 24.0, 24.0, 25.0, 25.0, 24.0, 24.0, 24.0, 24.0, 23.0, 23.0, 23.0, 23.0, 22.0, 22.0, 22.0, 22.0, 22.0, 23.0, 23.0, 23.0, 23.0, 23.0, 24.0, 24.0, 24.0, 25.0, 24.0, 24.0, 24.0, 23.0, 23.0, 23.0, 23.0, 23.0, 23.0, 23.0, 24.0, 24.0, 25.0, 25.0, 25.0, 25.0, 25.0, 24.0, 23.0, 23.0, 23.0, 23.0, 24.0, 24.0, 24.0, 24.0, 23.0, 23.0, 23.0, 23.0, 23.0, 23.0, 23.0, 24.0, 24.0, 24.0, 24.0, 23.0, 23.0, 22.0, 22.0, 22.0, 22.0, 22.0, 22.0, 21.0, 20.0, 18.0, 16.0, 14.0, 12.0, 12.0, 12.0, 13.0, 15.0, 18.0, 21.0, 24.0, 26.0, 28.0, 30.0, 30.0, 31.0, 31.0, 30.0, 29.0, 28.0, 27.0, 26.0, 25.0, 24.0, 23.0, 23.0, 24.0, 24.0, 24.0, 24.0, 24.0, 24.0, 24.0, 24.0, 24.0, 24.0, 24.0, 23.0, 23.0, 23.0, 23.0, 23.0, 23.0, 24.0, 24.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 26.0, 25.0, 25.0, 25.0, 24.0, 24.0, 24.0, 24.0, 24.0, 24.0, 23.0, 23.0, 24.0, 24.0, 24.0, 25.0, 25.0, 25.0, 24.0, 24.0, 24.0, 24.0, 24.0, 24.0, 24.0, 23.0, 23.0, 23.0, 22.0, 22.0, 23.0, 23.0, 24.0, 25.0, 25.0, 25.0, 25.0, 26.0, 26.0, 26.0, 26.0, 26.0, 25.0, 24.0, 24.0, 24.0, 24.0, 23.0, 24.0, 24.0, 23.0, 23.0, 23.0, 23.0, 22.0, 22.0, 21.0, 19.0, 17.0, 14.0, 12.0, 10.0, 9.0, 10.0, 12.0, 15.0, 18.0, 21.0, 23.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0, 30.0, 30.0, 29.0, 28.0, 27.0, 26.0, 25.0, 25.0, 25.0, 24.0, 24.0, 24.0, 23.0, 23.0, 23.0, 23.0, 23.0, 24.0, 25.0, 25.0, 25.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 25.0, 24.0, 24.0, 24.0, 24.0, 24.0, 24.0, 24.0, 24.0, 24.0, 24.0, 25.0, 25.0, 26.0, 26.0, 26.0, 26.0, 26.0, 25.0, 25.0, 24.0, 23.0, 23.0, 23.0, 24.0, 24.0, 24.0, 24.0, 24.0, 24.0, 25.0, 26.0, 27.0, 27.0, 27.0, 26.0, 26.0, 25.0, 25.0, 25.0, 25.0, 25.0, 26.0, 26.0, 25.0, 25.0, 24.0, 24.0, 24.0, 24.0, 24.0, 24.0, 25.0, 25.0, 25.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 25.0, 24.0, 24.0, 24.0, 23.0, 24.0, 25.0, 25.0, 25.0, 24.0, 23.0, 23.0, 22.0, 21.0, 21.0, 20.0, 18.0, 16.0, 14.0, 12.0, 11.0, 12.0, 13.0, 16.0, 18.0, 21.0, 24.0, 27.0, 29.0, 31.0, 33.0, 34.0, 35.0, 34.0, 34.0, 33.0, 31.0, 30.0, 29.0, 28.0, 27.0, 27.0, 26.0, 26.0, 25.0, 25.0, 25.0, 25.0, 26.0, 27.0, 26.0, 26.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 28.0, 27.0, 27.0, 26.0, 26.0, 26.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 26.0, 26.0, 26.0, 27.0, 27.0, 28.0, 28.0, 28.0, 28.0, 28.0, 28.0, 27.0, 27.0, 26.0, 26.0, 26.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 26.0, 27.0, 27.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 27.0, 27.0, 27.0, 27.0, 27.0, 26.0, 26.0, 26.0, 25.0, 25.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 24.0, 24.0, 24.0, 23.0, 23.0, 22.0, 22.0, 21.0, 20.0, 18.0, 17.0, 16.0, 14.0, 13.0, 12.0, 10.0, 9.0, 8.0, 6.0, 5.0, 4.0, 3.0, 3.0, 4.0, 6.0, 8.0, 10.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 18.0, 19.0, 18.0, 18.0, 18.0, 17.0, 17.0, 17.0, 17.0, 17.0, 18.0, 18.0, 18.0, 18.0, 18.0, 17.0, 17.0, 17.0, 17.0, 17.0, 17.0, 16.0, 16.0, 16.0, 16.0, 17.0, 16.0, 16.0, 17.0, 17.0, 17.0, 16.0, 17.0, 16.0, 16.0, 17.0, 16.0, 16.0, 15.0, 15.0, 14.0, 14.0, 14.0, 15.0, 15.0, 16.0, 15.0, 15.0, 15.0, 15.0, 14.0, 14.0, 14.0, 14.0, 14.0, 14.0, 14.0, 13.0, 13.0, 13.0, 13.0, 12.0, 12.0, 11.0, 11.0, 11.0, 12.0, 13.0, 13.0, 13.0, 12.0, 12.0, 12.0, 11.0, 11.0, 10.0, 10.0, 10.0, 10.0, 11.0, 12.0, 13.0, 13.0, 13.0, 14.0, 15.0, 16.0, 17.0, 19.0, 22.0, 24.0, 26.0, 27.0, 28.0, 28.0, 27.0, 25.0, 23.0, 20.0, 19.0, 17.0, 16.0, 17.0, 19.0, 20.0, 23.0, 26.0, 28.0, 30.0, 32.0, 33.0, 33.0, 34.0, 33.0, 33.0, 32.0, 31.0, 30.0, 28.0, 27.0, 26.0, 26.0, 25.0, 26.0, 25.0, 25.0, 26.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 26.0, 26.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 26.0, 26.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 26.0, 26.0, 26.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 26.0, 26.0, 26.0, 26.0, 26.0, 25.0, 26.0, 26.0, 26.0, 26.0, 27.0, 26.0, 26.0, 26.0, 26.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 26.0, 27.0, 27.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 26.0, 27.0, 27.0, 27.0, 26.0, 27.0, 26.0, 26.0, 25.0, 23.0, 21.0, 19.0, 16.0, 14.0, 11.0, 10.0, 10.0, 10.0, 11.0, 14.0, 17.0, 20.0, 22.0, 25.0, 27.0, 29.0, 31.0, 32.0, 33.0, 33.0, 33.0, 32.0, 31.0, 30.0, 29.0, 27.0, 27.0, 27.0, 26.0, 26.0, 27.0, 27.0, 27.0, 27.0, 26.0, 26.0, 26.0, 26.0, 26.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 21.0, 21.0, 21.0};
  std::vector<float> values;
  for(int i=0;i<1080;++i)
    values.push_back(values_[i]);

  std::vector<std::vector<float>::iterator > result;
  dsp::findLocalMinMax(25,25,values.begin(),values.end(),result);
  BOOST_CHECK_EQUAL(6,result.size());
  BOOST_CHECK_EQUAL(8,*result[0]);
  BOOST_CHECK_EQUAL(79,*result[1]);
  BOOST_CHECK_EQUAL(8,*result[2]);
}
