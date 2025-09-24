# CMake generated Testfile for 
# Source directory: /home/suraj/Downloads/Simulation/sumo
# Build directory: /home/suraj/Downloads/Simulation/sumo/build/cmake-build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(exampletest "/usr/bin/python3.10" "/home/suraj/Downloads/Simulation/sumo/docs/examples/runAll.py")
set_tests_properties(exampletest PROPERTIES  _BACKTRACE_TRIPLES "/home/suraj/Downloads/Simulation/sumo/CMakeLists.txt;601;add_test;/home/suraj/Downloads/Simulation/sumo/CMakeLists.txt;0;")
subdirs("src")
subdirs("unittest")
