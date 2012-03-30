#ifndef __SONARBEAMTOSONARSCAN_H
#define __SONARBEAMTOSONARSCAN_H 

/*
 * SonarBeamToSonarScan.h - 
 *
 * This Class is for creating SonarScans from SonarBeams 
 *
 * Copyright (C) 2012 aduda
 * German Research Center for Artificial Intelligence
 * DFKI RIC Bremen 2007-2012
 *
 */
#include <vector>
#include <algorithm>
#include <base/samples/sonar_scan.h>

namespace dsp 
{
    class SonarBeamToSonarScan
    {
        public:
            //An error will be thrown if the maximum_number_of_beams are reached without getting a complete sonar scan 
            //this is for protecting components against doing nothing if the sonar is wrongly configured
            SonarBeamToSonarScan(int maximum_number_of_beams = 180):maximum_number_of_beams(maximum_number_of_beams)
            {
                reset();
            }

            //returns true every time a new sonar scan was created from the added sonar beams
            //otherwise if the sonar scan would not be completed it returns false 
            //
            //a sonar scan is regarded as complete if 360° are reached
            //or if the scan direction changed
            //
            //if copy is set to false the data are being swapped instead
            bool addSonarBeam(base::samples::SonarBeam &sonar_beam,bool copy=true)
            {   
                if(current_index >= maximum_number_of_beams)
                    throw std::runtime_error("addSonarBeam: maximum number of beams is reached without getting a full sonar scan!");

                bool result = false;
                float delta1 =0;
                float delta2 =0;

                if(current_index != -1)
                    delta2 = (sonar_beam.bearing - sonar_beams[current_index].bearing).rad;

                if(current_index > 0)
                {
                    delta1 = (sonar_beams[current_index].bearing - sonar_beams[current_index-1].bearing).rad;
                    if(delta1*delta2 <= 0 || total_angle_delta +fabs(delta2) >= 2*M_PI-0.5*delta2)
                    {
                        base::Angle start_angle;
                        base::Angle end_angle;
                        base::Angle angular_resolution;
                        int number_of_bins = 0;

                        //find start and end angle
                        if(delta1 > 0)
                        {
                            start_angle = sonar_beams[current_index].bearing;
                            end_angle = sonar_beams[0].bearing;
                        }
                        else
                        {
                            start_angle = sonar_beams[0].bearing;
                            end_angle = sonar_beams[current_index].bearing;
                        }

                        //find maximum number of bins
                        std::vector<base::samples::SonarBeam>::iterator iter = sonar_beams.begin();
                        for(;iter != sonar_beams.end();++iter)
                            number_of_bins = std::max(number_of_bins,(int)iter->beam.size());

                        //calculate average angular resolution
                        angular_resolution = base::Angle::fromRad(total_angle_delta/current_index);

                        //initialize sonar scan
                        sonar_scan.init(current_index+1,number_of_bins,start_angle,angular_resolution,false,0);
                        sonar_scan.time_beams.resize(current_index+1);

                        //add all sonar_beams to the sonar_scan
                        iter = sonar_beams.begin();
                        std::vector<base::samples::SonarBeam>::iterator end = sonar_beams.begin()+current_index+1;
                        for(;iter != end;++iter)
                        {   
                            //protect against overwriting sonar beams
                            if(sonar_scan.hasSonarBeam(*iter))
                                throw std::runtime_error("addSonarBeam: multiple SonarBeams were added which are overwriting each other!");
                            sonar_scan.addSonarBeam(*iter,false);
                        }

                        //save last received beam and start collecting beams for a new sonar scan 
                        sonar_beams[0].swap(sonar_beams[current_index]);
                        current_index = 0;
                        total_angle_delta = 0;
                        result = true;
                    }
                }

                ++current_index;
                total_angle_delta += fabs(delta2);
                if((int)sonar_beams.size() < current_index+1)
                    sonar_beams.resize(current_index+1);
                if(copy)
                    sonar_beams[current_index] = sonar_beam;
                else
                    sonar_beams[current_index].swap(sonar_beam);

                return result;
            }

            bool addSonarBeam(const base::samples::SonarBeam &sonar_beam)
            {
                base::samples::SonarBeam beam = sonar_beam;
                return addSonarBeam(beam,false);
            }

            //copies the sonar scan into the given one 
            //if copy is set to false the sonar scans are swapped
            void getSonarScan(base::samples::SonarScan &sonar_scan,bool copy=true)
            {
                if(copy)
                    sonar_scan.init(this->sonar_scan,true);
                else
                    sonar_scan.swap(this->sonar_scan);

            }

            int getNumberOfBeams()
            {
                return current_index;
            }

            void reset()
            {
                current_index = -1;
                total_angle_delta = 0;
                sonar_beams.resize(2);
            }

        private:
            std::vector<base::samples::SonarBeam> sonar_beams;
            base::samples::SonarScan sonar_scan;
            int current_index;
            float total_angle_delta;
            int maximum_number_of_beams;

    };
}
#endif
