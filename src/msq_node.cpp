#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "msq_node.h"
#include "burst_conf.h"
using namespace std; 

//Constructor
Msq_node::Msq_node(vector<string> args)
{
    //Testing arguments (localized on the deploy platform file)
    xbt_assert(args.size() > 2, "Number of sensors and burst_config_id needed for each msq_node");

    string burst_config_id = args[1];
    num_bursts = burst_config.get_intervals(burst_config_id).size(); //Number of intervals in the burst_config

    num_sensors = stoi(args[2]);   


    //Getting host variables
    host = simgrid::s4u::this_actor::get_host();
    host_name = host->get_name();
    
    //Setting up mailbox
    mailbox =  simgrid::s4u::Mailbox::by_name(host_name);

}


//This is the function that will first run when the platform executes
void Msq_node::operator()(void)
{
    
    receive();
    //cout << host_name << " Operator Executed." << endl;
}

//Keeps receiving data untill told to stop
void Msq_node::receive()
{
    int complete_bursts=0;
    double current_time;
    

    int total_bursts = num_sensors*num_bursts;
    int *flag;
    do{
        //Receive a payload
        flag = static_cast<int*>(mailbox->get());
        
        
        /*
        //Getting current time
        current_time = simgrid::s4u::Engine::get_clock();  
        */

        //Saves the fact that a node has ended
        if(*flag == 0){
            complete_bursts++;
        }
        
    }
    while(complete_bursts < total_bursts); //Check if all sensors of this node have ended
    cout << host_name << " " << complete_bursts << endl;
    
    
    

}