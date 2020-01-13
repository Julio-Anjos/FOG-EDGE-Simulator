#!/bin/bash
make 

chmod 644 $1

#Grab first line of file
read -r line<$1
echo $line 


#Create platform with config file
cd platforms
python createplats.py "../$1"
cd ..
