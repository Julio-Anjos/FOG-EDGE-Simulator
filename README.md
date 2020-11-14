# FogSimulator
Simgrid simulation of fog stream processing on a big data scenario.

# How to use
Please use a version of simgrid 3.22 or higher.

You can execute a example run using the following command:

```
./compile_run.sh config/plat_config.txt config/burst_config.txt
```
The results of the simulation will be displayed on screen, and more specific details will be stored on the result_logs/ file.

After that, you can build your own platform configs and burst configs, following the examples found in the config/ file.


# How to compile

If you wish to just compile the simulator without running a simulation and using the script, you must:

```
cmake .
make
```

# How to run

If you wish to run without using the script, you must compile first and create your platform files on platforms/ , and then follow the example:

```
./main platforms/test_plat.xml platforms/d_test_plat.xml config/burst_config.txt 
```
