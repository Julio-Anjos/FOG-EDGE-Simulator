#include <simgrid/s4u.hpp>
#include <iostream>

#include "stream_buffer.h"
using namespace std; 


//Class used my msq_nodes. It acts as the streaming buffer for the node. Indicating
//when window_sized byte packages can be processed, or when the transmission must stop due to
//the buffer being full.
Stream_buffer::Stream_buffer(int wind_size, int buff_size, float timeout)
{
    window_size = wind_size;
    buffer_size = buff_size;
    stream_timeout = timeout;
    current_amount = 0;
    
    

}
