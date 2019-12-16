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
    
    receive();
    cout << host_name << " Operator Executed." << endl;
}


void Msq_node::receive()
{
    int* payload = static_cast<int*>(mailbox->get());
    printf("%d\n",*payload);

    delete payload;

}