#!/bin/bash

# Create Assignment-1 directory
mkdir Assignment-1

# Create Query directories and response files
for i in {1..10}
do
    query_dir="Assignment-1/Query-$i"
    response_file="response-$i.sh"
    
    # Create Query directory
    mkdir "$query_dir"
    
    # Create response file inside Query directory
    touch "$query_dir/$response_file"
done
