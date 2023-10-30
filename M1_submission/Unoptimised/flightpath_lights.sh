#!/bin/bash
#SBATCH --job-name=flightpath
#SBATCH --partition=cosc
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=20G
#SBATCH --time=0-02:59 # time (D-HH:MM)

# Define the range of values for N
start_N=50
end_N=35000
step_N=2

# Specify the source file to edit
source_file="setup_draft.h"

# Store the last line in a variable
last_line=$(tail -n 1 "$source_file")

# Loop through the values of N
for ((N = start_N; N <= end_N; N *= step_N)); do
    # Create a temporary file for editing
    temp_file="${source_file}.temp"
    touch "$temp_file"

    # Initialize a flag to check if the #define N line has been replaced
    define_L_replaced=0

    # Iterate through each line in the source file
    while IFS= read -r line; do
        # Check if the line contains the #define N statement
        if [[ "$line" == "#define L "* ]]; then
            if [ "$define_L_replaced" -eq 0 ]; then
                # Replace the existing N value with the current N value
                echo "#define L $N // number of lights" >> "$temp_file"
                define_L_replaced=1
            else
                # Skip the original #define N line if it has already been replaced
                continue
            fi
        else
            # Copy the line as is
            echo "$line" >> "$temp_file"
        fi
    done < "$source_file"

    # Overwrite the original source file with the edited contents
    mv "$temp_file" "$source_file"

    # Compile or perform other actions with the updated source file here
    # For example, you can use a C compiler to compile the source file
    # gcc -o your_program "$source_file"
    # ./your_program

    make
    time ./flightpath_draft

    echo "Edited $source_file with N=$N"
done

# Append the last line to the source file
echo "$last_line" >> "$source_file"

echo "Script completed."
