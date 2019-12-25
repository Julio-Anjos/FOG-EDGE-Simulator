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
    xbt_assert(args.size() > 1, "Burst config id needed for each msq_node");

    burst_config_id = args[1];
    num_bursts = burst_config.get_intervals(burst_config_id).size(); //Number of intervals in the burst_config


    //Getting host variables
    host = simgrid::s4u::this_actor::get_host();
    host_name = host->get_name();
    
    string receive_mailbox_name;
    //Getting the sensor names and mailboxes


    //Creating mailbox vectors
    for(int i =2; i< args.size(); i++){
        sensor_mailboxes.push_back(simgrid::s4u::Mailbox::by_name(args[i]));
        receive_mailboxes.push_back(simgrid::s4u::Mailbox::by_name(host_name + "_" + args[i]));  //This node has one mailbox for each sensor to receive their info
    }

    num_sensors =  sensor_mailboxes.size();
    
  
    

}


//This is the function that will first run when the platform executes
void Msq_node::operator()(void)
{
    
    //Send starting information for the sensors
    for(int i =0;i<num_sensors ;i++){
        sensor_mailboxes[i]->put(&burst_config_id,0);
        sensor_mailboxes[i]->put(&host_name,0);
        sensor_mailboxes[i]->put(&num_sensors,0);
    }
    

    //Receive the bursts from the sensors
    
    receive();
   
        
    
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
        for(int i=0;i<num_sensors;i++){
            flag = static_cast<int*>(receive_mailboxes[i]->get());
            if(*flag == 0){
                complete_bursts++;
            }
        }
       
        //Getting current time
        current_time = simgrid::s4u::Engine::get_clock();  
    
    }
    while(complete_bursts < total_bursts); //Check if all sensors of this node have ended
    
    cout << host_name << " completed all " << complete_bursts << " bursts." << endl;
    
    
    

}