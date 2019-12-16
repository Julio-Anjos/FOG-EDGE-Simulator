#ifndef SENSOR_H
#define SENSOR_H
#include <simgrid/s4u.hpp>


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
};

#endif