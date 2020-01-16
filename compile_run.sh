#!/bin/bash
make 

chmod 644 $1

#Grab first line of file
read -r line<$1



#Create platform with config file
cd platforms
python createplats.py "../$1"
cd ..

#Get platform name
plat_name=$(echo $line | cut -d' ' -f1)



platform="platforms/${plat_name}.xml"
deploy_platform="platforms/d_${plat_name}.xml"


./main ${platform} ${deploy_platform} $2