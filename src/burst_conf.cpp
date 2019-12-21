#include <iostream>
#include <string>
#include <fstream>
#include <simgrid/s4u.hpp>
#include <boost/algorithm/string.hpp>
using namespace std; 
#include "burst_conf.h"

//Auxiliar function not of burst_class
string trim_string(string s){
    boost::trim_right(s);
    boost::trim_left(s);
    return s;
        
}

//Initialized at main
void Burst_conf::initialize(string path){
    this->file_path = path;
    parse_file();

}



void Burst_conf::parse_file(){
    
    //Opening the file
    ifstream file(this->file_path);
    xbt_assert(file.is_open(), "Failed to open burst_config file.");
        
    

    //Iterate through each line of the file
    string line; 
    size_t found;

    //Auxiliar variables used to complete the interval map
    vector<interval> aux_vec;
    interval aux_interval;
    string current_config; //Config in each the iteration is at
    

    float start_time, end_time;
    int num_packages,package_size;

    bool first_config = true;

    while (getline(file, line)) {
        
        
        
        //Removing comments
        found = line.find("//");
        if(found != string::npos){
            line.erase (found,line.length());
        }
    
        line = trim_string(line);
        
        //Skip empty lines and comments 
        if(line == "")
            continue;

        //If it is the burst_config parameter
        found = line.find("burst_config=");
        if(found != string::npos){
            
            //Every time we change config we save the new information on the map
            if (!first_config){
                
                this->interval_map.insert(make_pair(current_config, aux_vec));
                aux_vec.clear();
                
            }

            //13 is burst_config size
            current_config = line.substr(found+13,line.length());
            current_config = trim_string(current_config);


            first_config = false;
            continue;
        }
        
        
        string::iterator end_pos;
        //Getting a burst interval information
        found = line.find("-");
        if(found != string::npos){
            
            //Remove all tabs and spaces from this interval information
            end_pos = std::remove(line.begin(), line.end(), ' ');
            line.erase(end_pos, line.end());
            end_pos = std::remove(line.begin(), line.end(), '\t');
            line.erase(end_pos, line.end());
            
            //Get starting time
            found = line.find("-");
            if(found != string::npos){
                aux_interval.start_time = stof(line.substr(0,found));
                line.erase(0,found+1);
            

                //Get end time
                found = line.find(":");
                if(found != string::npos){
                    aux_interval.end_time = stof(line.substr(0,found));
                    line.erase(0,found+1);
                

                    //Get number of packages
                    found = line.find("x");
                    if(found != string::npos){
                        aux_interval.num_packages = stoi(line.substr(0,found));
                        line.erase(0,found+1);
            
                        //Get packages_size
                        aux_interval.package_size = stoi(line);

                        //Save auxiliar vector
                        aux_vec.push_back(aux_interval);
                        

                        continue;
                    }
                }
            }
        }
                
            
        xbt_assert(false, "Burst configuration file is not correct, line: %s",line.c_str());
    }
            
        
      
    file.close();
}

vector<interval> Burst_conf::get_intervals(string burst_config){

    vector<interval> intervals = this->interval_map[burst_config];
    
    for (int i = 0; i < intervals.size(); i++) {
		cout << intervals[i].num_packages << endl;
	}


    return intervals;
}

//Will be accessed globally
Burst_conf burst_config;