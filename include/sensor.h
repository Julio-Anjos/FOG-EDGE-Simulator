#ifndef SENSOR_H
#define SENSOR_H
#include <simgrid/s4u.hpp>


using namespace std; 

class Sensor
{
    private:
        int test_parameter;

    public:
        
        Sensor(vector<string> args); //Constructor
        
        void operator()(void); //This is the function that will first run when the platform executes
};

#endif