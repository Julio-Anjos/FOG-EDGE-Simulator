#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "sensor.h"
using namespace std; 


Sensor::Sensor(vector<string> args)
{
    xbt_assert(args.size() > 1, "One argument needed.");
    test_parameter = stoi(args[1]);
    
    cout << "Constructor Executed: " << test_parameter << endl;
    
}


//This is the function that will first run when the platform executes
void Sensor::operator()(void)
{
    cout << "Operator Executed: " << test_parameter << endl;
}





