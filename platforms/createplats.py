import sys

file_start = "<?xml version=\'1.0\'?>\n<!DOCTYPE platform SYSTEM \"http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd\">\n<platform version=\"4.1\">\n  <AS id=\"AS1\" routing=\"Full\">"
file_end = "\n</platform>"


#Getting arguments
def get_arguments():
	arguments = sys.argv
	del(arguments[0])

	if len(arguments) < 3:
		print("Number of sensors,number of nodes and platform_name is needed.")
		exit()

	try:
		num_sensors = int(arguments[0])
		num_nodes = int(arguments[1])
		del(arguments[0])
		del(arguments[0])
	except:
		print("Number of sensors and number of nodes must be numbers.")

	platform_name = arguments[0]
	del(arguments[0])

	return num_sensors,num_nodes,platform_name,arguments


def write_plat_file(num_sensors,num_nodes, platform_name):
	if not platform_name.endswith(".xml"):
		platform_name = platform_name + ".xml"
	
	f = open(platform_name, "w")
	
	f.write(file_start)

	f.write(file_end)



	f.close()

def write_d_plat_file(num_sensors,num_nodes,platform_name,arguments):
	
	platform_name = "d_" + platform_name 
	
	if not platform_name.endswith(".xml"):
		platform_name = platform_name + ".xml"
	
	f = open(platform_name, "w")
	

	f.write(file_start)

	f.write(file_end)





	f.close()



def main():
	num_sensors,num_nodes,platform_name,arguments = get_arguments()
	
	write_plat_file(num_sensors,num_nodes, platform_name)
	
	write_d_plat_file(num_sensors,num_nodes,platform_name,arguments)



if __name__ == "__main__":
	main()