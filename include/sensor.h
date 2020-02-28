#ifndef SENSOR_H
#define SENSOR_H
#include "burst_conf.h"


using namespace std; 
//This class represents an actor of a sensor
class Sensor
{
    private:
        string connected_msq_node;    //Msq node that is connected to this sensor
        string burst_config_id;       //Burst configuration that its being used
        int num_concurrent_sensors; //Number of sensors that are also connected to the msq_node
        
        simgrid::s4u::Mailbox* mailbox;     
        simgrid::s4u::Mailbox* msq_mailbox; //mailbox of target msq

        vector<interval> intervals; //intervals this node needs to send, defined on the burst config

        simgrid::s4u::Host * host;
        string host_name;

        int num_sensors; //number of sensor this node divides a msq node with

    public:
        
        Sensor(vector<string> args); //Constructor
        
        
        void get_msq_information();//Receiving starting information from the msq_node (burst config and host name)

        void operator()(void); //This is the function that will first run when the platform executes
        
        void send_packages(float end_time, int num_packages, int package_size);   //Makes sensor send its stream data to its msq_nodes

};

#endif