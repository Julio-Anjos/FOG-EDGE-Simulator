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

#Read the config file and returning it as a list o strings
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




def write_plat_msq_ps_node(f,id,speed,num_disks,disks_bw):
    f.write("       <host id=\"MsqPSNode-"+str(id)+"\" speed=\""+ speed +"f\"  >\n")
    for i in range(num_disks):
        f.write("              <disk id=\"Disk-"+str(i)+"\" read_bw=\""+disks_bw[0]+"\" write_bw=\""+disks_bw[1]+"\"  />\n")
    f.write("       </host>\n")

def write_plat_sensor(f,id,speed):
    f.write("       <host id=\"Sensor-"+str(id)+"\" speed=\""+ speed +"f\"  />\n")

def write_plat_msq_node(f,id,speed):
    f.write("       <host id=\"MsqNode-"+str(id)+"\" speed=\""+ speed +"f\"  />\n")

def write_link_ps(f,msq_node_id, msq_ps_id, connection_speed, latency):
    f.write("\n       <link id=\"" + "MsqNode-"+str(msq_node_id)+"_MsqPSNode-"+str(msq_ps_id)+"\" bandwidth=\""+ connection_speed +"\" latency=\""+ latency +"\" />")

def write_link(f,sensor_id,msq_node_id,connection_speed,latency):
    f.write("\n       <link id=\"" + "Sensor-"+ str(sensor_id) + "_MsqNode-" +str(msq_node_id)  + "\" bandwidth=\""+ connection_speed +"\" latency=\""+ latency +"\" />")

def write_connection(f,sensor_id,msq_node_id):
    f.write("       <route src=\"Sensor-"+str(sensor_id)+"\" dst=\"MsqNode-"+str(msq_node_id)+"\" >\n           <link_ctn id=\"" + "Sensor-"+ str(sensor_id) + "_MsqNode-" +str(msq_node_id)+"\" />\n       </route>\n")

def write_connection_ps(f,msq_node_id,msq_ps_id):
    f.write("       <route src=\"MsqNode-"+str(msq_node_id)+"\" dst=\"MsqPSNode-"+str(msq_ps_id)+"\" >\n           <link_ctn id=\"" + "MsqNode-"+ str(msq_node_id) + "_MsqPSNode-" +str(msq_ps_id)+"\" />\n       </route>\n")
      

