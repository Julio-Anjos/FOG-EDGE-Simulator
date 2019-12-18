#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "sensor.h"

using namespace std; 



Sensor::Sensor(vector<string> args)
{
    //Testing arguments (localized on the deploy platform file)
    //xbt_assert(args.size() > 1, "One argument needed.");
    //test_parameter = stoi(args[1]);
    
    //Setting host variables
    host = simgrid::s4u::this_actor::get_host();
    host_name = host->get_name();
    
    burst_config.dosomething();
    cout << host_name << " Constructor Executed."  << endl;
    
}

//This is the function that will first run when the platform executes
void Sensor::operator()(void)
{
    
    //burst(450,100,200);
    cout << host_name << " Operator Executed."  << endl;
}

void Sensor::burst(int num_packages, long int size , double end_time)
{
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

}



