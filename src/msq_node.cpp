#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "msq_node.h"
using namespace std; 

//Constructor
Msq_node::Msq_node(vector<string> args)
{
    xbt_assert(args.size() > 1, "One argument needed.");
    test_parameter = stoi(args[1]);
    
    host = simgrid::s4u::this_actor::get_host();
    host_name = host->get_name();
    
    cout << host_name <<" Constructor Executed."<< endl;

}


//This is the function that will first run when the platform executes
void Msq_node::operator()(void)
{
    cout << host_name << " Operator Executed." << endl;
}



