#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "sensor.h"
#include "msq_actor.h"
#include "burst_conf.h"
#include "msq_host.h"
using namespace std; 

XBT_LOG_NEW_DEFAULT_CATEGORY(sample_simulator, "Messages specific for this simulator");



map <string,Msq_host> msq_host_map; //declaring to be used globally, its extern on the msq_host.h file

int main(int argc, char* argv[])
{
   
    system("exec rm -r result_logs/*.txt");
    //Starting engine and confirming number of arguments
    simgrid::s4u::Engine e(&argc, argv);
    xbt_assert(argc > 3, "Usage: %s platform_file deployment_file burst_config_file\n", argv[0]);

    

    //Getting burst config path
    burst_config.initialize(argv[3]);
   

    
    //Register sensors and msq_actors
    e.register_actor<Msq_actor>("msq_actor");
    e.register_actor<Sensor>("sensor");
    //e.register_actor<Msq_actor_ps>("Msq_actor_ps");   //Currently not being used

    //Loading platforms
    e.load_platform(argv[1]);
    e.load_deployment(argv[2]);

    //Start of simulation
    e.run();

    //End of simulation
    XBT_INFO("\nTotal simulation time: %f", e.get_clock());

    
    
    return 0;
}

