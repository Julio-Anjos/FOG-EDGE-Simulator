#include <simgrid/s4u.hpp>
#include <iostream>
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
    //Getting info used to divide packages
    int *temp_payload3  = static_cast<int*>(mailbox->get());
    num_sensors = *temp_payload3;
    int *temp_payload4  = static_cast<int*>(mailbox->get());
    sensors_position = *temp_payload4;


    //Open the log file
    logfile.open ("result_logs/"+connected_msq_node+"_sensor_stream.txt",fstream::app);
    
    

}



//Function to guaranteed that packages aren't lost due to doing a common division
//If you have n sensors:
//Sensors are ordered as 0,1,2,3... n-1
//When rest of the division by n can be 0,1,2,3... n-1
//So to make sure the packages are divided correctly, we are putting a package for every sensor
//if his position is between the 0 and the rest+1 
//Ex: 5 % 3 = 2, sensor 0 is not 0 < and <=2, but the other 2 are, division becomes: 1,2,2
//Sensor with relative position 0 never gets extra packages
int calculate_num_packages_divided(int num_packages, int num_sensors, int sensor_position){
    //gets the rest of division
    int rest = num_packages % num_sensors; 
    int num_packages_divided = num_packages / num_sensors;
    
    
    if(0 < sensor_position && sensor_position <= rest){
        num_packages_divided++;
    }


    return num_packages_divided;
}


//This is the function that will first run when the platform executes
void Sensor::operator()(void)
{
    float last_end_time = 0;
    
    vector<int> packages;
    float step;
    int package_size;
    int num_packages;
    int interval_sent_packages; //counts how many packages this sensor sent in one burst interval
    float start_time = 0;
    float end_time;

    //Iterate over the intervals, every interval is divided in small divisions of time, on each
    //division a certain amount of packages must be sent, this are defined by the user using mathematical functions
   
    for(interval inter : intervals ){
        interval_sent_packages =0;
        package_size = inter.package_size;
        end_time = inter.end_time;
        packages = inter.package_amounts;
        step = inter.step;
        

        if(sensors_position == 0){
            logfile << "-------------------------------------------------" << endl;
            logfile << "Starting burst: " << start_time << " - " << end_time;
            logfile << ": " << inter.num_packages << "x" << package_size << " f(x)=" << inter.math_function << endl;
            logfile << "-------------------------------------------------" << endl; 
        }

        //Send the packages according to the previous defined division
        for(int i =0;i<packages.size();i++){
            
            num_packages = calculate_num_packages_divided(packages[i],num_sensors, sensors_position);
            interval_sent_packages += send_packages(start_time+(step*(i+1)),num_packages, package_size);
            
        }
    
        //Checking for missing packages
        msq_mailbox->put(new int(interval_sent_packages),0); //Sending to the msq_node how sucessfull was the interval burst          
        

        start_time = inter.end_time;
        }

}



int Sensor::send_packages(float end_time, int num_packages, int package_size)
{
    
    //Flag send when the communication must stop
    int *stop_flag = new int(-1);
   
   
    double* current_time = new double();
    double* start_time = new double();
  

    *start_time = simgrid::s4u::Engine::get_clock();
    int counter = 0;

    double duration = end_time - *start_time;
    double spacing = duration/num_packages;

    logfile << host_name << " sending " <<  num_packages << " packages of size " << package_size << " from times " << *start_time << " - " << end_time << endl;
    

    //Main loop of sending packages
    while(counter < num_packages &&  *current_time < end_time ){
        
        *current_time = simgrid::s4u::Engine::get_clock();
        
        //Send the package
        msq_mailbox->put(&package_size,package_size);
       
        
        simgrid::s4u::this_actor::sleep_until(counter*spacing + *start_time);
    
        counter++;
        //Wait to receive new one
        
    }
    
    

    //Checking for missing packages in this division
    if(counter < num_packages ){
        logfile << host_name << " COULD NOT SEND " << num_packages-counter << " PACKAGES IN TIME." << endl;  
    }

    //Send to the msq_node that the transmission has ended
    msq_mailbox->put(stop_flag,0);

    simgrid::s4u::this_actor::sleep_until(end_time);  //Making sure a new interval doesn't start before the correct time
    
    
    
    
    

    return counter;
}

