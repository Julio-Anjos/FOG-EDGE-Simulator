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



    // Stream_master constructor
Stream_master::Stream_master(vector<string> args){

    //Number of stream_buffers connected
    num_buffers = stoi(args[1]);
    //Testing arguments (localized on the deploy platform file)
    xbt_assert(args.size() > num_buffers, "Stream_master missing arguments.");


    size_t found;
    //Initializing auxiliary struct stream_node, each stream-buffer is paired with a msq_node, used to make the in and out mailboxes
    for(int i = 2; i<num_buffers+2;i++){
        line = args[i];
        found = line.find("_");
        aux_stream_node.sensor_name = line.substr(0, found);
        aux_stream_node.msq_name = line.substr(found+1, line.length());
        aux_stream_node.burst_counter = 0;
        aux_stream_node.end_burst = false;
        stream_nodes.push_back(aux_stream_node);
        mailbox_in.push_back(simgrid::s4u::Mailbox::by_name(aux_stream_node.sensor_name + "_StreamMaster"));
        mailbox_out.push_back(simgrid::s4u::Mailbox::by_name( "StreamMaster_" + aux_stream_node.msq_name));
    }
}

void Stream_master::operator()(void){
    int pos_pair = 0;
    aux_stream_node = stream_nodes[pos_pair];
    host = &msq_host_map[aux_stream_node.msq_name];
    finished_pairs=0;
    //Vector to keep track of the end flags that we'll receive
    for(int i = 0; i < num_buffers; i++)
        end_flags.push_back(0);
    //The loop goes through each streambuffer-msqnode pair for the stream-master (these pairs can be found in the deployment file)
    //until all messages are sent
    while(finished_pairs < stream_nodes.size()){
        //If the current pair has not finished, receive and send the messages, otherwise go to the next pair
        if(!end_flags[pos_pair]){
            int *stop_flag = new int(-1);
            double* current_time = new double();
            // payload = -1 MEANS THE COMMUNICATION MUST STOP
            // Otherwise, payload is the number of bytes that were transmitted
            // When a payload = -1 is received, it indicates that the current burst ended
            int *payload;  

            int interval_sent_packages = 0;
            receive_packages(pos_pair);
            //For this specific interval, get the amount of packages correctly sent
            if(!end_flags[pos_pair]){
                int *temp  = static_cast<int*>(mailbox_in[pos_pair]->get());
                interval_sent_packages =*temp;
                mailbox_out[pos_pair]->put(new int(interval_sent_packages), 0);
            }
        }
        //pos_pair is used has the index of the position of each pair in a circular loop
        pos_pair = (pos_pair+1) % stream_nodes.size();
        aux_stream_node = stream_nodes[pos_pair];

        
    }
}
void Stream_master::receive_packages(int pos_pair){

    int *payload;       
    // payload = -1 MEANS THE COMMUNICATION MUST STOP
    // Otherwise, payload is the number of bytes that were transmitted
    // When a payload = -1 is received, it indicates that the current burst ended
    double* current_time = new double();

    do{
        //Receive a payload
        payload = static_cast<int*>(mailbox_in[pos_pair]->get()); //Receive data from sensor
        *current_time = simgrid::s4u::Engine::get_clock();
        if(*payload != -1 && *payload != -2){
            mailbox_out[pos_pair]->put(payload, *payload);  
        }
    }while(*payload != -1 && *payload != -2); //Check if all sensors of this node have ended        
    if(*payload == -2){ //If received an end flag, increment finished pairs and mark it
        finished_pairs++;
        end_flags[pos_pair] = 1;
    } else
        mailbox_out[pos_pair]->put(payload, 0); //Otherwise send the -1 received


}

Stream_buffer::Stream_buffer(vector<string> args){
    num_pairs = stoi(args[1]);
    xbt_assert(args.size() > num_pairs,"Stream_buffer missing arguments.");
    size_t found;

    host_name = simgrid::s4u::this_actor::get_host()->get_name();
    //This is an auxiliary structure used to organize info about the in and out mailboxes
    for(int i = 2; i<num_pairs+2;i++){
        line = args[i];
        found = line.find("_");
        aux_sensor_node.sensor_name = line.substr(0, found);
        aux_sensor_node.msq_name = line.substr(found+1, line.length());
        aux_sensor_node.burst_counter = 0;
        aux_sensor_node.end_burst = false;
        sensor_nodes.push_back(aux_sensor_node);
        mailbox_in.push_back(simgrid::s4u::Mailbox::by_name(aux_sensor_node.sensor_name + "_" + host_name));  
    }
    // There is only one outgoing mailbox, that funnels all the messages from the incoming mailboxes
    mailbox_out = simgrid::s4u::Mailbox::by_name(host_name + "_StreamMaster");
    stream_host = simgrid::s4u::Host::by_name("StreamMaster");
    actors = stream_host->get_all_actors();
    stream_actor = actors.front();
    mailbox_out->set_receiver(stream_actor);

} 


