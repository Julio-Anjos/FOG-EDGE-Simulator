#include <simgrid/s4u.hpp>
#include <iostream>
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


    simgrid::s4u::Link *this_link = simgrid::s4u::Link::by_name(host_name + "_" + connected_msq_node);
    cout << this_link->get_bandwidth() << endl;

    

}

//This is the function that will first run when the platform executes
void Sensor::operator()(void)
{
    
    //burst(450,100,200);
    cout << host_name << " Operator Executed."  << endl;
}

void Sensor::burst(int num_packages, long int size , double end_time)
{
    
    return;
    /*
    
    simgrid::s4u::Mailbox* msq_mailbox = simgrid::s4u::Mailbox::by_name("Msq_node-0");
    
    int counter =0 ;
    int *payload_continue = new int(1);
    int *payload_stop = new int(0);
    double current_time;

    do{
        current_time = simgrid::s4u::Engine::get_clock();
        msq_mailbox->put(payload_continue, size);
        counter++;
    }
    while(current_time < end_time && counter < num_packages);
    
    msq_mailbox->put(payload_stop, 0);
    */
}



