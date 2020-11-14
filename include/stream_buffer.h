#ifndef STREAM_BUFFER_H
#define STREAM_BUFFER_H
#include "burst_conf.h"
#include "msq_host.h"
#include <fstream>
using namespace std; 

//THIS CLASS IS CURRENTLY ON A TEMPLATE STAGE, NOT BEING USED

//Class used my msq_nodes. It acts as the streaming buffer for the node. Indicating
//when window_sized byte packages can be processed, or when the transmission must stop due to
//the buffer being full.
typedef struct sensor_node_info{
    string sensor_name;
    string msq_name;
    int burst_counter;
    bool end_burst;


} sensor_node;


class Stream_master{
    private:
        int num_buffers;

        sensor_node aux_stream_node;
        vector<simgrid::s4u::Mailbox*> mailbox_in; //This node has one mailbox for each stream_buffer to receive their info
        vector<simgrid::s4u::Mailbox*> mailbox_out; //This node has one mailbox for each msq_node to send their info
        vector <sensor_node> stream_nodes;
        vector<interval> intervals;
        string line;
        vector<simgrid::s4u::CommPtr> stream_comms;
        simgrid::s4u::SemaphorePtr sem_full; /* indicates whether the buffer is full */
        int finished_pairs;
        vector<bool> end_flags;
        Msq_host *host; //Host class that manages this actor

    public:
        Stream_master(vector <string> args);
        void operator()(void);
        void receive_packages(int pos_pair);
};
class Stream_buffer
{


    private:

        int window_size; //Size in bytes, when this window fills up, the data will be send to be processed, unless warned by the processing unit that it cant. 
        int buffer_size; //Size in bytes, the total buffer size, if the buffer fills up without the data being processed, the sensors are warned to stop sending data.
        float stream_timeout;  //Amount of time in seconds before the data in the buffer is send to be processed even if the window size hasn't completly filled up.
        vector<interval> intervals;
        
        int lost_bytes; // Amount of bytes that are lost because the buffer is full
        int current_amount; //Current amount of bytes in the buffer
        int aux_amount; // Amount used when timeout and bytes are below window size
        float last_time;; //Last time a window was send for processing
        int total_received;
    	int total_lost;
        int process_amount;
        int count_pkgs;

        sensor_node aux_sensor_node;

        
        simgrid::s4u::CommPtr comm;
        vector<simgrid::s4u::Mailbox*> mailbox_in; //This node has one mailbox for each sensor to receive their info
        simgrid::s4u::Mailbox* mailbox_out; //This node has one mailbox for each msq_actor to send their info
        Msq_host *host; //Host class that manages this actor

        int num_pairs;
        string line;
        string host_name; //Name of the current actor's host
        simgrid::s4u::Host * stream_host;
        simgrid::s4u::ActorPtr stream_actor;
        vector<simgrid::s4u::ActorPtr> actors;

        vector <sensor_node> sensor_nodes;

    public:
        Stream_buffer(vector<string> args);
        vector<simgrid::s4u::CommPtr> stream_comms;
        void operator()(void);
        int add(int bytes, float current_time, bool end_early); //Adds a new quantity of bytes to the buffer and test the basic conditions if the node must attempt to send a window to be processed
        float get_loss_ratio();
        void receive_packages(int pos_pair);
};
extern vector<simgrid::s4u::CommPtr> stream_comms;
extern int finished_buffers;
#endif