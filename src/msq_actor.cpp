#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "shunting-yard.h"
#include "msq_actor.h"
#include "msq_host.h"
#include "sensor.h"


using namespace std; 

//Constructor
Msq_actor::Msq_actor(vector<string> args)
{
    //Testing arguments (localized on the deploy platform file)
    xbt_assert(args.size() > 7,"Msq_actor missing arguments.");

    //Getting host variables
    host_name = simgrid::s4u::this_actor::get_host()->get_name();
    
    //Burst config arguments
    burst_config_id = args[1];
    process_amount = -1;
    intervals =  burst_config.get_intervals(burst_config_id);
    num_intervals = intervals.size(); //numbers of intervals
    

    //This actor will receive messages from a single sensor
    num_sensors = stoi(args[2]);


    receive_mailbox = simgrid::s4u::Mailbox::by_name( "StreamMaster_" + host_name);  
   
    
    
    //Streaming arguments, currently not being used (these are used when creating the msq_host class)
    int window_size = stoi(args[3]);
    int buffer_size = stoi(args[4]);
    float stream_timeout = stof(args[5]);

    string process_equation = args[6];

    for(int i = 7; i < num_sensors+7; i++)
        connected_sensors.push_back(args[i]);

    //Creating sensor buffer(currently not used)
    //streaming_buffer = new Stream_buffer(window_size,buffer_size,stream_timeout, process_equation); 

    //Getting msq_host that this actor acts on, he is used to store compile info on all actors
    host = fetch_host(host_name,burst_config_id,window_size,buffer_size,stream_timeout, process_equation); //gets the host that will manage this actor
}


Msq_host* Msq_actor::fetch_host(string host_name, string burst_config_id, int window, int buffer, float timeout, string equation){


    //WAS THE HOST ALREADY CREATED?
    if ( msq_host_map.count(host_name)) {
    //YES, HOST FOUND
    //Add this sensor to the list of sensors that the following host manages
        for(int i = 0; i < num_sensors; i++)
        msq_host_map[host_name].add_sensor(connected_sensors);

    } 
    else {
    //NO, HOST NOT FOUND
        //Creates the host and add this sensor
        Msq_host new_host(host_name,burst_config_id);
        new_host.add_sensor(connected_sensors);
        new_host.add_info( window,buffer,timeout, equation);
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
        host->inform_burst_result(burst_counter, interval_sent_packages, simgrid::s4u::Engine::get_clock());

    }
    //cout << "Loss ratio is " << streaming_buffer->get_loss_ratio() << "% for " << host_name << " connected to " << connected_sensor_name << endl;
    /*while (not pending_executions.empty()) {
        int pos;
        pos = simgrid::s4u::Exec::wait_any(&pending_executions);
    if (pos < 0) {
        pending_executions.clear();
    } else {
        pending_executions.erase(pending_executions.begin() + pos);
    }
    }*/
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
    int process_cost = 0;
    
    //TokenMap vars;  //Initialize constants

    //string process_function = host->get_equation();

    //calculator calc2(process_function.c_str());
    

    do{
        //Receive a payload
    
        payload = static_cast<int*>(receive_mailbox->get()); //Receive data from sensor
        //cout << *payload << " " << host_name << endl;
        *current_time = simgrid::s4u::Engine::get_clock();
        /*
        vars["x"] = *payload;

        if(*payload != -1){

            process_cost = calc2.eval(vars).asInt();
            //cout << process_cost << " cost vs " << *payload << endl;
            exec = simgrid::s4u::this_actor::exec_async(process_cost);
            pending_executions.push_back(exec);
        }*/

        
        //Update the buffer with the new amount of data, currently incomplete
        //update_buffer(*payload,*current_time);
                
    }
    while(*payload != -1); //Check if all sensors of this node have ended
    
}

