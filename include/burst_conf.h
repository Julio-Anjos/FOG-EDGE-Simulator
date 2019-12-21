#ifndef BURST_CONF_H
#define BURST_CONF_H


using namespace std; 

//This class reads the config file. An instance of this class
// can be accessed by the sensors so they know what is the bursting configuration.

class Burst_conf
{
    private:
        string something;
    
    public:
    
    Burst_conf();
    void dosomething();
    void set_path(string x);
        
};

extern Burst_conf burst_config;

#endif