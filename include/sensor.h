#ifndef SENSOR_H
#define SENSOR_H
using namespace std; 
#include <simgrid/s4u.hpp>

class Sensor
{
private:
    simgrid::s4u::Host* host;
    string name;
    
public:
   
    Sensor(simgrid::s4u::Host* host);
 
    void display_loaded();
};
 
#endif