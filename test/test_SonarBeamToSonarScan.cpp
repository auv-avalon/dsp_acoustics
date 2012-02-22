

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE "math"
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>
#include "../src/SonarBeamToSonarScan.h"
#include <algorithm>


//test SonarBeamToSonarScan
BOOST_AUTO_TEST_CASE(test_SonarBeamToSonarScan)
{
    dsp::SonarBeamToSonarScan converter;
    base::samples::SonarBeam sonar_beam;

    sonar_beam.beam.resize(25);
    sonar_beam.sampling_interval = 0.1;
    sonar_beam.speed_of_sound = 1500;
    sonar_beam.beamwidth_horizontal = 0.1;
    sonar_beam.beamwidth_vertical = 0.1;
    sonar_beam.time = base::Time::now();

    //this is wrong ordered 
    //the beams in the sonar scan are ordered from left to right 
    for(int i=0;i<20;++i)
    {
        sonar_beam.beam[i] = i;
        sonar_beam.bearing = base::Angle::fromDeg(i);
        BOOST_CHECK(!converter.addSonarBeam(sonar_beam));
        BOOST_CHECK(converter.getNumberOfBeams() == i);
    }
    sonar_beam.bearing = base::Angle::fromDeg(18);
    BOOST_CHECK(converter.addSonarBeam(sonar_beam));

    base::samples::SonarScan sonar_scan;
    converter.getSonarScan(sonar_scan,false);
   
    BOOST_CHECK(sonar_scan.number_of_bins == 25);
    BOOST_CHECK(sonar_scan.number_of_beams == 20);
    BOOST_CHECK(sonar_scan.getStartBearing().rad == base::Angle::fromDeg(19).rad);
    BOOST_CHECK_SMALL(sonar_scan.getEndBearing().rad, 0.0001);
    //check that the sonar scan was generated right despite of the order
    //the sonar beams were added 
    for(int i=0;i<20;++i)
        BOOST_CHECK(sonar_scan.data[(19-i)+i*sonar_scan.number_of_bins] == (19- i));

    //for manual control plot sonar scan
    std::cout << "raw data of the sonar scan:" << std::endl;
    for(int i=0;i<20;++i)
    {
        for(int i2=0;i2<25;++i2)
            std::cout << (int) sonar_scan.data[i2+i*sonar_scan.number_of_bins] << " ";
        std::cout << std::endl;
    }

    //test that we will get an exception if we are adding the same beam all other again
    //this should happen after adding more than 180 sonar beams
    converter.reset();
    for(int i=0;i<= 180;++i)
    {
        sonar_beam.bearing = base::Angle::fromRad(i*0.001);
        BOOST_CHECK(!converter.addSonarBeam(sonar_beam));
    }
    BOOST_CHECK_THROW(converter.addSonarBeam(sonar_beam),std::runtime_error);

    //test that an exception is thrown if a sonar_beam was added for the same bearing 
    converter.reset();
    sonar_beam.bearing = base::Angle::fromDeg(0);
    BOOST_CHECK(!converter.addSonarBeam(sonar_beam));
    for(int i=0;i<20;++i)
    {
        sonar_beam.beam[i] = i;
        sonar_beam.bearing = base::Angle::fromDeg(i+0.1);
        BOOST_CHECK(!converter.addSonarBeam(sonar_beam));
        BOOST_CHECK(converter.getNumberOfBeams() == i+1);
    }
    sonar_beam.bearing = base::Angle::fromDeg(18);
    BOOST_CHECK_THROW(converter.addSonarBeam(sonar_beam),std::runtime_error);
    
    //test that 360 Degree separation is working
    converter.reset();
    for(int i=0;i<180;++i)
    {
        sonar_beam.bearing = base::Angle::fromDeg(i*2);
        BOOST_CHECK(!converter.addSonarBeam(sonar_beam));
        BOOST_CHECK(converter.getNumberOfBeams() == i);
    }
    sonar_beam.bearing = base::Angle::fromDeg(360);
    BOOST_CHECK(converter.addSonarBeam(sonar_beam));
    converter.getSonarScan(sonar_scan,false);
    BOOST_CHECK(sonar_scan.number_of_bins == 25);
    BOOST_CHECK(sonar_scan.number_of_beams == 180);
    BOOST_CHECK_SMALL(sonar_scan.getStartBearing().rad-base::Angle::fromDeg(358).rad,0.0001);
    BOOST_CHECK_SMALL(sonar_scan.getEndBearing().rad - base::Angle::fromDeg(0).rad,0.0001);

    //test that 170 - 190 Degree are working 
    converter.reset();
    for(int i=170;i<190;++i)
    {
        sonar_beam.bearing = base::Angle::fromDeg(i);
        BOOST_CHECK(!converter.addSonarBeam(sonar_beam));
    }
    sonar_beam.bearing = base::Angle::fromDeg(189);
    BOOST_CHECK(converter.addSonarBeam(sonar_beam));
    converter.getSonarScan(sonar_scan,false);
    BOOST_CHECK(sonar_scan.number_of_bins == 25);
    BOOST_CHECK(sonar_scan.number_of_beams == 20);
    BOOST_CHECK_SMALL(sonar_scan.getStartBearing().rad -base::Angle::fromDeg(189).rad,0.0001);
    BOOST_CHECK_SMALL(sonar_scan.getEndBearing().rad -base::Angle::fromDeg(170).rad,0.0001);

    //test that 10 - -10 Degree are working 
    converter.reset();
    for(int i=10;i>-10;--i)
    {
        sonar_beam.bearing = base::Angle::fromDeg(i);
        BOOST_CHECK(!converter.addSonarBeam(sonar_beam));
    }
    sonar_beam.bearing = base::Angle::fromDeg(-9);
    BOOST_CHECK(converter.addSonarBeam(sonar_beam));
    converter.getSonarScan(sonar_scan,false);
    BOOST_CHECK(sonar_scan.number_of_bins == 25);
    BOOST_CHECK(sonar_scan.number_of_beams == 20);
    BOOST_CHECK_SMALL(sonar_scan.getStartBearing().rad -base::Angle::fromDeg(10).rad,0.0001);
    BOOST_CHECK_SMALL(sonar_scan.getEndBearing().rad -base::Angle::fromDeg(-9).rad,0.0001);

    //test that ping pong is working 
    converter.reset();
    int counter = 0;
    for(int i=0;i<5;++i)
    {
        for(int i2=-10;i2<=10;++i2)
        {
            sonar_beam.bearing = base::Angle::fromDeg(i2);
            if(converter.addSonarBeam(sonar_beam))
                counter++;
        }
        for(int i2=9;i2>=-9;--i2)
        {
            sonar_beam.bearing = base::Angle::fromDeg(i2);
            if(converter.addSonarBeam(sonar_beam))
               counter++;
        }
    }
    sonar_beam.bearing = base::Angle::fromDeg(-8);
    if(converter.addSonarBeam(sonar_beam))
        counter++;
    BOOST_CHECK(counter == 10);
}


