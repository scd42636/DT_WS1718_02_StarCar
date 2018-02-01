#!/bin/bash

# A script frequently getting files using psftp

# $0 local path

cd /home/pi/SensorOutput
lcd D:\DT\FinalRun

mget "BeschleunigungX.txt" "BeschleunigungY.txt" "Compass.txt" "Lidar.txt" "UltraHinten.txt" "UltraVorne.txt"
quit