void Stream_buffer::operator()(void){

    int pos_pair = 0;
    aux_sensor_node = sensor_nodes[pos_pair];
    host = &msq_host_map[aux_sensor_node.msq_name];
    int finished_pairs=0;
    //The loop goes through each sensor-streambuffer pair for the current stream-buffer (these pairs can be found in the deployment file)
    //until all messages are sent
    while(finished_pairs < sensor_nodes.size()){

        host = &msq_host_map[aux_sensor_node.msq_name];
        window_size = host->get_window();
        buffer_size = host->get_buffer();
        stream_timeout = host->get_timeout();
        intervals = host->get_intervals();
        //If the current pair hasn't gone through all bursts yet 
        if(aux_sensor_node.burst_counter < intervals.size()){

            int *stop_flag = new int(-1);
            double* current_time = new double();
            // payload = -1 MEANS THE COMMUNICATION MUST STOP
            // Otherwise, payload is the number of bytes that were transmitted
            // When a payload = -1 is received, it indicates that the current burst ended
            int *payload;
            // Values used when adding bytes to the buffer window  
            current_amount = 0;
            total_received = 0;
            lost_bytes = 0;
            total_lost = 0;
            aux_amount = 0;
            last_time = simgrid::s4u::Engine::get_clock();
            process_amount = 0;
            int interval_sent_packages = 0;
            int num_pkg_divisions = intervals[aux_sensor_node.burst_counter].package_amounts.size(); //How many division each interval has 
            // Receiving packages function
            for(int i =0;i<num_pkg_divisions;i++){
                receive_packages(pos_pair);
            }
            //For this specific interval, get the amount of packages correctly sent
            int *temp  = static_cast<int*>(mailbox_in[pos_pair]->get());
            interval_sent_packages =*temp;
            //cout << interval_sent_packages << endl;
            comm = mailbox_out->put_async(new int(interval_sent_packages), 0);
            stream_comms.push_back(comm);

            sensor_nodes[pos_pair].burst_counter += 1;
        }
        else{
            //If pair has ended increment finished pairs and set end flag
            if(!aux_sensor_node.end_burst){
                aux_sensor_node.end_burst = true;
                finished_pairs++;
            }
        }
        //pos_pair is used has the index of the position of each pair in a circular loop
        pos_pair = (pos_pair+1) % sensor_nodes.size();
        aux_sensor_node = sensor_nodes[pos_pair];        
    }
    int *end_flag = new int(-2);
    comm = mailbox_out->put_async(end_flag, 0);
    stream_comms.push_back(comm);
    simgrid::s4u::Comm::wait_all(&stream_comms);

}
void Stream_buffer::receive_packages(int pos_pair){


    int *payload;       
    // payload = -1 MEANS THE COMMUNICATION MUST STOP
    // Otherwise, payload is the number of bytes that were transmitted
    // When a payload = -1 is received, it indicates that the current burst ended
    double* current_time = new double();
    do{
        //Receive a payload
        payload = static_cast<int*>(mailbox_in[pos_pair]->get()); //Receive data from sensor
        *current_time = simgrid::s4u::Engine::get_clock();
        if(*payload != -1){
            process_amount = add(*payload, *current_time, 0);
            if(process_amount != -1){
                comm = mailbox_out->put_async(&process_amount, process_amount);
                stream_comms.push_back(comm);
            }
        }
    }while(*payload != -1); //Check if all sensors of this node have ended
    if(process_amount == -1){ // If current burst has ended and the buffer is not empty send what's remaining
        process_amount = add(0, *current_time, 1);
        comm = mailbox_out->put_async(&process_amount, process_amount);
        stream_comms.push_back(comm);
    }
    comm = mailbox_out->put_async(payload, 0);
    stream_comms.push_back(comm);
    }

//Function to get the total loss ratio of bytes in percentage
float Stream_buffer::get_loss_ratio(){
    cout << total_lost << " of " << total_received << endl;
    return float(total_lost)/float(total_received)*100;
}

//Adds a new quantity of bytes to the buffer and test the basic conditions if the node must attempt to 
//send a window to be processed
int Stream_buffer::add(int bytes, float current_time, bool end_early){

    //Amount of bytes that have been received in total
    total_received += bytes;
    //If the buffer gets full, we should count the amount lost
    if(current_amount + bytes > buffer_size){
        lost_bytes = current_amount + bytes - buffer_size;
        total_lost += lost_bytes;
        current_amount = buffer_size;
    }
    else
        current_amount += bytes;   
    //Attempt to send bytes to be processed when window_size is filled or a timeout happens
    if (current_amount < window_size && current_time < last_time + stream_timeout && !end_early){
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
