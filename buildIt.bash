#!/bin/bash
g++ -Wall -pthread -lpigpio ~/miscellany/main.cpp -o ~/a.out && cp ~/a.out ~/raz_follower
g++ -Wall -pthread -lpigpio ~/miscellany/stop_motors.c -o ~/stop_motors
