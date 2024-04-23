#!/bin/bash

# Step 1: Extract input.tar.gz to the current directory
tar -xzvf input.tar.gz

# Step 2: Create and move .txt files to the TXT directory
mkdir -p TXT
find input/ -maxdepth 1 -type f -name "*.txt" -exec mv {} TXT/ \;

# Step 3: Create and move .jpg files to the JPG directory
mkdir -p JPG
find input/ -maxdepth 1 -type f -name "*.jpg" -exec mv {} JPG/ \;

# Step 4: Move the remaining files to the ZIP directory
mkdir -p ZIP
find input/ -maxdepth 1 -type f -exec mv {} ZIP/ \;

# Step 5: Compress and archive the ZIP directory
tar -czvf rest_zipped.tar.gz ZIP

# Clean up: Remove the extracted input directory
rm -r input

