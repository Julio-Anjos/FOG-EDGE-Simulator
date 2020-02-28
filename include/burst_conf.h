#ifndef BURST_CONF_H
#define BURST_CONF_H


using namespace std; 


typedef struct burst_interval{
    
    float end_time;
    int num_packages,package_size;

    string math_function;       //Math function that defines how the sensor will behave during this interval
    float math_start, math_end; //Start and end of interval in which we will consider the math function

    
    vector<int> package_amounts;  //Amount of packages that will be sent during the interval for each interval division (divided in steps)
    float step;   //Example: start_time = 0, end_time = 2, step = 0.5, and packages_amount=[10,20,10,5],
                  // Means that: 0 - 0.5 : 10 packages sent, 0.5 - 1 : 20 packages sent etc...                       

} interval;


//This class reads the config file. An instance of this class
// can be accessed by the sensors globally so they know what is the burst configuration.
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
        void calculate_send_times();
        
};

extern Burst_conf burst_config;

#endif