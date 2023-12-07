#!/bin/bash

# Specify the folder containing the .tsp.gz files
input_folder="ALL_tsp"

# Specify the output folder
output_folder="./output"

# Create the output folder if it doesn't exist
mkdir -p "$output_folder"

# Iterate through each .tsp.gz file in the input folder
for file in "$input_folder"/*.tsp.gz; do
    # Extract the filename without extension
    filename=$(basename -- "$file")
    filename_no_ext="${filename%%.*}"

    # Create the output folder based on the prefix
    output_subfolder="$output_folder/$filename_no_ext"
    mkdir -p "$output_subfolder"

    # Unzip the .tsp file into the output folder
    gzip -d -c "$file" > "$output_subfolder/$filename_no_ext.tsp"

    # Check if the corresponding .opt.tour file exists and unzip it
    if [ -e "$input_folder/$filename_no_ext.opt.tour.gz" ]; then
        gzip -d -c "$input_folder/$filename_no_ext.opt.tour.gz" > "$output_subfolder/$filename_no_ext.opt.tour"
    fi
done
