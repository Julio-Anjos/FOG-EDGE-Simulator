#include <simgrid/s4u.hpp>
#include "stream_buffer.h"


using namespace std; 


//Class used my msq_nodes. It acts as the streaming buffer for the node. Indicating
//when window_sized byte packages can be processed, or when the transmission must stop due to
//the buffer being full.
Stream_buffer::Stream_buffer()
{
    
    return;

}
