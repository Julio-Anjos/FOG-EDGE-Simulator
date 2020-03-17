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

void Msq_host::add_sensor(string sensor){

    this->sensor_list.push_back(sensor);
}

//Test
void Msq_host::test(){


    for(string sensor : sensor_list){
        cout << host_name <<" has sensor " << sensor << endl;
    }  
}

