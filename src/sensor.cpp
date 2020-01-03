#include <simgrid/s4u.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "burst_conf.h"
#include "sensor.h"


using namespace std; 



Sensor::Sensor(vector<string> args)
{
    
    //Setting host variables
    host = simgrid::s4u::this_actor::get_host();
    host_name = host->get_name();

    //Target msq_node mailbox
    mailbox = simgrid::s4u::Mailbox::by_name(host_name);

    //Receive some information from the msq_node
    get_msq_information();

}

 //Receiving starting information from the msq_node
void Sensor::get_msq_information(){

    //Get the msq node for creating the mailbox
    string *temp_payload1 =  static_cast<string*>(mailbox->get());
    connected_msq_node = *temp_payload1;
    msq_mailbox = simgrid::s4u::Mailbox::by_name(connected_msq_node+ "_" +host_name);

    //Gets the bursts
    vector<interval> *temp_payload2  = static_cast<vector<interval>*>(mailbox->get());
    bursts = *temp_payload2;
}



//This is the function that will first run when the platform executes
void Sensor::operator()(void)
{
    float last_end_time = 0;
 
    
    //Iterate over the intervals
    for(interval burst : bursts ){
        
        start_burst(burst.end_time - last_end_time,burst.end_time, burst.num_packages ,burst.package_size);
        last_end_time = burst.end_time;
    }

}



void Sensor::start_burst(float duration,float end_time, int num_packages, int package_size )
{
    
    double* current_time = new double();
    double* start_time = new double();
  

    *start_time = simgrid::s4u::Engine::get_clock();
    int *stop_flag = new int(0);
    int *continue_flag = new int(1);
    int counter = 0;

    double spacing = duration/num_packages;
    
   
    do{
        
        *current_time = simgrid::s4u::Engine::get_clock();
        //simgrid::s4u::this_actor::sleep_until(counter*spacing + *start_time);


        //Receive the message
        msq_mailbox->put(continue_flag,package_size);
        

    
        counter++;
        //Wait to receive new one
        
    }
    while(counter < num_packages &&  *current_time < end_time );
    cout << host_name << " finished burst " <<  end_time << ":" << num_packages <<"x" << package_size << " at the time: " << *current_time << endl;
    

    //Checking for missing packages
    if(counter < num_packages ){
        cout << host_name << " COULD NOT SEND " << num_packages-counter << " PACKAGES IN TIME." << endl;
    }

    //Send to the msq_node that the transmission has ended
    msq_mailbox->put(stop_flag,0);
   
    return;
}


