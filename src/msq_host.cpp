#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "msq_host.h"


using namespace std; 

//Constructor
Msq_host::Msq_host(string name){

    host_name = name;
}
extern map <string,Msq_host> msq_hosts;