#ifndef BURST_CONF_H
#define BURST_CONF_H


using namespace std; 


typedef struct burst_interval{
    float start_time, end_time;
    int num_packages,package_size;
} interval;


//This class reads the config file. An instance of this class
// can be accessed by the sensors so they know what is the bursting configuration.
class Burst_conf
{
    
    private:
    //Vars
        string file_path;
        //Burst configuration to a vector of intervals map
        map <string,vector<interval>> interval_map;

    //Methods
        void parse_file();
    public:
        void initialize(string file_path);
        vector<interval> get_intervals(string burst_config);
    
        
};

extern Burst_conf burst_config;

#endif