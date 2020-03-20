#ifndef MSQ_HOST_H
#define MSQ_HOST_H
#include "burst_conf.h"
#include "stream_buffer.h"
#include <fstream>
using namespace std; 

//This class manages the communication between all msq_actors on a specific host, it helps to combine all the information
//obtained on each actor.
class Msq_host
{
    private:
    
        string host_name;
        vector<interval> intervals; //Msq to sensor burst intervals
        vector<string> sensor_list; //list of the sensors each actor is connected to, every actor has an id that correspond to a sensor on this list
    
    public:
        Msq_host(); //Constructor
        Msq_host(string name, string burst_config_id); //Constructor
        void add_sensor(string sensor);
        int get_sensor_list_size();

        void inform_burst_start(int actor_id,int current_burst_id,double time);//lets an actor inform it started a burst
        void inform_burst_result(int actor_id,int current_burst_id, int sent_packages, double time);//lets an actor inform the host of the results of a burst
        void inform_all_bursts_end(int actor_id,double time);//lets an actor inform it has ended all bursts
        
};
extern map <string,Msq_host> msq_host_map;
#endif

