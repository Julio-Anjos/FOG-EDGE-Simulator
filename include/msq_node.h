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
        


        simgrid::s4u::Host * host;  
        string host_name;
    
    public:
        
        Msq_node(vector<string> args); //Constructor
        
        void operator()(void); //This is the function that will first run when the platform executes
        void receive_burst(); //Gets info from one of the sensors
};

#endif