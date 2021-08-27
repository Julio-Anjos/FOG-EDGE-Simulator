#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "msq_actor_ps.h"
using namespace std;

Msq_actor_ps::Msq_actor_ps(vector<string> args)
{
    //Testing arguments (localized on the deploy platform file)
    xbt_assert(args.size() > 5, "Msq_actor_ps missing arguments.");

    //Getting host variables
    host_name = simgrid::s4u::this_actor::get_host()->get_name();

    //This actor will receive messages from a msq_node
    connected_msqNode_name = args[2];
    receive_mailbox = simgrid::s4u::Mailbox::by_name(host_name + "_" + connected_msqNode_name);
 

}


void Msq_actor_ps::operator()(void)
{

    //Teste comunicação
    int *payload;

    //Teste discos
    vector<simgrid::s4u::Disk*> const& disk_list = simgrid::s4u::Host::current()->get_disks();
	simgrid::s4u::Disk* disk = disk_list.front();

    payload = static_cast<int*>(receive_mailbox->get());

    disk->set_data(payload);

    payload = static_cast<int*>(disk->get_data());

    buffer.push(*payload);

    cout << endl << buffer.front() << endl;
}

void Msq_actor_ps::receive_packages()
{

    //payload = -1 means the communication ends
    //Otherwise, payload is the number of bytes that were transmitted
    int *payload;

    do{
        
        //Receive data from a msq_node
        payload = static_cast<int*>(receive_mailbox->get());


    }while(*payload != -1);


}


