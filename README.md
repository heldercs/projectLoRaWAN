# projectLoRaWAN

This project is an ns-3 module that can be used to perform simulations of a LoRaWAN network, and it is based in the initial version of this code was developed as part of a master's thesis at the University of Padova (https://github.com/signetlabdei/lorawan), under the supervision of Prof. Lorenzo Vangelista, Prof. Michele Zorzi and with the help of Marco Centenaro.

## Getting Started ##

This section is aimed at getting a user to a working state starting with a machine that may never have had LoRaWAN-NS3 installed. It covers prerequisites, ways to obtain LoRaWAN-NS3, ways to build LoRaWAN-NS3, and ways to verify your build and run LoRaWAN program.

### Prerequisites ###
```
$ sudo apt-get update
$ sudo apt-get -y install gcc g++ python gcc g++ python python-dev qt4-dev-tools libqt4-dev mercurial \
bzr cmake libc6-dev libc6-dev-i386 g++-multilib gdb valgrind gsl-bin libgsl2 libgsl2:i386 flex bison \
libfl-dev tcpdump sqlite sqlite3 libsqlite3-dev libxml2 libxml2-dev libgtk2.0-0 libgtk2.0-dev \
vtun lxc git
```

### Downloading LoRaWAN-NS3 ###

Let’s assume that you, as a user, wish to build LoRaWAN-NS3 in a local directory called Workspace. If you adopt the Workspace directory approach, you can get a copy of a release by typing the following into your Linux shell:
```
$ git clone https://github.com/heldercs/projectLoRaWANNS3
```

### Building LoRaWAN-NS3 ###

To configure the LoRaWAN-NS3 you will need to execute the following commands:
```
$ ./waf configure --build-profile=debug --enable-examples --enable-tests
```
The build system is now configured and you can build the debug versions of the LoRaWAN-NS3 programs by simply typing
```
$ ./waf
```

### Testing LoRaWAN-NS3 ###

You can run the unit tests of the ns-3 distribution by running the ./test.py -c core script:
```
$ ./test.py -c core
```
These tests are run in parallel by Waf. You should eventually see a report saying that
```
246 of 249 tests passed (246 passed, 3 skipped, 0 failed, 0 crashed, 0 valgrind errors)
```
### Running the Script ###

We typically run scripts under the control of Waf. This script allows the build system to ensure that the shared library paths are set correctly and that the libraries are available at run time. To run the applications LoRa program, to run the runSimulator shell script by typing the following:
```
$ ./runSimulator.sh gwRing rad gwRad simTime interval pEDs
```
in which the above parameters are defined as:
```
* App      - Applications options 
* gwRing   - Number of gateway rings to include;
* rad      - The radius of the area to simulate;
* gwRad    - The distance between two gateways;
* simTime  - The time for which to simulate;
* interval - The period in seconds to be used by periodically transmitting applications;
* trial    - The results directory; 
* pED      - Whether or not to print a file containing the ED's positions;
```
Applications Options (App):
```
* 0      - lorawan Application base 
* 1      - lorawan Application multiClass
* 2      - lorawan Application Regular and Alarm 
* 3      - lorawan Application  dualClass

```
The results will be placed in TestReult:
```
$ ls TestResult/
$ test0 test1 test2 testX
```
where X is trial's options
