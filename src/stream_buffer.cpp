#include <simgrid/s4u.hpp>
#include <iostream>

#include "stream_buffer.h"
using namespace std; 

//THIS CLASS IS CURRENTLY ON A TEMPLATE STAGE, NOT BEING USED

//Class used my msq_nodes. It acts as the streaming buffer for the node. Indicating
//when window_sized byte packages can be processed, or when the transmission must stop due to
//the buffer being full.
Stream_buffer::Stream_buffer(int wind_size, int buff_size, float timeout, string equation)
{
    window_size = 0.999*wind_size;
    buffer_size = buff_size;
    stream_timeout = timeout;
    lost_bytes = 0;
    last_time = 0;
    current_amount = 0;
    aux_amount = 0;
    total_received = 0;
    total_lost = 0;
    process_equation = equation;
    
}

//Adds a new quantity of bytes to the buffer and test the basic conditions if the node must attempt to 
//send a window to be processed
int Stream_buffer::add(int bytes, float current_time){

    total_received += bytes;

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
        //cout << current_amount << " at time " << last_time << endl;
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

string Stream_buffer::get_equation(){
    return process_equation;
}

float Stream_buffer::get_loss_ratio(){
    cout << total_lost << " of " << total_received << endl;
    return float(total_lost)/float(total_received)*100;
}