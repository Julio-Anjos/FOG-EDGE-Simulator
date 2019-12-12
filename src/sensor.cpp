#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "sensor.h"
using namespace std; 


Sensor::Sensor(vector<string> args)
{
    //Testing arguments (localized on the deploy platform file)
    xbt_assert(args.size() > 1, "One argument needed.");
    test_parameter = stoi(args[1]);
    
    //Getting host variables
    host = simgrid::s4u::this_actor::get_host();
    host_name = host->get_name();


    cout << host_name << " Constructor Executed."  << endl;
    
}


//This is the function that will first run when the platform executes
void Sensor::operator()(void)
{
    cout << host_name << " Operator Executed."  << endl;
}





