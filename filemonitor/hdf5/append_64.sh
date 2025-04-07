#!/bin/bash
#
# Run experiment for HDF5 files
#
echo "==================== Run write operation rank=64 count= 1 size=1024  ======================="
mpirun -n  64 file_write 1 1024
echo "==================== Run append operation rank=64 count= 1 size=1024  ======================="
mpirun -n 64  append 1 1024
echo "==================== Run append operation rank=64 count= 2 size=1024  ======================="
mpirun -n 64  append 2 1024
echo "==================== Run append operation rank=64 count= 4 size=1024  ======================="
mpirun -n 64  append 4 1024
echo "==================== Run append operation rank=64 count= 8 size=1024  ======================="
mpirun -n 64  append 8 1024
echo "==================== Run append operation rank=64 count= 16 size=1024  ======================="
mpirun -n 64  append 16 1024
echo "==================== Run append operation rank=64 count= 32 size=1024  ======================="
mpirun -n 64  append 32 1024
echo "==================== Run append operation rank=64 count= 64 size=1024  ======================="
mpirun -n 64  append 64 1024
echo "==================== Run append operation rank=64 count= 128 size=1024  ======================="
mpirun -n 64  append 128 1024
echo "==================== Run append operation rank=64 count= 256 size=1024  ======================="
mpirun -n 64  append 256 1024
echo "==================== Run append operation rank=64 count= 512 size=1024  ======================="
mpirun -n 64  append 512 1024
echo "==================== Run append operation rank=64 count= 1024 size=1024  ======================="
mpirun -n 64  append 1024 1024



