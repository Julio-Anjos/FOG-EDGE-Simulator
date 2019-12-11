#include <simgrid/s4u.hpp>
#include <iostream>

XBT_LOG_NEW_DEFAULT_CATEGORY(sample_simulator, "Messages specific for this simulator");


int main(int argc, char* argv[])
{
  simgrid::s4u::Engine e(&argc, argv);


  //Loading platform file
  xbt_assert(argc==2, "Usage: %s platform_file.xml", argv[0]);
  e.load_platform(argv[1]);
   
  //Getting every host
  std::vector<simgrid::s4u::Host*> hosts_list = e.get_all_hosts();
  
  for(simgrid::s4u::Host* host : hosts_list){
      std::cout << host->get_cname() << std::endl;
  }



  //simgrid::s4u::Actor::create("pinger", simgrid::s4u::Host::by_name("host0"), pinger, mb1, mb2);
  //simgrid::s4u::Actor::create("ponger", simgrid::s4u::Host::by_name("host1"), ponger, mb2, mb1);

  e.run();

  XBT_INFO("Total simulation time: %.3f", e.get_clock());

  return 0;
}
