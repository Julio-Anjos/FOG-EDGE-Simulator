#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "msq_node.h"
#include "stream_buffer.h"

using namespace std; 

//Constructor
Msq_node::Msq_node(vector<string> args)
{
    
    //Getting host variables
    host = simgrid::s4u::this_actor::get_host();
    host_name = host->get_name();
    
    
    //Testing arguments (localized on the deploy platform file)
    xbt_assert(args.size() > 1, "Burst config id needed for each msq_node");

    //Burst config arguments
    burst_config_id = args[1];
    bursts =  burst_config.get_intervals(burst_config_id);
    num_bursts = bursts.size(); //numbers of intervals
    
    //Streaming arguments
    window_size = stoi(args[2]);
    buffer_size = stoi(args[3]);
    stream_timeout = stof(args[4]);

   
    //Getting mailboxes for each sensors (Arguments from 5.. are sensor names)
    for(int i =5; i< args.size(); i++){
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
            if(rest > 0){   //Basically divide the packages equally, then grabs the rest of the division
                            //and add to the sensors untill the rest ends.
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

    // payload = -1 MEANS THE COMMUNICATION MUST STOP
    // Otherwise, payload is the number of bytes that were transmitted
    int *payload;  
    // When a payload = -1 is received, the flags vector is updated on that sensor, indicating that the current burst ended
    int flags[num_sensors];   
    for(int i=0;i<num_sensors;i++)//Initialize with 1 (communication burst still running)
        flags[i] = 1;
    

    do{
        //Receive a payload
        for(int i=0;i<num_sensors;i++){
            
            if(flags[i]){ //Test if this communication hasn't ended
                payload = static_cast<int*>(receive_mailboxes[i]->get()); //Receive data from sensor

                if(*payload == -1){  //Update the flag vector in case the payload is -1 (burst ended)
                    complete_bursts++;
                    flags[i] = 0;
                }
            }     
        }
    }
    while(complete_bursts < num_sensors); //Check if all sensors of this node have ended
    

    
    

}