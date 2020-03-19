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
        vector<string> sensor_list;
    
    public:
        Msq_host(); //Constructor
        Msq_host(string name); //Constructor
        void add_sensor(string sensor);
        void test();
        
    
        
};
extern map <string,Msq_host> msq_host_map;
#endif

