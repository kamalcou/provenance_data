#!/bin/bash
#
# Run experiment for HDF5 files
#


echo "==================== Run write operation rank=4 count= 1 size=1024  ======================="
mpirun -n 4 file_write 1 1024


echo "==================== Run Read operation rank=4 count= 1 size=1024  ======================="
mpirun -n 4 file_read


echo "==================== Run write operation rank=4 count= 2 size=1024 ======================="
mpirun -n 4 file_write 2 1024


echo "==================== Run Read operation rank=4 count= 2 size=1024  ======================="
mpirun -n 4 file_read


echo "==================== Run write operation rank=4 count= 4 size=1024 ======================="
mpirun -n 4 file_write 4 1024


echo "==================== Run Read operation  rank=4 count= 4 size=1024 ======================="
mpirun -n 4 file_read


echo "==================== Run write operation rank=4 count= 8 size=1024 ======================="
mpirun -n 4 file_write 8 1024


echo "==================== Run Read operation rank=4 count= 8 size=1024  ======================="
mpirun -n 4 file_read


echo "==================== Run write operation rank=4 count= 16 size=1024 ======================="
mpirun -n 4 file_write 16 1024


echo "==================== Run Read operation rank=4 count= 16 size=1024  ======================="
mpirun -n 4 file_read


echo "==================== Run write operation rank=4 count= 32 size=1024 ======================="
mpirun -n 4 file_write 32 1024


echo "==================== Run Read operation  rank=4 count= 32 size=1024 ======================="
mpirun -n 4 file_read


echo "==================== Run write operation rank=4 count= 64 size=1024 ======================="
mpirun -n 4 file_write 64 1024


echo "==================== Run Read operation  rank=4 count= 64 size=1024 ======================="
mpirun -n 4 file_read


echo "==================== Run write operation rank=4 count= 128 size=1024 ======================="
mpirun -n 4 file_write 128 1024


echo "==================== Run Read operation  rank=4 count= 128 size=1024 ======================="
mpirun -n 4 file_read


echo "==================== Run write operation rank=4 count= 256 size=1024 ======================="
mpirun -n 4 file_write 256 1024


echo "==================== Run Read operation  rank=4 count= 256 size=1024 ======================="
mpirun -n 4 file_read


echo "==================== Run write operation rank=4 count= 512 size=1024 ======================="
mpirun -n 4 file_write 512 1024


echo "==================== Run Read operation  rank=4 count= 512 size=1024 ======================="
mpirun -n 4 file_read


echo "==================== Run write operation rank=4 count= 1024 size=1024 ======================="
mpirun -n 4 file_write 1024 1024


echo "==================== Run Read operation  rank=4 count= 1024 size=1024 ======================="
mpirun -n 4 file_read
