#ifndef MSQ_ACTOR_H
#define MSQ_ACTOR_H
#include "burst_conf.h"
#include "stream_buffer.h"
#include <fstream>
using namespace std; 

class Msq_actor
{
    private:
        
        simgrid::s4u::Mailbox* sensor_mailbox;
        simgrid::s4u::Mailbox* receive_mailbox; //This node has one mailbox for each sensor to receive their info

        string connected_sensor_name; //name of the sensor this actor communicates with

        string burst_config_id;  
        vector<interval> intervals;
        int num_intervals; //Number of intervals in the burst_config
        int num_sensors;
        
        ofstream sensor_stream_logfile; //file that logs information about the stream between sensor and msq node

        //Stream_buffer *streaming_buffer; //Buffer that controls when data will be send to be processed

        simgrid::s4u::Host *host;  
        string host_name;
    
    public:
        
        Msq_actor(vector<string> args); //Constructor
        
        void operator()(void); //This is the function that will first run when the platform executes
        void receive_packages(); //Receives packages from the

        void update_buffer(int num_bytes, double current_time); //Updates the buffer and tests if data must/can be executed
};

#endif