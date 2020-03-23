#ifndef STREAM_BUFFER_H
#define STREAM_BUFFER_H

using namespace std; 

//THIS CLASS IS CURRENTLY ON A TEMPLATE STAGE, NOT BEING USED

//Class used my msq_nodes. It acts as the streaming buffer for the node. Indicating
//when window_sized byte packages can be processed, or when the transmission must stop due to
//the buffer being full.
class Stream_buffer
{


    private:
        int window_size; //Size in bytes, when this window fills up, the data will be send to be processed, unless warned by the processing unit that it cant. 
        int buffer_size; //Size in bytes, the total buffer size, if the buffer fills up without the data being processed, the sensors are warned to stop sending data.
        float stream_timeout;  //Amount of time in seconds before the data in the buffer is send to be processed even if the window size hasn't completly filled up.
        int current_amount; //Current amount of bytes in the buffer
        float last_time;; //Last time a window was send for processing

    public:
        
        Stream_buffer(int window_size, int buffer_size, float stream_timeout);
        string add(int bytes, float current_time); //Adds a new quantity of bytes to the buffer and test the basic conditions if the node must attempt to send a window to be processed
        
};

#endif