#ifndef SENSOR_H
#define SENSOR_H
#include <simgrid/s4u.hpp>
#include "burst_conf.h"


using namespace std; 
//This class represents an actor of a sensor
class Sensor
{
    private:
        int test_parameter;
        simgrid::s4u::Host * host;
        string host_name;

    public:
        
        Sensor(vector<string> args); //Constructor
        
        void operator()(void); //This is the function that will first run when the platform executes
        void burst(int num_packages, long int size , double end_time);   //Makes sensor send its stream data to its msq_nodes

};

#endif