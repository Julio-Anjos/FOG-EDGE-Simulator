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
    float last_end_time = 0;
    int num_packages = 0;
    vector<interval> burst_intervals = burst_config.get_intervals(burst_config_id);
    //Iterate over the intervals
    for(interval burst : burst_intervals ){

        //Divide the packages between the sensors
        num_packages = burst.num_packages/num_concurrent_sensors;   
        
        start_burst(burst.end_time - last_end_time,burst.end_time, num_packages ,burst.package_size);
        last_end_time = burst.end_time;
    }

}



void Sensor::start_burst(float duration,float end_time, int num_packages, int package_size )
{
    
    double* current_time = new double();
    double* start_time = new double();
    *start_time = simgrid::s4u::Engine::get_clock();
    int *stop_flag = new int(0);
    int *continue_flag = new int(1);
    int counter = 0;

    double spacing = duration/num_packages;
    
    
    double transference_time = 0;  //Time it takes for a single message to be transfered
    bool first_msg_flag = true;
    do{
        

        *current_time = simgrid::s4u::Engine::get_clock();

        msq_mailbox->put(continue_flag,package_size);
        counter++;

    
        //cout << host_name << " finished interval " <<  end_time << ":" << num_packages <<"x" << package_size << " at the time: " << *current_time << endl;
        simgrid::s4u::this_actor::sleep_until(*current_time + spacing);
        
    }
    while(counter < num_packages or end_time < *current_time );
    
    cout << host_name << " finished burst " <<  end_time << ":" << num_packages <<"x" << package_size << " at the time: " << *current_time << endl;
    
    if(counter < num_packages ){
        cout << "MISSED " << num_packages-counter << " PACKAGES" << endl;
    }

    msq_mailbox->put(stop_flag,0);
    *current_time = simgrid::s4u::Engine::get_clock();  
    
    return;
}



