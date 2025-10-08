
   
# projectLoRaWAN

This project is an ns-3 module that can be used to perform simulations of a LoRaWAN network, and it is based in the initial version of this code was developed as part of a master's thesis at the University of Padova (https://github.com/signetlabdei/lorawan), under the supervision of Prof. Lorenzo Vangelista, Prof. Michele Zorzi and with the help of Marco Centenaro.

## Getting Started ##

This section is aimed at getting a user to a working state starting with a machine that may never have had LoRaWAN-NS3 installed. It covers prerequisites, ways to obtain LoRaWAN-NS3, ways to build LoRaWAN-NS3, and ways to verify your build and run LoRaWAN program.

### Prerequisites ###
```
Make sure that your system has these prerequisites.  Download can be via either ``git`` or via
source archive download (via a web browser, ``wget``, or ``curl``).

+--------------------+--------------------------------------+------------------------------+
| **Purpose**        | **Tool**                             | **Minimum Version**          |
+====================+==================+===================+==============================+
| **Download**       | ``git`` (for Git download)           | No minimum version           |
+                    +                                      +                              +
|                    | or ``tar`` and ``bunzip2``           | No minimum version           |
|                    | (for Web download)                   |                              |
+--------------------+--------------------------------------+------------------------------+
| **Compiler**       | ``g++``                              | >= 10                        |
+                    +                                      +                              +
|                    | or ``clang++``                       | >= 11                        |
+--------------------+--------------------------------------+------------------------------+
| **Configuration**  | ``python3``                          | >= 3.8                       |
+--------------------+--------------------------------------+------------------------------+
| **Build system**   | ``cmake``,                           | >= 3.13                      |
+                    +                                      +                              +
|                    | and at least one of:                 | No minimum version           |
|                    | ``make``, ``ninja``, or ``Xcode``    |                              |
+--------------------+--------------------------------------+------------------------------+

.. note::

  If you are using an older version of ns-3, other tools may be needed (such as
  ``python2`` instead of ``python3`` and ``Waf`` instead of ``cmake``).  Check the file
  ``RELEASE_NOTES`` in the top-level directory for requirements for older releases.

From the command line, you can check the version of each of the above tools with version
requirements as follows:

+--------------------------------------+------------------------------------+
| **Tool**                             | **Version check command**          |
+======================================+====================================+
| ``g++``                              | ``$ g++ --version``                |
+--------------------------------------+------------------------------------+
| ``clang++``                          | ``$ clang++ --version``            |
+--------------------------------------+------------------------------------+
| ``python3``                          | ``$ python3 -V``                   |
+--------------------------------------+------------------------------------+
| ``cmake``                            | ``$ cmake --version``              |
+--------------------------------------+------------------------------------+
```

### Downloading LoRaWAN-NS3 ###

Let’s assume that you, as a user, wish to build LoRaWAN-NS3 in a local directory called Workspace. If you adopt the Workspace directory approach, you can get a copy of a release by typing the following into your Linux shell:
```
$ git clone https://github.com/heldercs/projectLoRaWAN
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
