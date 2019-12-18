#include <iostream>
#include <string>
using namespace std; 
#include "burst_conf.h"

Burst_conf::Burst_conf(string x){
    something = x;
}

void Burst_conf::dosomething(){
    cout << something << endl;
}

Burst_conf burst_config("Hello, this is a test.");