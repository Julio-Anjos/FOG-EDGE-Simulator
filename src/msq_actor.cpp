#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "msq_actor.h"
#include "msq_host.h"


using namespace std; 

//Constructor
Msq_actor::Msq_actor(vector<string> args)
{
    //Testing arguments (localized on the deploy platform file)
    xbt_assert(args.size() > 5,"Msq_actor missing arguments.");

    //Getting host variables
    host_name = simgrid::s4u::this_actor::get_host()->get_name();
    
    //Burst config arguments
    burst_config_id = args[1];
    intervals =  burst_config.get_intervals(burst_config_id);
    num_intervals = intervals.size(); //numbers of intervals
    

    //This actor will receive messages from a single sensor
    connected_sensor_name = args[2];
    receive_mailbox = simgrid::s4u::Mailbox::by_name(host_name + "_" + connected_sensor_name);  
   
    
    
    //Streaming arguments, currently not being used (these are used when creating the msq_host class)
    int window_size = stoi(args[3]);
    int buffer_size = stoi(args[4]);
    float stream_timeout = stof(args[5]);
    
    


    //Getting msq_host that this actor acts on, he is used to store compile info on all actors
    host = fetch_host(); //gets the host that will manage this actor
}


Msq_host* Msq_actor::fetch_host(){


    //WAS THE HOST ALREADY CREATED?
    if ( msq_host_map.count(host_name)) {
    //YES, HOST FOUND
    //Add this sensor to the list of sensors that the following host manages
        msq_host_map[host_name].add_sensor(connected_sensor_name);
    } 
    else {
    //NO, HOST NOT FOUND
        //Creates the host and add this sensor
        Msq_host new_host(host_name,burst_config_id,window_size,buffer_size,stream_timeout);
        new_host.add_sensor(connected_sensor_name);
        msq_host_map.insert(make_pair(host_name, new_host));
        


    }
    actor_id = msq_host_map[host_name].get_sensor_list_size() -1 ;


    simgrid::s4u::this_actor::yield();  //this makes sure all actors have gotten to this part of the code before going foward
                                        
    return &msq_host_map[host_name];
}    


//The operator is the function that will run automatically as the platform starting executing
void Msq_actor::operator()(void)
{
    
    int interval_sent_packages=0;
    //Receive the packages from the sensor   
    for(int burst_counter=0;burst_counter<num_intervals;burst_counter++){
        host->inform_burst_start(burst_counter,simgrid::s4u::Engine::get_clock());
        
        
        int num_pkg_divisions = intervals[burst_counter].package_amounts.size(); //How many division each interval has
        //the divisions have different amount of packages that allow the interval do match a math function

        //Receive the packages that are being sent from the sensors
        for(int i =0;i<num_pkg_divisions;i++){
            receive_packages();
        }
        
        //For this specific interval, get the amount of packages correctly sent
        int *temp  = static_cast<int*>(receive_mailbox->get());
        interval_sent_packages =*temp;
        //cout << host_name << " sent " << interval_sent_packages << " at time " << simgrid::s4u::Engine::get_clock() << " count " << burst_counter << endl;
        host->inform_burst_result(burst_counter, interval_sent_packages, simgrid::s4u::Engine::get_clock());

    }
    host->inform_all_bursts_end();
}

//Keeps receiving data untill told to stop by the sensor
//receives a full burst from each sensor
void Msq_actor::receive_packages()
{
    
    double* current_time = new double();
    
    
    int complete_bursts=0;

    // payload = -1 MEANS THE COMMUNICATION MUST STOP
    // Otherwise, payload is the number of bytes that were transmitted
    int *payload;  
    // When a payload = -1 is received, iit ndicates that the current burst ended
    int counter = 0;
    
    

    do{
        //Receive a payload
    
        payload = static_cast<int*>(receive_mailbox->get()); //Receive data from sensor

        
        *current_time = simgrid::s4u::Engine::get_clock();

        counter ++;
        //Update the buffer with the new amount of data, currently incomplete
        //update_buffer(*payload,*current_time);
                
    }
    while(*payload != -1); //Check if all sensors of this node have ended
    counter --;

    cout << host_name << " received " << counter << " packages at " << *current_time << " seconds" << endl;
    
}

