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

    //Gets the intervals
    vector<interval> *temp_payload2  = static_cast<vector<interval>*>(mailbox->get());
    intervals = *temp_payload2;
}



//This is the function that will first run when the platform executes
void Sensor::operator()(void)
{
    float last_end_time = 0;
    
    vector<int> packages;
    float step;
    int package_size;
    float start_time = 0;
    float end_time;

    //Iterate over the intervals, every interval is divided in small divisions of time, on each
    //division a certain amount of packages must be sent, this are defined by the user using mathematical functions
   
    for(interval inter : intervals ){
        
        package_size = inter.package_size;
        end_time = inter.end_time;
        packages = inter.package_amounts;
        step = inter.step;

        //Send the packages according to the previous defined division
        for(int i =0;i<packages.size();i++){
            send_packages(start_time+(step*i),packages[i], package_size);
        }


        start_time = inter.end_time;
        simgrid::s4u::this_actor::sleep_until(start_time);  //Making sure a new interval doesn't start untill it's start time
        
    }

}



void Sensor::send_packages(float end_time, int num_packages, int package_size)
{
    
    //Flag send when the communication must stop
    int *stop_flag = new int(-1);
   
   
    double* current_time = new double();
    double* start_time = new double();
  

    *start_time = simgrid::s4u::Engine::get_clock();
    int counter = 0;

    double duration = end_time - *start_time;
    double spacing = duration/num_packages;

    
    do{  
        *current_time = simgrid::s4u::Engine::get_clock();
        
        //Send the package
        msq_mailbox->put(&package_size,package_size);
       
        
        simgrid::s4u::this_actor::sleep_until(counter*spacing + *start_time);
    
        counter++;
        //Wait to receive new one
        
    }
    while(counter < num_packages &&  *current_time < end_time );
    cout << host_name << " finished  " <<  end_time << ":" << num_packages <<"x" << package_size << " at the time: " << *current_time << endl;
    

    //Checking for missing packages
    if(counter < num_packages ){
        cout << host_name << " COULD NOT SEND " << num_packages-counter << " PACKAGES IN TIME." << endl;
    }

    //Send to the msq_node that the transmission has ended
    msq_mailbox->put(stop_flag,0);
    
    return;
}


