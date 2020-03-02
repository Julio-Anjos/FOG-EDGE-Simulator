import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import sys

#Getting path of the config
def get_file():
    arguments = sys.argv
    del(arguments[0])

    if not arguments:
        raise Exception("Please include path to a Msq_sensor_stream log file.")


    return arguments[0] #Config file name



f = open(get_file())




#For every line in the files, gets only the numbers. (2 numbers means a failure alert and 5 means a start of division alert)
data=[]
numbers = []
for line in f:
	if line.startswith("Sensor-"):
	 	line = line[7:]
		for t in line.split():
		    try:
		        numbers.append(float(t))
		    except ValueError:
		        pass
		data.append(numbers)
		numbers = []

x=[]
y=[]
fig, ax = plt.subplots()
for d in data:
	first_sensor = d[0] #For now, displaying only a single sensor, due to all of the sensors connected to the same MSQ_node doing the same thing
	
	if len(d) == 5:
		if d[0] == first_sensor:  
			ax.plot([d[3],d[4]],[d[1],d[1]])
		
		



ax.set(xlabel='time (s)', ylabel='num packages sent at time',
       title='Sensor_Msq Stream Plotting')
ax.grid()
fig.savefig("test.png")
plt.show()
