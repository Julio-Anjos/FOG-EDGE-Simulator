#include <simgrid/s4u.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "sensor.h"
#include "burst_conf.h"

using namespace std; 



Sensor::Sensor(vector<string> args)
{
    
    //Setting host variables
    host = simgrid::s4u::this_actor::get_host();
    host_name = host->get_name();

    //Target msq_node mailbox
    mailbox = simgrid::s4u::Mailbox::by_name(host_name);



    //Receiving starting information from the msq_node
    string *temp_payload1 =  static_cast<string*>(mailbox->get());
    string *temp_payload2 = static_cast<string*>(mailbox->get());
    int *temp_payload3 = static_cast<int*>(mailbox->get());
    
    burst_config_id = *temp_payload1;  
    connected_msq_node = *temp_payload2;
    num_concurrent_sensors = *temp_payload3;


    msq_mailbox = simgrid::s4u::Mailbox::by_name(connected_msq_node+ "_" +host_name);

}

//This is the function that will first run when the platform executes
void Sensor::operator()(void)
{
   
    int num_packages = 0;
    vector<interval> burst_intervals = burst_config.get_intervals(burst_config_id);
    //Iterate over the intervals
    for(interval burst : burst_intervals ){

        //Divide the packages between the sensors
        num_packages = burst.num_packages/num_concurrent_sensors;   
        
        start_burst(burst.end_time, num_packages ,burst.package_size);
      
    }

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



