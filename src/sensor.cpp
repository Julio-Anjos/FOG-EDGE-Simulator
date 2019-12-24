#include <simgrid/s4u.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "sensor.h"
#include "burst_conf.h"
using namespace std; 



Sensor::Sensor(vector<string> args)
{
    //Testing arguments (localized on the deploy platform file)
    xbt_assert(args.size() > 3, "Three arguments needed. Connected msq node id, burst config id and number of sensors in the same msq node connection");
    
    //Getting arguments
    connected_msq_node = args[1];
    burst_config_id = args[2];
    num_concurrent_sensors = stoi(args[3]);

    //Setting host variables
    host = simgrid::s4u::this_actor::get_host();
    host_name = host->get_name();

    //Target msq_node mailbox
    msq_mailbox = simgrid::s4u::Mailbox::by_name(connected_msq_node);

}

//This is the function that will first run when the platform executes
void Sensor::operator()(void)
{
    
    vector<interval> burst_intervals = burst_config.get_intervals(burst_config_id);
    
    float last_end_time = 0;
    //Iterate over the intervals
    for(interval burst : burst_intervals ){
        start_burst(burst.end_time, burst.num_packages,burst.package_size);
    }



    
    //cout << host_name << " Operator Executed."  << endl;
}



void Sensor::start_burst(float end_time, int num_packages, int package_size )
{
    
    double* current_time = new double();
    
    
    int *stop_flag = new int(0);
    int *continue_flag = new int(1);
    
    int counter = 0;
    do{

        *current_time = simgrid::s4u::Engine::get_clock();
        
        
        msq_mailbox->put(continue_flag,package_size);
        counter++;
    }
    while(counter < num_packages);
    
    cout << host_name << " finished burst " <<  end_time << ":" << num_packages <<"x" << package_size << " at the time: " << *current_time << endl;

    msq_mailbox->put(stop_flag,0);
    *current_time = simgrid::s4u::Engine::get_clock();  
    
    return;
}



