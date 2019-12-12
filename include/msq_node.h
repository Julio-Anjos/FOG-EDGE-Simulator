#ifndef MSQ_NODE_H
#define MSQ_NODE_H
#include <simgrid/s4u.hpp>

using namespace std; 

class Msq_node
{
    private:
        int test_parameter;
        simgrid::s4u::Host * host;
        string host_name;
    
    public:
        
        Msq_node(vector<string> args); //Constructor
        
        void operator()(void); //This is the function that will first run when the platform executes
};

#endif