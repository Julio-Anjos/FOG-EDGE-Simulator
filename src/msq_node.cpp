#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "msq_node.h"


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
    intervals =  burst_config.get_intervals(burst_config_id);
    num_intervals = intervals.size(); //numbers of intervals
    
    //Streaming arguments
    int window_size = stoi(args[2]);
    int buffer_size = stoi(args[3]);
    float stream_timeout = stof(args[4]);

    //Currently not being used
    streaming_buffer = new Stream_buffer(window_size,buffer_size,stream_timeout); 

   
    //Getting mailboxes for each sensors (Arguments from 5.. are sensor names)
    for(int i =5; i< args.size(); i++){
        sensor_mailboxes.push_back(simgrid::s4u::Mailbox::by_name(args[i]));
        receive_mailboxes.push_back(simgrid::s4u::Mailbox::by_name(host_name + "_" + args[i]));  //This node has one mailbox for each sensor to receive their info
    }

    num_sensors =  sensor_mailboxes.size();

}


//The operator is the function that will run automatically as the platform starting executing
void Msq_node::operator()(void)
{
   
    //Send starting information to the sensors
    for(int i =0;i<num_sensors ;i++){
        sensor_mailboxes[i]->put(&host_name,0);
        sensor_mailboxes[i]->put(&(intervals),0);
        sensor_mailboxes[i]->put(&(num_sensors),0);
    }

    
    vector<int> packages;
    //Receive the packages from the sensors    
    for(interval inter : intervals ){
        
        packages = inter.package_amounts;
        
        cout << "------------------------------ " << host_name << " START NEW BURST AT " <<  simgrid::s4u::Engine::get_clock()  << "--------------------------------------------" << endl;
    
        //Receive the packages thata are being sent from the sensors
        for(int i =0;i<packages.size();i++){
            receive_packages();
        }
        cout << "------------------------------ " << host_name << " FINISHED BURST AT " <<  simgrid::s4u::Engine::get_clock()  << "---------------------------------------------" << endl;
    
    
    }
    cout << host_name << " completed all " << num_intervals << " bursts." << endl;
    
    
}

//Keeps receiving data untill told to stop by the sensor
//receives a full burst from each sensor
void Msq_node::receive_packages()
{
    
    double* current_time = new double();
    
    
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
                
                *current_time = simgrid::s4u::Engine::get_clock();
                
                
                if(*payload == -1){  //Update the flag vector in case the payload is -1 (burst ended)
                    complete_bursts++;
                    flags[i] = 0;
                }
                
                
                //Update the buffer with the new amount of data, currently incomplete
                //update_buffer(*payload,*current_time);
            }     
        }
    }
    while(complete_bursts < num_sensors); //Check if all sensors of this node have ended
    
}

void Msq_node::update_buffer(int num_bytes, double current_time){
   
    string buffer_command;
    string exec_flag;
    buffer_command = streaming_buffer->add(num_bytes, current_time); //Add the receiving payload to the buffer
    //TODO
    //This is the last step for the simulator completion.
    //The streaming_buffer->add returns a buffer_command that explains if the data must be send for processing, and the  way of this processing
    //will be defined by the user. There must be communication between the buffer and the processing of data to know if more data 
    //can be sent for processing. If the buffer fills up without being able send the data for processing, that data will be lost.
}