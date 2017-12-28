#!/bin/bash
g++ -Wall -pthread -lpigpio ~/miscellany/main.cpp -o ~/a.out && cp ~/a.out ~/raz_follower
g++ -Wall -pthread -lpigpio ~/miscellany/stop_motors.c -o $PWD/motors_stop
g++ -Wall -pthread -lpigpio ~/miscellany/forward.cpp -o $PWD/motors_forward
g++ -Wall -pthread -lpigpio ~/miscellany/backward.cpp -o $PWD/motors_backward
g++ -Wall -pthread -lpigpio ~/miscellany/commandLoop.cpp -o ~/commandLoop


chmod +x motors_*


