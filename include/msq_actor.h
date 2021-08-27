#ifndef MSQ_ACTOR_H
#define MSQ_ACTOR_H
#include "burst_conf.h"

#include "msq_host.h"
#include <fstream>
using namespace std; 


//This class is an actor inside a Msq Host, this actor receives packages from a specific sensor
//Then all actors on the same host have their info collected by the msq_host
class Msq_actor
{
    private:
        string host_name;  //Name of the host that this actor is in
        string connected_sensor_name; //Name of the sensor this actor communicates with
        Msq_host *host; //Host class that manages this actor
        int actor_id;// Position of this actor on the host sensor list

        simgrid::s4u::Mailbox* receive_mailbox; //This node has one mailbox for each sensor to receive their info

        
        //Interval information
        string burst_config_id;  
        vector<interval> intervals;

        
        int num_intervals; //Number of intervals in the burst_config
        

        //Streaming arguments, these are used when creating the msq_host class, to make a stream_buffer
        int window_size;
        int buffer_size;
        float stream_timeout;

    public:
        
        Msq_actor(vector<string> args); //Constructor
        void operator()(void);          //This is the function that will first run when the platform executes
        void receive_packages();        //Receives packages from the connected sensor

        Msq_host* fetch_host();         //Gets the host manager class for this actor


        

};

#endif