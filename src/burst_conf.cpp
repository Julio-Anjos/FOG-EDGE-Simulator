#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <simgrid/s4u.hpp>
#include <boost/algorithm/string.hpp>
using namespace std; 
#include "burst_conf.h"
#include "shunting-yard.h"

 


//Auxiliar function not of burst_class
string trim_string(string s){
    boost::trim_right(s);
    boost::trim_left(s);
    return s;
        
}

//Initialized at main
void Burst_conf::initialize(string path){
    this->file_path = path;
    burst_logfile.open("result_logs/burst_math_logs.txt",fstream::out | fstream::trunc);
    parse_file();

}


void Burst_conf::parse_file(){
    
    //Opening the file
    ifstream file(this->file_path);
    xbt_assert(file.is_open(), "Failed to open burst_config file.");
        
    

    //Iterate through each line of the file
    string line; 
    size_t found;
    size_t defined_interval;
    size_t division;

    //Auxiliar variables used to complete the interval map
    vector<interval> aux_vec;
    interval aux_interval;
    string current_config; //Config in each the iteration is at
    

    float start_time, end_time;
    int num_packages,package_size;

    bool first_config = true;
    string::iterator end_pos;
    
    while (getline(file, line)) {
        
        
        //Remove all tabs and spaces from this line
        end_pos = std::remove(line.begin(), line.end(), ' ');
        line.erase(end_pos, line.end());
        end_pos = std::remove(line.begin(), line.end(), '\t');
        line.erase(end_pos, line.end());
            


        
        //Removing comments
        found = line.find("//");
        if(found != string::npos){
            line.erase (found,line.length());
        }
    
        line = trim_string(line);
        
        //Skip empty lines and comments 
        if(line == "")
            continue;
        

        //If it is the PRECISION parameter
        found = line.find("PRECISION=");
        if(found != string::npos){
            precision = stof(line.substr(found+10,line.length()));
            continue;
        }


        //If it is the burst_config parameter
        found = line.find("burst_config=");
        if(found != string::npos){
            

            //Save the new information on the interval map
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
        
    
      
       
        //Getting a burst interval information
        found = line.find(":");
        if(found != string::npos){
            //Get end time
            aux_interval.end_time = stof(line.substr(0,found));
            line.erase(0,found+1);


            //Get number of packages
            found = line.find("x");
            if(found != string::npos){
                aux_interval.num_packages = stoi(line.substr(0,found));
                line.erase(0,found+1);
    
                //Get packages_size
                aux_interval.package_size = stoi(line);

                continue;
            }
        }
       

        
        string math_function;
        //Getting a burst interval information
        found = line.find("f(x)=");
        if(found != string::npos){
            defined_interval = line.find("["); // searching for the defined interval ex: [0,2]
            
            aux_interval.math_function = line.substr(found+5,defined_interval -(found+5) ); // 5 is 1 + the length of "f(x)="
            
            division= line.find(",");   //Find the "," that divides both the start and end of the interval

            aux_interval.math_start =stof(line.substr(defined_interval+1, division - (defined_interval+1)));
            aux_interval.math_end = stof(line.substr(division+1, line.size()-1 -(division+1) ));
            
            
            //Save auxiliar vector
            aux_vec.push_back(aux_interval);
      
            continue;
        }
            
        xbt_assert(false, "Burst configuration file is not correct, line: %s",line.c_str());
    
    
    }
            
    //Add the last one
    this->interval_map.insert(make_pair(current_config, aux_vec));
    file.close();
    
    calculate_send_times();
}


//Get the intervals of a certain burst_config
vector<interval> Burst_conf::get_intervals(string burst_config){

    vector<interval> intervals = this->interval_map[burst_config];
    return intervals;
}



//Auxiliar function not of burst_class, returns a vector of packages amount per division
vector<int>  Burst_conf::math_function_match(string math_function, float math_start, float math_end, int num_packages, float interval_start, float interval_end){
    TokenMap vars;  //Initialize constants
    vars["pi"] = 3.14;
    vars["e"] = 2.73;
    double sum=0;
    double y=0;
    vector<double> vy;
    



    burst_logfile << "------------------------------------------------------------------------"<<endl;
    burst_logfile << "STREAM_INTERVAL: " << interval_start << " - " << interval_end << endl;
    burst_logfile << "MATH FUNCTION: f(x)=" << math_function << endl;
    burst_logfile << "MATH_INTERVAL: " << math_start << " - " << math_end << endl;
    

    //Matches a certain mathematical function (math_function) defined in a interval [math_start,math_end]
    //With the function that represents the amount of packages a sensor will send per time

    //Divides in num_divisions the interval between math_start and math_end
    int num_divisions = (interval_end - interval_start)*precision;
    
    
    double step = (interval_end - interval_start)/num_divisions;
    double math_step = (math_end - math_start)/num_divisions;
    calculator calc(math_function.c_str());
    float x = math_start + math_step/2;


    //Calculates a vector with the result of applying the math function with x = (math_start+(math_step/2)) + math_step+i, with i from 0 to num_divisions
    //We use math_start+(math_step/2) so that x doesn't start at 0, which produces worst resulsts
    //We will later scale the results from this vector to get the amount of packages for each division of the interval
    for(int i=0;i<num_divisions;i++){
        vars["x"] = x;
        y= abs(calc.eval(vars).asDouble());
        
        burst_logfile << "x = " << x << "\t\ty = " << y << endl;
        
        sum = sum + y;
        x = x + math_step;

        vy.push_back(y);
    }
    
    double ratio = num_packages/sum;    //Multiplying this number by the calculated y values, we are matching the math function
                                        //with the number of sent packages
    int pkg_sum=0;
    int pkg_amount=0;
    vector<int> package_amounts;
    
    //Create a vector with the amount of packages for each division
    for(int i=0;i<num_divisions;i++){
        pkg_amount = round(vy[i]*ratio);    //This rounding up might cause problems that must be fixed
        package_amounts.push_back(pkg_amount);
        pkg_sum += pkg_amount;
  
    }
    
    //Fix problems due to aproximation of values when using round (maybe less/more packages could be send)
    int difference = num_packages - pkg_sum;
    int i = num_divisions/2;
    
    while(difference < 0){//If the difference is negative, we must remove the extra packages
        if(package_amounts[i] > 0){
            package_amounts[i] -= 1;
            difference += 1;
        }
        i++;
    }
    while(difference > 0){//If the difference is positive, we must add the missing packages
        package_amounts[i] += 1;
        difference -= 1;
        i--;
    }

    
    for(int i =0; i < num_divisions; i++){
        burst_logfile << "Time: " << interval_start+i*step << " - " << interval_start+ (1+i)*step << " Packages: " << package_amounts[i] << endl;
    }
    
   
    return package_amounts;
    
}





//According to each math function calculate how many packets must be send at each second of an interval
void Burst_conf::calculate_send_times(){

    //LOOP THROUGH EACH INTERVAL IN EACH BURST CONFIG
    for(auto const& map_pair : this->interval_map){
        vector<interval>intervals = map_pair.second;
        burst_logfile << "------------------------- Burst_id= " << map_pair.first <<" -------------------------" << endl ;
        float interval_start = 0;
        for(int i=0;i<intervals.size();i++){
           //For every interval calculate the amount of messages to be sent every second 

            intervals[i].package_amounts = math_function_match(intervals[i].math_function,intervals[i].math_start,intervals[i].math_end, intervals[i].num_packages,interval_start,intervals[i].end_time);
            intervals[i].step = (intervals[i].end_time - interval_start)/(intervals[i].package_amounts.size()); //get the step size again
        
        
            interval_start = intervals[i].end_time;

        
        
        }

        //Update the interval map
        this->interval_map[map_pair.first] = intervals;
    }

    vector<interval> intervals  =this->get_intervals("0");

   

};



//Will be accessed globally
Burst_conf burst_config;