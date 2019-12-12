import sys

file_start = "<?xml version=\'1.0\'?>\n<!DOCTYPE platform SYSTEM \"http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd\">\n<platform version=\"4.1\">\n"
file_end = "\n</platform>"


#Getting arguments
def get_arguments():
	arguments = sys.argv
	del(arguments[0])


	if len(arguments) < 3:
		print("Number of sensors,number of nodes and platform_name is needed.")
		exit()

	platform_name = arguments[0]
	del(arguments[0])

	try:
		num_sensors = int(arguments[0])
		num_nodes = int(arguments[1])
		del(arguments[0])
		del(arguments[0])
	except:
		print("Number of sensors and number of nodes must be numbers.")



	return num_sensors,num_nodes,platform_name,arguments

def write_connections(f,num_sensors,num_nodes):
	f.write("\n\n 	<link id=\"1\" bandwidth=\"50MBps\" latency=\"50us\"/>\n\n")
	
	for i in range(num_sensors):
		for j in range(num_nodes):
			f.write("	<route src=\"Sensor-"+str(i)+"\" dst=\"Msq_node-"+str(j)+"\">\n 		<link_ctn id=\"1\"/>\n 	</route>\n")
	
	
def write_plat_file(num_sensors,num_nodes, platform_name):
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
	num_sensors,num_nodes,platform_name,arguments = get_arguments()
	
	write_plat_file(num_sensors,num_nodes, platform_name,)
	
	write_d_plat_file(num_sensors,num_nodes,platform_name,arguments)



if __name__ == "__main__":
	main()