def write_plat_file(config):
    
    #Creating file with the platform name

    platform_name = config[0][0]
    if not platform_name.endswith(".xml"):
        platform_name = platform_name + ".xml"
    f = open(platform_name, "w")



    #Start file
    f.write(file_start)
    f.write("   <zone id=\"zone0\" routing=\"Full\">\n")


    num_disks = 0
    disks_bw = []

    #Keeping track of links and their speeds
    link_id=0
    link_speed = []
    link_latency = []
    link_ps_id = 0
    link_ps_speed = []
    link_ps_latency = []
    
    #Keeping track of how many msq_nodes are in each msq_ps_node
    msq_nodes_amount = []

    #Keeping track of how many sensors are in each msq_node
    sensor_amount = []
    #Lets us keep track of msq_ps_nodes, sensors and msq_nodes
    id_sensor = 0
    id_msq_node = 0
    id_msq_ps_node = 0
    
    for i in range(len(config)):
        if i == 0:  #first parameter is always plat_name
            continue

        parameter = config[i]

        if i != len(config)-1:
            next_parameter = config[i+1]


        if parameter[0] == "msq_ps_node_speed":
            if next_parameter[0] == "num_disks":
                num_disks = int(next_parameter[1])
                disks_bw.append(config[i + 2][1])
                disks_bw.append(config[i + 3][1])

            write_plat_msq_ps_node(f,id_msq_ps_node, parameter[1], num_disks, disks_bw)
            continue
 

        if parameter[0] == "num_msq_nodes":
            if next_parameter[0] == "msq_node_speed":
                num_msq_nodes = int(parameter[1])

                msq_nodes_amount.append(num_msq_nodes)

                for i in range(num_msq_nodes):
                    write_plat_msq_node(f, id_msq_node, next_parameter[1])
                    id_msq_node += 1
            continue


        if parameter[0] == "msq_node_connection_speed":
            if next_parameter[0] == "msq_node_connection_latency":
                for i in range(num_msq_nodes):
                    link_ps_speed.append(parameter[1])
                    link_ps_latency.append(next_parameter[1])
                    link_ps_id += 1
                continue


        if parameter[0] == "connection_speed":
            if next_parameter[0] == "connection_latency":
                for i in range(num_sensors):
                    link_speed.append(parameter[1])
                    link_latency.append(next_parameter[1])
                    link_id += 1
                continue


        if parameter[0] == "num_sensors":
            if next_parameter[0] == "sensors_speed":
               num_sensors = int(parameter[1])
               
               sensor_amount.append(num_sensors) 
               
               for i in range(num_sensors):
                    write_plat_sensor(f,id_sensor,next_parameter[1])
                    id_sensor += 1
            continue


        #MAKING SURE ONLY THE ALLOWED PARAMETERS ARE BEING USED
        if parameter[0] == "burst_config" or parameter[0] ==  "sensors_speed" or parameter[0] == "plat_name" or parameter[0] ==  "connection_latency":
            continue 
        if parameter[0] == "stream_timeout_time" or parameter[0] == "stream_buffer_size" or parameter[0] == "stream_window_size":
            continue
        if  parameter[0] == "msq_node_speed" or parameter[0] == "msq_node_connection_speed" or parameter[0] == "msq_node_connection_latency":
            continue
        if parameter[0] == "num_disks" or parameter[0] == "disks_read_bw" or parameter[0] == "disks_write_bw":
            continue
        if parameter[0] == "ps_stream_timeout_time" or parameter[0] == "ps_stream_buffer_size" or parameter[0] == "ps_stream_window_size":
            continue

        raise Exception("Parameter " + parameter[0] + " does not exist.")


    f.write("\n        <!-- The bandwidth is defined during the program execution by the burst_config -->\n")
        
    #Making the links

    msq_ps_node_id = 0
    msq_node_id = 0
    for num_msq_nodes in msq_nodes_amount:
        #creates empty profile files, they will be written during the program
        for i in range(num_msq_nodes):
            write_link_ps(f, msq_node_id,msq_ps_node_id,link_ps_speed[msq_node_id],link_latency[msq_node_id])
            msq_node_id += 1
        msq_ps_node_id += 1


    msq_node_id = 0
    sensor_id = 0
    for num_sensors in sensor_amount:
        #creates empty profile files, they will be written during the program
        for i in range(num_sensors):
            write_link(f,sensor_id,msq_node_id,link_speed[sensor_id],link_latency[sensor_id])
            sensor_id += 1
        msq_node_id += 1


    f.write("\n\n")
    #Making the connections

    msq_node_id = 0
    msq_ps_id = 0
    for num_msq_nodes in msq_nodes_amount:
        for i in range(num_msq_nodes):
            write_connection_ps(f,msq_node_id,msq_ps_id)
            msq_node_id += 1
        msq_ps_id+= 1


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

def write_deploy_msq_ps_actor(f,msq_ps_node_id):
    f.write("   <actor host=\"MsqPSNode-"+str(msq_ps_node_id)+"\" function=\"msq_actor_ps\">\n")

def write_deploy_sensor(f,sensor_id):
    f.write("   <actor host=\"Sensor-"+str(sensor_id)+"\" function=\"sensor\">\n")

def write_deploy_msq_actor(f,msq_node_id):
    f.write("   <actor host=\"MsqNode-"+str(msq_node_id)+"\" function=\"msq_actor\">\n")

