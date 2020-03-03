import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import sys
import matplotlib.patches as mpatches
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



plt.subplot(2, 1, 1)
sensor_id=[]
start=0
end=0
pkgs=0
red_printed = False
#Plotting number of packages sent at specific time
for d in data:
    if len(d) == 5:




    if len(d) == 2:











    first_sensor = d[0] #For now, displaying only a single sensor, due to all of the sensors connected to the same MSQ_node doing the same thing
    
    
    if d[0] == first_sensor: 
        if len(d) == 5: 
            if not red_printed:
                plt.plot([start,end],[pkgs,pkgs],"b")
            
            red_printed= False
            
            start = d[3]
            end = d[4]
            pkgs = d[1]

            
        if len(d) ==2:
            plt.plot([start,end],[pkgs,pkgs],"r")  
            red_printed = True

blue_patch = mpatches.Patch(color='blue', label='Sucess sending all packages')
red_patch = mpatches.Patch(color='red', label='Missed some packages')
plt.legend(handles=[blue_patch,red_patch])


plt.title('Sensor_Msq Stream Packages Sent at Specific Time')
plt.xlabel('time (s)'), 
plt.ylabel('num packages sent at time')




plt.subplot(2, 1, 2)

#Plotting total number of packages sent 
expected_sum = 0
missed_sum=0

time = []
expected_pkgs=[]
sent_pkgs=[]
for d in data:
    first_sensor = d[0] #For now, displaying only a single sensor, due to all of the sensors connected to the same MSQ_node doing the same thing
    
    
    if d[0] == first_sensor: 
        if len(d) == 5: 
            start = d[3]
            end = d[4]
            pkgs = d[1]
            time.append((end+start)/2)
            expected_sum += pkgs
            missed_sum +=pkgs
            expected_pkgs.append(expected_sum)
            sent_pkgs.append(missed_sum)

        if len(d) ==2:
            missed_pkgs = d[1]
            missed_sum -= missed_pkgs
            sent_pkgs[-1] -= missed_pkgs    


blue_patch = mpatches.Patch(color='blue', label='Expected Total Sum of Packages')
red_patch = mpatches.Patch(color='red', label='Obtained Total Sum of Packages')
plt.legend(handles=[blue_patch,red_patch])


plt.plot(time,expected_pkgs,"b")
#plt.plot(time,sent_pkgs,"r")
plt.title('Sensor_Msq Stream Total Amount of Packages')
plt.xlabel('time (s)'), 
plt.ylabel('total packages sent')
plt.grid()
plt.show()
