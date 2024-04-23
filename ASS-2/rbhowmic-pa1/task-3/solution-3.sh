#!/bin/bash

totalProcessors=$(grep -c '^processor' /proc/cpuinfo)
# Get core ids and cache sizes
coreIDs=$(grep -oP '^core id\s+:\s+\K\d+' /proc/cpuinfo)
cacheSizes=$(grep -oP '^cache size\s+:\s+\K\d+\s\w+' /proc/cpuinfo)
# Printing the data in cpuinfo.txt
echo "Num of processors in the system (Total) :  $totalProcessors" > cpuinfo.txt
echo "Core IDs in the system:  $coreIDs" >> cpuinfo.txt
echo "Cache sizes in the system :  $cacheSizes" >> cpuinfo.txt
