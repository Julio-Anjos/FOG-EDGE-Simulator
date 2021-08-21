#include <simgrid/s4u.hpp>
#include <iostream>
#include "stream_buffer.h"
using namespace std; 

//THIS CLASS IS CURRENTLY ON A TEMPLATE STAGE, NOT BEING USED

//Class used my msq_nodes. It acts as the streaming buffer for the node. Indicating
//when window_sized byte packages can be processed, or when the transmission must stop due to
//the buffer being full.
Stream_buffer::Stream_buffer(int wind_size, int buff_size, float timeout)
{
    window_size = wind_size;
    buffer_size = buff_size;
    stream_timeout = timeout;
    last_time = 0;
    current_amount = 0;
    
}

//Adds a new quantity of bytes to the buffer and test the basic conditions if the node must attempt to 
//send a window to be processed
string Stream_buffer::add(int bytes, float current_time)
{

    current_amount = current_amount + bytes;
    cout << "WS = " << window_size << " CA = " << current_amount << endl;

    //Attempt to send bytes to be processed when window_size is filled or a timeout happens
    if (current_amount < window_size && current_time < last_time + stream_timeout)
        return "continue";
    else{
        current_amount -= window_size; //this functions might need a confirmation that the send packages were able to be sent
        last_time = current_time;
        return "send";
    }

}
