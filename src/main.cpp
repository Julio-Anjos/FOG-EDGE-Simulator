#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include "sensor.h"
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
  
  Sensor sensor(host_list[0]);
  sensor.something();


  /*
  for(simgrid::s4u::Host* host : host_list){
      
  }
  */


  //simgrid::s4u::Actor::create("pinger", simgrid::s4u::Host::by_name("host0"), pinger, mb1, mb2);
  //simgrid::s4u::Actor::create("ponger", simgrid::s4u::Host::by_name("host1"), ponger, mb2, mb1);

  e.run();

  XBT_INFO("Total simulation time: %.3f", e.get_clock());

  return 0;
}
