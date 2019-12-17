import sys

file_start = "<?xml version=\'1.0\'?>\n<!DOCTYPE platform SYSTEM \"http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd\">\n<platform version=\"4.1\">\n"
file_end = "\n</platform>"


#Getting path of the config
def get_config():
	arguments = sys.argv
	del(arguments[0])

	if not arguments:
		print("Please include path to config file.")
		exit()

	return arguments[0]	#Config file name

#Read the config file
def read_config(config_path):
	f = open(config_path, "r")
	
	config = []

	for line in f:
		line = line.strip('\n')
		if not line.startswith("//") and line: #Removing comments and empty lines
			line = line.split("//")[0].strip("\t").strip(" ") #Removing comments and \t and " "
			line = line.split("=")
			config.append(line)

	return config


def write_connections(f,num_sensors,num_nodes):
	f.write("\n\n 	<link id=\"1\" bandwidth=\"50MBps\" latency=\"50us\"/>\n\n")
	
	for i in range(num_sensors):
		for j in range(num_nodes):
			f.write("	<route src=\"Sensor-"+str(i)+"\" dst=\"Msq_node-"+str(j)+"\" symmetrical=\"yes\">\n 		<link_ctn id=\"1\"/>\n 	</route>\n")
	
	
def write_plat_file(config):
	
	print(config)
	'''
	if not platform_name.endswith(".xml"):
		platform_name = platform_name + ".xml"
	
	f = open(platform_name, "w")
	
	f.write(file_start)
	f.write("	<zone id=\"zone0\" routing=\"Full\">\n")
	for i in range(num_sensors):
		f.write("	<host id=\"Sensor-"+str(i)+"\" speed=\"1000000000.0f\"  />\n")

	for i in range(num_nodes):
		f.write("	<host id=\"Msq_node-"+str(i)+"\" speed=\"1000000000.0f\"  />\n")


	write_connections(f,num_sensors,num_nodes)

	f.write("\n 	</zone>")
	f.write(file_end)
	f.close()
	'''
def write_d_plat_file(num_sensors,num_nodes,platform_name,arguments):
	
	platform_name = "d_" + platform_name 
	
	if not platform_name.endswith(".xml"):
		platform_name = platform_name + ".xml"
	
	f = open(platform_name, "w")

	f.write(file_start)

	
	for i in range(num_sensors):
		f.write("	<actor host=\"Sensor-"+str(i)+"\" function=\"sensor\">\n")
		for value in arguments:
			f.write("		<argument value=\""+value+"\"/>\n")
		f.write("	</actor>\n")


	for i in range(num_nodes):
		f.write("	<actor host=\"Msq_node-"+str(i)+"\" function=\"msq_node\">\n")
		for value in arguments:
			f.write("		<argument value=\""+value+"\"/>\n")
		f.write("	</actor>\n")



	f.write(file_end)
	f.close()



def main():
	config_path = get_config()

	config = read_config(config_path)
	
	write_plat_file(config)
	
	#write_d_plat_file(num_sensors,num_nodes,platform_name,arguments)



if __name__ == "__main__":
	main()