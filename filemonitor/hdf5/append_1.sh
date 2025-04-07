#!/bin/bash
#
# Run experiment for HDF5 files
#
echo "==================== Run write operation rank=1 count= 1 size=1024  ======================="
mpirun -n 1 file_write 1 1024
echo "==================== Run append operation rank=1 count= 1 size=1024  ======================="
mpirun -n 1 append 1 1024
echo "==================== Run append operation rank=1 count= 2 size=1024  ======================="
mpirun -n 1 append 2 1024
echo "==================== Run append operation rank=1 count= 4 size=1024  ======================="
mpirun -n 1 append 4 1024
echo "==================== Run append operation rank=1 count= 8 size=1024  ======================="
mpirun -n 1 append 8 1024
echo "==================== Run append operation rank=1 count= 16 size=1024  ======================="
mpirun -n 1 append 16 1024
echo "==================== Run append operation rank=1 count= 32 size=1024  ======================="
mpirun -n 1 append 32 1024
echo "==================== Run append operation rank=1 count= 64 size=1024  ======================="
mpirun -n 1 append 64 1024
echo "==================== Run append operation rank=1 count= 128 size=1024  ======================="
mpirun -n 1 append 128 1024
echo "==================== Run append operation rank=1 count= 256 size=1024  ======================="
mpirun -n 1 append 256 1024
echo "==================== Run append operation rank=1 count= 512 size=1024  ======================="
mpirun -n 1 append 512 1024
echo "==================== Run append operation rank=1 count= 1024 size=1024  ======================="
mpirun -n 1 append 1024 1024



