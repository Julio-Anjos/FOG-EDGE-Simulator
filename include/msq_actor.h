#ifndef MSQ_ACTOR_H
#define MSQ_ACTOR_H
#include "burst_conf.h"
#include "stream_buffer.h"
#include "msq_host.h"
#include <fstream>
using namespace std; 

class Msq_actor
{
    private:
        string host_name;
        string connected_sensor_name; //name of the sensor this actor communicates with
        Msq_host *host; //Host class that manages this actor
        int actor_id;//position of this actor on the host sensor list, used to communicate with the msq_host

        simgrid::s4u::Mailbox* sensor_mailbox;
        simgrid::s4u::Mailbox* receive_mailbox; //This node has one mailbox for each sensor to receive their info

        
        //Interval information
        string burst_config_id;  
        vector<interval> intervals;
        int num_intervals; //Number of intervals in the burst_config
        
        
        ofstream sensor_stream_logfile; //file that logs information about the stream between sensor and msq node

        //Stream_buffer *streaming_buffer; //Buffer that controls when data will be send to be processed

        
        
    
    public:
        
        Msq_actor(vector<string> args); //Constructor
        void operator()(void); //This is the function that will first run when the platform executes
        void receive_packages(); //Receives packages from the

        Msq_host* fetch_host(); //Gets the host manager class for this actor


        //void update_buffer(int num_bytes, double current_time); //Updates the buffer and tests if data must/can be executed

};

#endif