def write_d_plat_file(config):



    #Creating file with the platform name

    platform_name = config[0][0]
    platform_name = "d_" + platform_name 
    if not platform_name.endswith(".xml"):
        platform_name = platform_name + ".xml"
    f = open(platform_name, "w")
    

    if not platform_name.endswith(".xml"):
        platform_name = platform_name + ".xml"
    f = open(platform_name, "w")
    f.write(file_start)

   
    #Lets us keep track of sensors and msq_nodes
    msq_nodes_amount = []
    sensor_amounts = []
    burst_configs = []
    
    ps_window_sizes= []
    ps_buffer_sizes=[]
    ps_timeout_times=[]
    
    window_sizes= []
    buffer_sizes=[]
    timeout_times=[]

    #Getting the arguments for the msq_nodes
    for i in range(len(config)):

        if i == 0:  #first parameter is always plat_name
            continue
            
        parameter = config[i]
       
        if parameter[0] == "num_msq_nodes":
            msq_nodes_amount.append(int(parameter[1]))

        if parameter[0] == "num_sensors":
            sensor_amounts.append(int(parameter[1]))
        
        if parameter[0] == "burst_config":
            burst_configs.append(parameter[1])

        if parameter[0] == "ps_stream_window_size":
            ps_window_sizes.append(parameter[1])

        if parameter[0] == "ps_stream_buffer_size":
            ps_buffer_sizes.append(parameter[1])
           
        if parameter[0] == "ps_stream_timeout_time":
            ps_timeout_times.append(parameter[1])
    
        if parameter[0] == "stream_window_size":
            window_sizes.append(parameter[1])

        if parameter[0] == "stream_buffer_size":
            buffer_sizes.append(parameter[1])
           
        if parameter[0] == "stream_timeout_time":
            timeout_times.append(parameter[1])
    


    msq_ps_node_id = 0
    msq_node_id1 = 0
    #Writing for the MSQ node each of its arguments, arguments include: Burst config id, connected sensors, and information about the streaming window and buffer
    for num_msq_nodes in msq_nodes_amount: #FOR EACH MSQ_PS_NODE

 
        for i in range(num_msq_nodes):
            write_deploy_msq_ps_actor(f,msq_ps_node_id)
            write_argument(f,burst_configs[msq_ps_node_id],"comms config id")
            write_argument(f,"MsqNode-"+str(msq_node_id1),"MsqNode connected actor")
            write_argument(f,ps_window_sizes[msq_ps_node_id],"stream window size")
            write_argument(f,ps_buffer_sizes[msq_ps_node_id],"stream buffer size")
            write_argument(f,ps_timeout_times[msq_ps_node_id],"stream timeout time")
            f.write("   </actor>\n\n")
            msq_node_id1 += 1


    msq_node_id = 0
    msq_ps_node_id = 0
    sensor_id1 = 0
    sensor_id2 = 0
    #Writing for the MSQ node each of its arguments, arguments include: Burst config id, connected sensors, and information about the streaming window and buffer
    for num_sensors in sensor_amounts: #FOR EACH MSQ_NODE

 
        for i in range(num_sensors):
            write_deploy_msq_actor(f,msq_node_id)
            write_argument(f,burst_configs[msq_node_id],"burst config id")
            write_argument(f,"Sensor-"+str(sensor_id1),"sensor connected actor")
            write_argument(f,window_sizes[msq_node_id],"stream window size")
            write_argument(f,buffer_sizes[msq_node_id],"stream buffer size")
            write_argument(f,timeout_times[msq_node_id],"stream timeout time")
            write_argument(f,"MsqPSNode-"+str(msq_ps_node_id),"connected msq ps actor")
            f.write("   </actor>\n\n")
            sensor_id1 += 1

        
        for i in range(num_sensors):
            write_deploy_sensor(f,sensor_id2)
            write_argument(f,burst_configs[msq_node_id],"burst config id")
            write_argument(f,"MsqNode-"+str(msq_node_id),"connected msq node host")
            write_argument(f,str(num_sensors),"total number of sensors connected to host")
            write_argument(f,str(i),"position of the sensor in relation of other sensors connected to msq_node")
            f.write("   </actor>\n\n")
            
            sensor_id2 += 1
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