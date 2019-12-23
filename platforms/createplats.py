import sys

file_start = "<?xml version=\'1.0\'?>\n<!DOCTYPE platform SYSTEM \"http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd\">\n<platform version=\"4.1\">\n"
file_end = "\n</platform>"


#Getting path of the config
def get_config():
    arguments = sys.argv
    del(arguments[0])

    if not arguments:
        raise Exception("Please include path to platform config file.")


    return arguments[0] #Config file name

#Read the config file
def read_config(config_path):
    f = open(config_path, "r")
    
    config = []

    for line in f:
        line = line.strip('\n')
        if not line.startswith("//") and line: #Removing comments and empty lines
            line = line.split("//")[0].replace(" ", "").replace("\t","")#Removing comments and \t and " "
            line = line.split("=")
 
            config.append(line)

    return config






def write_plat_sensor(f,id,speed):
    f.write("       <host id=\"Sensor-"+str(id)+"\" speed=\""+ speed +"f\"  />\n")

def write_plat_msq_node(f,id,speed):
    f.write("       <host id=\"Msq_node-"+str(id)+"\" speed=\""+ speed +"f\"  />\n")

def write_connection(f,sensor_id,msq_node_id):
    f.write("       <route src=\"Sensor-"+str(sensor_id)+"\" dst=\"Msq_node-"+str(msq_node_id)+"\" symmetrical=\"yes\">\n           <link_ctn id=\"1\"/>\n       </route>\n")
    

def write_plat_file(config):
    
    #Creating file with the platform name
    if config[0][0] == "plat_name":
        platform_name = config[0][1]
        if not platform_name.endswith(".xml"):
            platform_name = platform_name + ".xml"
        f = open(platform_name, "w")
    else:
        raise Exception("First parameter on platform config file must be plat_name")


    #Start file
    f.write(file_start)
    f.write("   <zone id=\"zone0\" routing=\"Full\">\n")


    #Keeping track of how many sensors are in each msq_node
    sensor_amount = []
    #Lets us keep track of sensors and msq_nodes
    id_sensor = 0
    id_msq_node = 0
    
    for i in range(len(config)):
        
        parameter = config[i]

        if i != len(config)-1:
            next_parameter = config[i+1]

        
        if parameter[0] == "burst_config" or parameter[0] ==  "sensors_speed" or parameter[0] == "plat_name":
            continue

        
        if parameter[0] == "msq_node_speed":
            write_plat_msq_node(f,id_msq_node,parameter[1])
            id_msq_node += 1
            continue
        
        if parameter[0] == "num_sensors":
            if next_parameter[0] == "sensors_speed":
               num_sensors = int(parameter[1])
               
               sensor_amount.append(num_sensors) 
               
               for i in range(num_sensors):
                    write_plat_sensor(f,id_sensor,next_parameter[1])
                    id_sensor += 1
            continue

        raise Exception("Parameter " + parameter[0] + "does not exist.")


        
    #Lets make the connections
    f.write("\n       <link id=\"1\" bandwidth=\"50MBps\" latency=\"0us\"/>\n\n")
    msq_node_id = 0
    sensor_id = 0
    for num_sensors in sensor_amount:
        for i in range(num_sensors):
            write_connection(f,sensor_id,msq_node_id)
            sensor_id += 1
        msq_node_id += 1


    #End file
    f.write("\n   </zone>")
    f.write(file_end)
    
    f.close()



def write_argument(f,value,comment):
    f.write("       <argument value=\""+value+"\"/>   <!-- "+ comment +" --> \n")

def write_deploy_sensor(f,sensor_id):
    f.write("   <actor host=\"Sensor-"+str(sensor_id)+"\" function=\"sensor\">\n")

def write_deploy_msq_node(f,msq_node_id):
    f.write("   <actor host=\"Msq_node-"+str(msq_node_id)+"\" function=\"msq_node\">\n")
    f.write("   </actor>\n\n")

def write_d_plat_file(config):



    #Creating file with the platform name
    if config[0][0] == "plat_name":
        platform_name = config[0][1]
        platform_name = "d_" + platform_name 
        if not platform_name.endswith(".xml"):
            platform_name = platform_name + ".xml"
        f = open(platform_name, "w")
    else:
        raise Exception("First parameter on platform config file must be plat_name")

    if not platform_name.endswith(".xml"):
        platform_name = platform_name + ".xml"
    f = open(platform_name, "w")
    f.write(file_start)

   
    #Lets us keep track of sensors and msq_nodes
    id_sensor = 0
    id_msq_node = 0
    sensor_amounts = []
    burst_configs = []
    for i in range(len(config)):
        parameter = config[i]
        if parameter[0] == "num_sensors":
            sensor_amounts.append(int(parameter[1]))
        if parameter[0] == "burst_config":
            burst_configs.append(parameter[1])


    msq_node_id = 0
    sensor_id = 0

    for num_sensors in sensor_amounts:
        write_deploy_msq_node(f,msq_node_id)
        for i in range(num_sensors):
            
            #Writes the nodes and arguments with comments explaining each argument
            write_deploy_sensor(f,sensor_id)
            write_argument(f,str(msq_node_id),"connected msq node id")
            write_argument(f,burst_configs[msq_node_id],"burst config id")
            write_argument(f,str(num_sensors),"number of sensors in the same msq node connection")
            f.write("   </actor>\n\n")
            
            sensor_id += 1
        msq_node_id += 1

    
    f.write(file_end)
    f.close()



def main():
    config_path = get_config()

    config = read_config(config_path)
    
    write_plat_file(config)
    
    write_d_plat_file(config)



if __name__ == "__main__":
    main()