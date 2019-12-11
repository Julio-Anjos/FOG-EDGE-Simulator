#ifndef SENSOR_H
#define SENSOR_H
using namespace std; 
class Sensor
{
private:
    simgrid::s4u::Host* host;
    string name;
    
 
public:
   
    Sensor(simgrid::s4u::Host* host);
 
    void something();
};
 
#endif