#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "msq_host.h"


using namespace std; 



Msq_host::Msq_host(){
}

//Constructor
Msq_host::Msq_host(string name, string burst_config_id){

    host_name = name;
    intervals = burst_config.get_intervals(burst_config_id);
}

//Sensor list functions
void Msq_host::add_sensor(string sensor){
    this->sensor_list.push_back(sensor);
}

int Msq_host::get_sensor_list_size(){
    return sensor_list.size();
}

//Msq_actors use this function the inform the results of it's bursts
//the Msq_host then collects the info and displays it.

void Msq_host::inform_burst_start(int actor_id,int current_burst_id, double time){
    //cout << connected_sensor_name << "_" << host_name << " started  burst "<<burst_counter <<" at " <<  simgrid::s4u::Engine::get_clock()  << endl;
    return;
}


void Msq_host::inform_burst_result(int actor_id,int current_burst_id, int sent_packages, double time){
    
    /*
    //Print information about missed packages
        if(interval_sent_packages < inter.num_packages){
            cout  << connected_sensor_name <<"_" << host_name << " finished burst "<< burst_counter <<" at " <<  simgrid::s4u::Engine::get_clock()<<" FAILED TO SEND " << inter.num_packages - interval_sent_packages << " packages out of " << inter.num_packages << endl;;
        }
        else{
            cout << connected_sensor_name << "_" << host_name << " finished burst "<< burst_counter <<" at " <<  simgrid::s4u::Engine::get_clock()<< " sucessfully sending all " <<  inter.num_packages <<" packages " << endl;
        }
    */
    return;
}

//Actors use this function to inform the end of all bursts
void Msq_host::inform_all_bursts_end(){
    
    this->num_completed_actors += 1;
    int num_actors = this->sensor_list.size();
    
    if(num_completed_actors == num_actors  ) //If every actor completed it's bursts, the host has done its job receiving all
        cout << host_name << " completed all " << intervals.size() << " bursts." << endl;
    
}