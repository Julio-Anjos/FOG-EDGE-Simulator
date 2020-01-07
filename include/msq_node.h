#ifndef MSQ_NODE_H
#define MSQ_NODE_H
#include "burst_conf.h"
using namespace std; 

class Msq_node
{
    private:
        
        vector<simgrid::s4u::Mailbox*> sensor_mailboxes;
        vector<simgrid::s4u::Mailbox*> receive_mailboxes; //This node has one mailbox for each sensor to receive their info

        string burst_config_id;  
        vector<interval> bursts;
        int num_bursts; //Number of intervals in the burst_config
        int num_sensors;
        
        int window_size; //Size in bytes, when this window fills up, the data will be send to be processed, unless warned by the processing unit that it cant. 
        int buffer_size; //Size in bytes, the total buffer size, if the buffer fills up without the data being processed, the sensors are warned to stop sending data.
        float stream_timeout;  //Amount of time in seconds before the data in the buffer is send to be processed even if the window size hasn't completly filled up.

        simgrid::s4u::Host * host;  
        string host_name;
    
    public:
        
        Msq_node(vector<string> args); //Constructor
        
        void operator()(void); //This is the function that will first run when the platform executes
        void receive_burst(); //Gets info from one of the sensors
};

#endif