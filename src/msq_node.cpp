#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "msq_node.h"
using namespace std; 

//Constructor
Msq_node::Msq_node(vector<string> args)
{
    //Testing arguments (localized on the deploy platform file)
    //xbt_assert(args.size() > 1, "One argument needed.");
    //test_parameter = stoi(args[1]);
    
    //Getting host variables
    host = simgrid::s4u::this_actor::get_host();
    host_name = host->get_name();
    mailbox = simgrid::s4u::Mailbox::by_name(host_name);
    


    cout << host_name <<" Constructor Executed."<< endl;

}


//This is the function that will first run when the platform executes
void Msq_node::operator()(void)
{
    
    //receive();
    cout << host_name << " Operator Executed." << endl;
}

//Keeps receiving data untill told to stop
void Msq_node::receive()
{
    
    double current_time;
    int *payload_test;
    do{

        payload_test = static_cast<int*>(mailbox->get());
        current_time = simgrid::s4u::Engine::get_clock();
        cout << "Receive at: " << current_time << endl;

    }
    while(*payload_test == 1); // Payload will be 0 when it needs to stop
    
    
    

}