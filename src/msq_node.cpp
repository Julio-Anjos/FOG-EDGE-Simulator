#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "msq_node.h"
using namespace std; 

//Constructor
Msq_node::Msq_node(vector<string> args)
{
    //Testing arguments (localized on the deploy platform file)
    xbt_assert(args.size() > 1, "Burst config id needed for each msq_node");

    burst_config_id = args[1];
    bursts =  burst_config.get_intervals(burst_config_id);
    num_bursts = bursts.size(); //numbers of intervals


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
   
    //Initialize a burst vector for each sensor
    vector<vector<interval>> sensor_burst;
    for(int i =0;i<num_sensors ;i++){
        sensor_burst.push_back(bursts);
    }
    //Fix the burst vectors so that the packages are equally divided 
    //When there is some division rest, the first nodes receive extra packages
    int rest = 0;
    int sensor_id = 0;
    for(int i=0;i<num_bursts;i++){
        rest = bursts[i].num_packages % num_sensors;
        
        for(int j=0; j< num_sensors;j++){
            sensor_burst[j][i].num_packages = sensor_burst[j][i].num_packages/num_sensors;   
            if(rest > 0){
               sensor_burst[j][i].num_packages = sensor_burst[j][i].num_packages + 1;
               rest--; 
            }
            sensor_id++;
        }
    }

    //Send starting information to the sensors
    for(int i =0;i<num_sensors ;i++){
        sensor_mailboxes[i]->put(&host_name,0);
        sensor_mailboxes[i]->put(&(sensor_burst[i]),0);
    }

    //Receive the bursts from the sensors    
    for(int i=0; i< num_bursts;i++){
        receive_burst();
    }
    cout << host_name << " completed all " << num_bursts << " bursts." << endl;
    
    
}

//Keeps receiving data untill told to stop by the sensor
//receives a full burst from each sensor
void Msq_node::receive_burst()
{
    int complete_bursts=0;

    int flags[num_sensors];   //Determines if the sensor is still sending messages
    for(int i=0;i<num_sensors;i++)//Initialize with 1
        flags[i] = 1;

    int *flag;
    do{
        //Receive a payload
        for(int i=0;i<num_sensors;i++){
            
            if(flags[i]){
                flag = static_cast<int*>(receive_mailboxes[i]->get()); //Receive data from sensor
                
                if(*flag == 0){  //Update the flag vector
                    complete_bursts++;
                    flags[i] = 0;
                }
            }     
        }
    }
    while(complete_bursts < num_sensors); //Check if all sensors of this node have ended
    

    
    

}