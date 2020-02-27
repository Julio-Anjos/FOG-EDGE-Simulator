#ifndef BURST_CONF_H
#define BURST_CONF_H


using namespace std; 


typedef struct burst_interval{
    
    float end_time;
    int num_packages,package_size;

    string math_function;   //Math function that defines how the sensor will behave during this interval
    float math_start, math_end; //Start and end of interval in which we will consider the math function
} interval;


//This class reads the config file. An instance of this class
// can be accessed by the sensors so they know what is the bursting configuration.
class Burst_conf
{
    
    private:
    //Vars
        string file_path;
        //Burst configuration to a vector of intervals map
        //<burst_config_id,intervals>
        map <string,vector<interval>> interval_map;

    //Methods
        void parse_file();
    public:
        void initialize(string file_path);
        vector<interval> get_intervals(string burst_config);
    
        
};

extern Burst_conf burst_config;

#endif