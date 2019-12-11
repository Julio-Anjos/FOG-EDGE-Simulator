#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "sensor.h"
using namespace std; 

//Constructor
Sensor::Sensor(simgrid::s4u::Host* host){

    this->host = host;
    this->name = host->get_name();
}

void Sensor::something(){
    cout << this->name << endl;
}




