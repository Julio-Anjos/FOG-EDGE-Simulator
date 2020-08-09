#ifndef MSQ_HOST_H
#define MSQ_HOST_H
#include "burst_conf.h"
#include <fstream>
using namespace std; 

//This class helps to combine all the information related to how each of the communication between actors and sensors went
//The actors inform the msq_host when they started and ended sending packages, and how many were correctly send, the host class compiles the
//information and displays it
class Msq_host
{
    private:
    
        string host_name;
        vector<string> sensor_list; //list of the sensors each actor is connected to, every actor has an id that correspond to a sensor on this list
        

        vector<interval> intervals; //Msq to sensor burst intervals
        int num_intervals;
        
        string equation;
        vector<int>  started_intervals; //How many actors that have already started each interval
        vector<int>  total_sent_packages; //Total amount of packages sucessfully sent between sensor and the msq_node on each interval
        vector<int>  finished_intervals; //Contains the number of actors that have finished each interval
        vector<int>  stream_pkg_amount;
        int sent_packages;



        int window;
        int buffer;
        float timeout;

        int num_actors=0;//Number of actors that act on this host
        int num_completed_actors=0;//Number of sensors that completed all the bursts, used on inform_all_bursts_end

        //Stream_buffer *streaming_buffer; //Buffer that controls when data will be send to be processed, not currently used


    public:

        Msq_host(); //Constructor
        Msq_host(string name, string burst_config_id); //Constructor
        void add_sensor(string sensor);
        void add_info(int window_size, int buffer_size, float stream_timeout, string process_equation);
        int get_sensor_list_size();
        vector<interval> get_intervals();
        int get_window();
        int get_buffer();
        float get_timeout();
        string get_equation();
        int get_sent_packages();
        void set_stream_pkg(int pkg_amount);
        vector<int> get_stream_pkg();
        void inform_burst_start(int current_burst_id,double time);//lets an actor inform it started a burst, then the host displays information about it
        void inform_burst_result(int current_burst_id, int sent_packages, double time);//lets an actor inform the host of the results of a burst, then the host displays information about it
        void inform_all_bursts_end();//lets an actor inform it has ended all bursts, then the host displays information about it
        
};
extern map <string,Msq_host> msq_host_map;
#endif

