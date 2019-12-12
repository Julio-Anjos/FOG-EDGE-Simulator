#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "sensor.h"
#include "msq_node.h"
using namespace std; 

XBT_LOG_NEW_DEFAULT_CATEGORY(sample_simulator, "Messages specific for this simulator");

int main(int argc, char* argv[])
{
    //Starting engine and confirming number of arguments
    simgrid::s4u::Engine e(&argc, argv);
    xbt_assert(argc > 2, "Usage: %s platform_file deployment_file\n", argv[0]);

    
    //Register sensors and msq_nodes
    e.register_actor<Msq_node>("msq_node");
    e.register_actor<Sensor>("sensor");

    //Loading platforms
    e.load_platform(argv[1]);
    e.load_deployment(argv[2]);

    //Start of simulation
    e.run();

    //End of simulation
    XBT_INFO("Total simulation time: %.3f", e.get_clock());


    return 0;
}
