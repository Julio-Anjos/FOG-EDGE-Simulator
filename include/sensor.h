#ifndef SENSOR_H
#define SENSOR_H
#include "burst_conf.h"
#include <fstream>

using namespace std; 
//This class represents an actor of a sensor



class Sensor
{
    private:
        string connected_msq_name;    //Msq node(host) that is connected to this sensor
        string burst_config_id;       //Burst configuration that its being used
         
        simgrid::s4u::Mailbox* msq_mailbox; //mailbox of target msq_actor

        vector<interval> intervals; //intervals this node needs to send, defined on the burst config

        simgrid::s4u::Host * host;
        string host_name;
        simgrid::s4u::CommPtr comm;
        simgrid::s4u::Host * stream_host;
        simgrid::s4u::ActorPtr stream_actor;
        vector<simgrid::s4u::ActorPtr> actors;
        int num_sensors; //number of sensor this node divides a msq node with
        int sensors_position; //This sensor position relative to other sensors, used to correctly divide packets
        int sensor_real_pos;

        ofstream logfile;

    public:
        
        Sensor(vector<string> args); //Constructor
        vector<simgrid::s4u::CommPtr> pending_comms;
        void operator()(void); //This is the function that will first run when the platform executes
    
        //Makes sensor send a stream of data, from current time to end_time
        int send_packages(float end_time, int num_packages, int package_size);  

};
extern vector<simgrid::s4u::CommPtr> pending_comms;

#endif