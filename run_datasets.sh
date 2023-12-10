#!/bin/bash

# Specify the compiled C++ executable
cpp_executable="./tp2_stable"

# Specify the output file
output_file="run_output.txt"

# List of dataset names
datasets=(
  "eil51" "berlin52" "st70" "eil76" "pr76" "rat99"
  "kroA100" "kroB100" "kroC100" "kroD100" "kroE100"
  "rd100" "eil101" "lin105" "pr107" "pr124" "bier127"
  "ch130" "pr136" "pr144" "ch150" "kroA150" "kroB150"
  "pr152" "u159" "rat195" "d198" "kroA200" "kroB200"
  "ts225" "tsp225" "pr226" "gil262" "pr264" "a280" "pr299"
  "lin318" "linhp318" "rd400" "fl417" "pr439" "pcb442"
  "d493" "u574" "rat575" "p654" "d657" "u724" "rat783"
  "pr1002" "u1060" "vm1084" "pcb1173" "d1291" "rl1304"
  "rl1323" "nrw1379" "fl1400" "u1432" "fl1577" "d1655"
  "vm1748" "u1817" "rl1889" "d2103" "u2152" "u2319" "pr2392"
  "pcb3038" "fl3795" "fnl4461" "rl5915" "rl5934" "rl11849"
  "usa13509" "brd14051" "d15112" "d18512"
)

# Function to run the C++ code for a given dataset
run_dataset() {
  dataset_name="$1"
  echo "Running $dataset_name..."
  "$cpp_executable" "$dataset_name" >> "$output_file" 2>&1
  echo "$dataset_name finished"
  echo "___________________________________________" >> "$output_file"
}

# Execute the C++ code for each dataset
for dataset in "${datasets[@]}"; do
  run_dataset "$dataset"
done