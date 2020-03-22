#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "msq_host.h"


using namespace std; 



Msq_host::Msq_host(){
}

//Constructor
Msq_host::Msq_host(string name, string burst_config_id, int window_size, int buffer_size, float stream_timeout){

    host_name = name;
    intervals = burst_config.get_intervals(burst_config_id);
    num_intervals = intervals.size();

    //Creating vectors used on inform_burst_* functions
    vector<int> temp_vect1(num_intervals,0);
    vector<int> temp_vect2(num_intervals,0);
    vector<int> temp_vect3(num_intervals,0);

    started_intervals = temp_vect1;
    total_sent_packages= temp_vect2;
    finished_intervals = temp_vect3;

    //Creating sensor buffer(currently not used)
    streaming_buffer = new Stream_buffer(window_size,buffer_size,stream_timeout); 
    

}

//Sensor list functions
void Msq_host::add_sensor(string sensor){
    this->sensor_list.push_back(sensor);
    this->num_actors += 1;
}

int Msq_host::get_sensor_list_size(){
    return sensor_list.size();
}


//Display info about what intervals started, shows only when all the actors start
//Msq_actors use this function when they start a new interval
void Msq_host::inform_burst_start(int current_burst_id, double time){
    
    started_intervals[current_burst_id] += 1;
    //If the interval hasn't started yet, display that it started
    if(started_intervals[current_burst_id] == num_actors){
        
        cout <<  host_name << " started  burst "<< current_burst_id <<" at " <<  time << endl;
    }
    
}

//Msq_actors use this function the inform the results of it's bursts
//the Msq_host then collects the info and displays it.
void Msq_host::inform_burst_result(int current_burst_id, int sent_packages, double time){
    
    finished_intervals[current_burst_id] += 1;
    total_sent_packages[current_burst_id] += sent_packages;
    
    //if all the actors finished this interval
    if(finished_intervals[current_burst_id] == num_actors){
        int sucessfully_sent_packages = total_sent_packages[current_burst_id];
        int expected_packages = intervals[current_burst_id].num_packages;
        
        
        //Is this the correct way to do this? Or do we need to send packages to the buffer more often?
        //stream_buffer->add(time,sent_packages)


        //Print information about missed packages
        if(sucessfully_sent_packages < expected_packages){ 
            cout  << host_name << " finished burst "<< current_burst_id <<" at " <<  time <<" FAILED TO SEND " << expected_packages - sucessfully_sent_packages << " packages out of " << expected_packages << endl;;
        }
        else{
            cout <<  host_name << " finished burst "<< current_burst_id <<" at " <<  time << " sucessfully sending all " <<  expected_packages <<" packages " << endl;
        }
    }
   
}

//Actors use this function to inform the end of all bursts
void Msq_host::inform_all_bursts_end(){
    
    this->num_completed_actors += 1;
    
    
    if(num_completed_actors == num_actors  ) //If every actor completed it's bursts, the host has done its job receiving all
        cout << host_name << " completed all " << intervals.size() << " bursts." << endl;
    
}

