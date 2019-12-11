#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "sensor.h"
#include "msq_node.h"
using namespace std; 
XBT_LOG_NEW_DEFAULT_CATEGORY(sample_simulator, "Messages specific for this simulator");


int main(int argc, char* argv[])
{
    simgrid::s4u::Engine e(&argc, argv);


    //Loading platform file
    xbt_assert(argc==2, "Usage: %s platform_file.xml", argv[0]);
    e.load_platform(argv[1]);

    //Getting every host
    vector<simgrid::s4u::Host*> host_list = e.get_all_hosts();

    //Create sensor and Msq_node list
    vector<Sensor> sensor_list;
    vector<Msq_node> node_list;
    string host_name;

    //Separate between sensors and msq_nodes
    for(simgrid::s4u::Host* host : host_list){
        host_name = host->get_name();
        
        if(host_name.compare(0,6,"Sensor") == 0)
            sensor_list.push_back(Sensor(host));
        else 
           if(host_name.compare(0,8,"Msq_node") == 0)
               node_list.push_back(Msq_node(host));
    }
 


    //simgrid::s4u::Actor::create("pinger", simgrid::s4u::Host::by_name("host0"), pinger, mb1, mb2);
    //simgrid::s4u::Actor::create("ponger", simgrid::s4u::Host::by_name("host1"), ponger, mb2, mb1);

    e.run();

    XBT_INFO("Total simulation time: %.3f", e.get_clock());

    return 0;
}
