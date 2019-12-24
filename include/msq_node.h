#ifndef MSQ_NODE_H
#define MSQ_NODE_H

using namespace std; 

class Msq_node
{
    private:
        int num_sensors;    //Num of sensors connected to this msq node
        int num_bursts;  //Number of intervals in the burst_config

        simgrid::s4u::Host * host;  
        string host_name;
        simgrid::s4u::Mailbox* mailbox;
    
    public:
        
        Msq_node(vector<string> args); //Constructor
        
        void operator()(void); //This is the function that will first run when the platform executes
        void receive(); //Temporary
};

#endif