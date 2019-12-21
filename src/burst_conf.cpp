#include <iostream>
#include <string>
using namespace std; 
#include "burst_conf.h"

Burst_conf::Burst_conf(){
    something = "nothing";
}

void Burst_conf::set_path(string x){
    something = x;
}

void Burst_conf::dosomething(){
    cout << something << endl;
}

//Will be accessed globally
Burst_conf burst_config;