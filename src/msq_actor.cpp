#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "msq_actor.h"
#include "msq_host.h"


using namespace std; 

//Constructor
Msq_actor::Msq_actor(vector<string> args)
{
    
    //Getting host variables
    host_name = simgrid::s4u::this_actor::get_host()->get_name();
    //host = create_or_fetch_host();
    
    //Testing arguments (localized on the deploy platform file)
    xbt_assert(args.size() > 5,"Msq_actor missing arguments.");

    //Burst config arguments
    burst_config_id = args[1];
    intervals =  burst_config.get_intervals(burst_config_id);
    num_intervals = intervals.size(); //numbers of intervals
    

    //This actor will receive messages from a single sensor
    connected_sensor_name = args[2];
    sensor_mailbox = simgrid::s4u::Mailbox::by_name(connected_sensor_name);
    receive_mailbox = simgrid::s4u::Mailbox::by_name(host_name + "_" + connected_sensor_name);  
   
    
    create_or_fetch_host();


    /*
    //Streaming arguments, currently not being used
    int window_size = stoi(args[3]);
    int buffer_size = stoi(args[4]);
    float stream_timeout = stof(args[5]);
    streaming_buffer = new Stream_buffer(window_size,buffer_size,stream_timeout); 
    */



    //Create logfile for the stream between msq and sensors
    sensor_stream_logfile.open ("result_logs/"+host_name+"_sensor_stream.txt",fstream::out | fstream::trunc );   
    sensor_stream_logfile.close();
}


//The operator is the function that will run automatically as the platform starting executing
void Msq_actor::operator()(void)
{
   

    int burst_counter=0;
    int interval_sent_packages;
    vector<int> packages;
    //Receive the packages from the sensor   
    for(interval inter : intervals ){
        burst_counter++;
        packages = inter.package_amounts;
        interval_sent_packages =0;
        
        cout << connected_sensor_name << "_" << host_name << " started  burst "<<burst_counter <<" at " <<  simgrid::s4u::Engine::get_clock()  << endl;
        //Receive the packages that are being sent from the sensors
        for(int i =0;i<packages.size();i++){
            receive_packages();
        }
        
        //For this specific interval, get the amount of packages correctly sent
        int *temp  = static_cast<int*>(receive_mailbox->get());
        interval_sent_packages =*temp;


        //Print information about missed packages
        if(interval_sent_packages < inter.num_packages){
            cout  << connected_sensor_name <<"_" << host_name << " finished burst "<< burst_counter <<" at " <<  simgrid::s4u::Engine::get_clock()<<" FAILED TO SEND " << inter.num_packages - interval_sent_packages << " packages out of " << inter.num_packages << endl;;
        }
        else{
            cout << connected_sensor_name << "_" << host_name << " finished burst "<< burst_counter <<" at " <<  simgrid::s4u::Engine::get_clock()<< " sucessfully sending all " <<  inter.num_packages <<" packages " << endl;
        }
       


    }
    cout << connected_sensor_name << "_" <<  host_name << " completed all " << num_intervals << " bursts." << endl;
    

    
    
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
    
    
    

    do{
        //Receive a payload
    
        payload = static_cast<int*>(receive_mailbox->get()); //Receive data from sensor
        
        *current_time = simgrid::s4u::Engine::get_clock();
        
        //Update the buffer with the new amount of data, currently incomplete
        //update_buffer(*payload,*current_time);
                
    }
    while(*payload != -1); //Check if all sensors of this node have ended
    
}

void Msq_actor::create_or_fetch_host(){

    cout << msq_host_map.count(host_name) << endl;
    
    
    
    if ( msq_host_map.count(host_name)) {
    // FOUND
        msq_host_map[host_name].add_sensor(connected_sensor_name);
    } 
    else {
    //NOT FOUND
        
        
        Msq_host new_host(host_name);
        
        new_host.add_sensor(connected_sensor_name);
        
        msq_host_map.insert(make_pair(host_name, new_host));
        


    }
    msq_host_map[host_name].test();
   
}   


/*
void Msq_actor::update_buffer(int num_bytes, double current_time){
   
    string buffer_command;
    string exec_flag;
    buffer_command = streaming_buffer->add(num_bytes, current_time); //Add the receiving payload to the buffer
    //TODO
    //This is the last step for the simulator completion.
    //The streaming_buffer->add returns a buffer_command that explains if the data must be send for processing, and the  way of this processing
    //will be defined by the user. There must be communication between the buffer and the processing of data to know if more data 
    //can be sent for processing. If the buffer fills up without being able send the data for processing, that data will be lost.
}
*/