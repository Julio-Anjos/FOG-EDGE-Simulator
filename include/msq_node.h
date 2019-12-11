#ifndef MSQ_NODE_H
#define MSQ_NODE_H
using namespace std; 
#include <simgrid/s4u.hpp>

class Msq_node
{
private:
    simgrid::s4u::Host* host;
    string name;
     
public:
   
    Msq_node(simgrid::s4u::Host* host);
 
    void display_loaded();
};
 
#endif