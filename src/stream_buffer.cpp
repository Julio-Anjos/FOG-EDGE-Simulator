#include <simgrid/s4u.hpp>
#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>
#include "stream_buffer.h"
#include "shunting-yard.h"
#include "msq_host.h"
#include "msq_actor.h"
#include "sensor.h"

using namespace std; 

//THIS CLASS IS CURRENTLY ON A TEMPLATE STAGE, NOT BEING USED

//Class used my msq_nodes. It acts as the streaming buffer for the node. Indicating
//when window_sized byte packages can be processed, or when the transmission must stop due to
//the buffer being full.


Stream_buffer::Stream_buffer(vector<string> args){
    num_pairs = stoi(args[1]);
    xbt_assert(args.size() > num_pairs,"Stream_buffer missing arguments.");

    lost_bytes = 0;
    last_time = 0;
    current_amount = 0;
    aux_amount = 0;
    total_received = 0;
    total_lost = 0;
    size_t found;

    for(int i = 2; i<num_pairs+2;i++){
        line = args[i];
        found = line.find("_");
        aux_sensor_node.sensor_name = line.substr(0, found);
        aux_sensor_node.msq_name = line.substr(found+1, line.length());
        aux_sensor_node.burst_counter = 0;
        aux_sensor_node.end_burst = false;
        sensor_nodes.push_back(aux_sensor_node);
        mailbox_in.push_back(simgrid::s4u::Mailbox::by_name(aux_sensor_node.sensor_name + "_" + aux_sensor_node.msq_name));  
        mailbox_out.push_back(simgrid::s4u::Mailbox::by_name(aux_sensor_node.msq_name + "_" + aux_sensor_node.sensor_name));

    }

} 


void Stream_buffer::operator()(void){

    aux_sensor_node = sensor_nodes.back();


    host = msq_host_map[aux_sensor_node.msq_name];
    intervals = host.get_intervals();

    int final_interval = intervals.size();

    int pos_pair = 0;
    aux_sensor_node = sensor_nodes.at(pos_pair);

    int finished_pairs=0;

    while(finished_pairs < sensor_nodes.size()){

        host = msq_host_map[aux_sensor_node.msq_name];
        window_size = host.get_window();
        buffer_size = host.get_buffer();
        stream_timeout = host.get_timeout();
        intervals = host.get_intervals();
        if(aux_sensor_node.burst_counter < intervals.size()){

            int *stop_flag = new int(-1);
            double* current_time = new double();
            // payload = -1 MEANS THE COMMUNICATION MUST STOP
            // Otherwise, payload is the number of bytes that were transmitted
            // When a payload = -1 is received, it indicates that the current burst ended
            int *payload;  
            current_amount = 0;
            total_received = 0;
            lost_bytes = 0;
            total_lost = 0;
            aux_amount = 0;
            last_time = simgrid::s4u::Engine::get_clock();
            process_amount = 0;
            int num_pkg = 0;
            int interval_sent_packages = 0;
            int num_pkg_divisions = intervals[aux_sensor_node.burst_counter].package_amounts.size(); //How many division each interval has 
            
            for(int i =0;i<num_pkg_divisions;i++){
                do{
                
                    //Receive a payload
                    payload = static_cast<int*>(mailbox_in[pos_pair]->get()); //Receive data from sensor
                    
                    *current_time = simgrid::s4u::Engine::get_clock();

                    if(*payload != -1){
                        process_amount = add(*payload, *current_time);
                        if(process_amount != -1){
                            mailbox_out[pos_pair]->put(&process_amount, process_amount);
                        }
                    }
                }while(*payload != -1); //Check if all sensors of this node have ended
                if(process_amount != -1)
                    num_pkg++;
                mailbox_out[pos_pair]->put(stop_flag, 0);   
            }
            //host.set_stream_pkg(num_pkg);
            //For this specific interval, get the amount of packages correctly sent
            int *temp  = static_cast<int*>(mailbox_in[pos_pair]->get());
            interval_sent_packages =*temp;
            mailbox_out[pos_pair]->put(new int(interval_sent_packages), 0);

            sensor_nodes.at(pos_pair).burst_counter += 1;
        }
        else{
            if(!aux_sensor_node.end_burst){
                aux_sensor_node.end_burst = true;
                finished_pairs++;
            }
        }
        if(pos_pair < sensor_nodes.size()-1)
            pos_pair++;
        else
            pos_pair = 0;
        aux_sensor_node = sensor_nodes.at(pos_pair);

        
    }
    simgrid::s4u::Comm::wait_all(&pending_comms);
}


float Stream_buffer::get_loss_ratio(){
    cout << total_lost << " of " << total_received << endl;
    return float(total_lost)/float(total_received)*100;
}




//Adds a new quantity of bytes to the buffer and test the basic conditions if the node must attempt to 
//send a window to be processed
int Stream_buffer::add(int bytes, float current_time){


    //Amount of bytes that have been received in total
    total_received += bytes;
    //If the buffer gets full, we should count the amount lost
    if(current_amount + bytes > buffer_size){
        lost_bytes = current_amount + bytes - buffer_size;
        total_lost += lost_bytes;
        current_amount = buffer_size;
        //cout << "Buffer full, lost " << float(lost_bytes)/float(bytes)*100 << "% of package" << endl;
    }
    else
        current_amount += bytes;   

    //Attempt to send bytes to be processed when window_size is filled or a timeout happens
    if (current_amount < window_size && current_time < last_time + stream_timeout){
        //cout << current_amount << " at time " << last_time << " window " << window_size << endl;
        return -1;
    }
    else { //If sending bytes by time_out then send the actual value that is below window size
        if(current_amount < window_size){
            aux_amount = current_amount;
            current_amount = 0; //this functions might need a confirmation that the send packages were able to be sent
            last_time = current_time;
            return aux_amount;
        }
        else{ //Otherwise send a window_sized amount of bytes for processing
            current_amount -= window_size;
            last_time = current_time;
            return window_size;
        }
    }


}
