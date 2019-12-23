#ifndef SENSOR_H
#define SENSOR_H



using namespace std; 
//This class represents an actor of a sensor
class Sensor
{
    private:
        string connected_msq_node;    //Msq node that is connected to this sensor
        string burst_config_id;       //Burst configuration that its being used
        int num_concurrent_sensors; //Number of sensors that are also connected to the msq_node

        simgrid::s4u::Host * host;
        string host_name;

    public:
        
        Sensor(vector<string> args); //Constructor
        
        void operator()(void); //This is the function that will first run when the platform executes
        void burst(int num_packages, long int size , double end_time);   //Makes sensor send its stream data to its msq_nodes

};

#endif