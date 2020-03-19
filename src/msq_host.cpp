#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "msq_host.h"


using namespace std; 



Msq_host::Msq_host(){
}

//Constructor
Msq_host::Msq_host(string name){

    host_name = name;
}

//Sensor list functions
void Msq_host::add_sensor(string sensor){
    this->sensor_list.push_back(sensor);
}

int Msq_host::get_sensor_list_size(){
    return sensor_list.size();
}

void Msq_host::inform_burst(int actor_id){
    cout << "Hey i'm the actor connected to: "<< sensor_list[actor_id] << endl;
}