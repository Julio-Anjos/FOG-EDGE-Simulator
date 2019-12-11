#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "msq_node.h"
using namespace std; 

//Constructor
Msq_node::Msq_node(simgrid::s4u::Host* host){

    this->host = host;
    this->name = host->get_name();
    this->display_loaded();
}

void Msq_node::display_loaded(){
    cout << "Host " << this->name << " has loaded successfully." << endl;
}


