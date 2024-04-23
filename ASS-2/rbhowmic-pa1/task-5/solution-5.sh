#!/bin/bash

# Extract the contents of the tar file
tar -xvf sample_data.tar

# Create directories for "smart" and "OS" files
mkdir -p smart OS

# Create an empty result.txt file
> result.txt

# Loop through each file in the sample_data directory
for file in sample_data/*; do
    if [ -f "$file" ]; then
        # Count occurrences of 'smart' and 'operating system' in the file
        smart_count=$(grep -o -i 'smart' "$file" | wc -l)
        os_count=$(grep -o -i 'operating system' "$file" | wc -l)

        # Get the filename without the path
        filename=$(basename "$file")

        # Append the counts to result.txt
        printf "%s smart %d\n" "$filename" "$smart_count" >> result.txt
        printf "%s operating system %d\n" "$filename" "$os_count" >> result.txt

        # Copy the file to the 'smart' directory if 'smart' is found
        if [ "$smart_count" -gt 0 ]; then
            cp "$file" smart/
        fi

        # Copy the file to the 'OS' directory if 'operating system' is found
        if [ "$os_count" -gt 0 ]; then
            cp "$file" OS/
        fi
    fi
done

