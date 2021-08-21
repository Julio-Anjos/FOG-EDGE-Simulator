#ifndef MSQ_ACTOR_PS_H
#define MSQ_ACTOR_PS_H

#include "burst_conf.h" // Precisa?
using namespace std;

class Msq_actor_ps
{
    private:
        string host_name;   //Name of the host that this actor is in
        string connected_msqNode_name; //Name of the msq_node this actor communicates with
        simgrid::s4u::Mailbox* receive_mailbox; //This node has one mailbox for each MsqNode to receive their info
        
        queue<int> buffer; //Buffer for received data from msq_nodes

    public:
        Msq_actor_ps(vector<string> args); //Constructor
        void operator()(void);
        void receive_packages();  //Receives packages from the msq_nodes

};

#endif