#!/bin/bash
#
# Run experiment for HDF5 files
#
# echo "==================== Run write operation  ======================="
# mpirun -n 2 file_write 1 1024
# echo "==================== Run Read operation   ======================="
# mpirun -n 2 file_read
echo "==================== Run Append operation ======================="
mpirun -n 2 append 1 1024
# echo "==================== Run Read operation   ========================"
# mpirun -n 2 file_read