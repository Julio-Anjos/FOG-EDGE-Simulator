#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "sensor.h"
#include "msq_node.h"
#include "burst_conf.h"
using namespace std; 

XBT_LOG_NEW_DEFAULT_CATEGORY(sample_simulator, "Messages specific for this simulator");



#include "shunting-yard.h"

int main(int argc, char* argv[])
{
    TokenMap vars;
    vars["pi"] = 3.14;
    std::cout << calculator::calculate("-pi+1", &vars) << std::endl;

    // Or if you want to evaluate an expression
    // several times efficiently:
    calculator c1("pi-b");
    vars["b"] = 0.14;
    std::cout << c1.eval(vars) << std::endl; // 3
    vars["b"] = 2.14;
    std::cout << c1.eval(vars) << std::endl; // 1
    /*
    //Starting engine and confirming number of arguments
    simgrid::s4u::Engine e(&argc, argv);
    xbt_assert(argc > 3, "Usage: %s platform_file deployment_file burst_config_file\n", argv[0]);

    
    //Getting burst config path
    burst_config.initialize(argv[3]);
   
    //Register sensors and msq_nodes
    e.register_actor<Msq_node>("msq_node");
    e.register_actor<Sensor>("sensor");

    //Loading platforms
    e.load_platform(argv[1]);
    e.load_deployment(argv[2]);

    //Start of simulation
    e.run();

    //End of simulation
    XBT_INFO("\nTotal simulation time: %.3f", e.get_clock());


    */
    return 0;
}

