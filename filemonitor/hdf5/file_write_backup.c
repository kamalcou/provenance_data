/* HDF5 Dataset example, collective  I/O */

/* System header files */
#include <assert.h>
#include <stdlib.h>
#include <time.h>   // for time()
#include <unistd.h> // for sleep()

/* HDF5 header file */
#include "hdf5.h"

/* Predefined names and sizes */
#define FILENAME "../files/test1.h5"
#define DATASETNAME "dataset1"
#define RANK    2
//#define rows    (230 * 1000)
// #define rows    (5)
// #define cols   4       /* Should be same as MPI rank */

/* Global variables */
int *write_buf;

int main(int argc, char *argv[])
{
    hid_t file_id;              /* File ID */
    hid_t fapl_id;		/* File access property list */
    hid_t dset_id;		/* Dataset ID */
    hid_t file_space_id;	/* File dataspace ID */
    hid_t mem_space_id;		/* Memory dataspace ID */
    hid_t dxpl_id;		/* Data transfer propserty ID */
    hsize_t file_dims[RANK];   	/* Dataset dimemsion sizes */
    hsize_t mem_dims[1];   	/* Memory buffer dimemsion sizes */
    hsize_t file_start[RANK];	/* File dataset selection start coordinates (for hyperslab setting) */
    hsize_t file_count[RANK];	/* File dataset selection count coordinates (for hyperslab setting) */
    hsize_t mem_start[1];	/* Memory buffer selection start coordinates (for hyperslab setting) */
    hsize_t mem_count[1];	/* Memory buffer selection count coordinates (for hyperslab setting) */
    
    
    int mpi_size, mpi_rank;	/* MPI variables */
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Info info = MPI_INFO_NULL;
    herr_t ret;         	/* Generic return value */
    double time1,time2,duration;
    int i; 
    int rows,cols; 
    int count,size; 
    hsize_t dims[2];                   /* Loop index */ 
    long long int file_size=0;
    
    hsize_t     X = 256;  // X is a constant. int is 4 bytes. so 4 bytes*256=1024B=1KB
    //hsize_t     X = 1;  // X is a constant. int is 4 bytes. so 4 bytes*256=1024B=1KB

    /* Initialize MPI */
    MPI_Init(&argc, &argv);

    /* Gather information about MPI comm size and my rank */
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    //printf("my rank = %d (%d)\n", mpi_rank, mpi_size);
    long    arg1, arg2;
    if (argc == 3) {
        char *p;

        arg1 = strtol(argv[1], &p, 10);
        arg2 = strtol(argv[2], &p, 10);
        rows = arg1;
        //FILE="sds.h5";


        cols = arg2;
    }
    else {

        printf("Need two values for size and counter values\n");
        return 0;
    }

    count=rows;
    size=cols;
    rows=rows*X; // X is a constant. int is 4 bytes. so 4 bytes*256=1024KB


    /* Iniialize buffer of dataset to write */
    /* (in a real application, this would be your science data) */
    /* <SKIPPED> */

    /* Create an HDF5 file access property list */
    fapl_id = H5Pcreate (H5P_FILE_ACCESS);
    assert(fapl_id > 0);

    /* Set file access property list to use the MPI-IO file driver */
    ret = H5Pset_fapl_mpio(fapl_id, MPI_COMM_WORLD, MPI_INFO_NULL);
    assert(ret >= 0);

    /* Create the file collectively */
    file_id = H5Fcreate(FILENAME, H5F_ACC_TRUNC, H5P_DEFAULT, fapl_id);
    assert(file_id > 0);

    /* Release file access property list */
    ret = H5Pclose(fapl_id);
    assert(ret >= 0);
    

    /* Set up the collective transfer properties list */
    dxpl_id = H5Pcreate(H5P_DATASET_XFER);
    assert(dxpl_id > 0);
    ret = H5Pset_dxpl_mpio(dxpl_id, H5FD_MPIO_COLLECTIVE);
    assert(ret >= 0);



   // int rows=rows;
   // printf("row by rank %d\n",rows);
    /* Define a file dataspace the dimensions of the dataset */
    file_dims[0] = rows*mpi_size;
    file_dims[1] = cols;
    hsize_t max_dims[2]={H5S_UNLIMITED, cols};
    file_space_id = H5Screate_simple(RANK, file_dims, max_dims);
    assert(file_space_id > 0);
    
    file_size=file_dims[0]*file_dims[1]*4;// bytes int is 4 bytes
    file_size/=1024; //for KB
    file_size/=1024; //for MB
     if(mpi_rank==0){
         printf("File dimensions %lldX%lld\n",file_dims[0],file_dims[1]);
         printf("File size %lld MB\n",file_size);
     }
     
    // Create a dataset creation property list
    // The layout of the dataset have to be chunked when using unlimited dimensions
    hid_t plist = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_layout(plist, H5D_CHUNKED);
    // The choice of the chunk size affects performances
    // This is a toy example so we will choose one line
    hsize_t chunk_dims[RANK] = {rows, cols};
    H5Pset_chunk(plist, RANK, chunk_dims);
    //printf("- Property list created\n");

    /* Create the dataset collectively */
    dset_id = H5Dcreate2(file_id, DATASETNAME, H5T_NATIVE_INT,
        file_space_id, H5P_DEFAULT, plist, H5P_DEFAULT);
    assert(dset_id > 0);


    /* Create memory dataspace for write buffer */
    mem_dims[0] = cols*rows;
    mem_space_id = H5Screate_simple(1, mem_dims, NULL);
    assert(mem_space_id > 0);

    time1 = MPI_Wtime();
    
    /* Select column of elements in the file dataset */
    file_start[0] = mpi_rank*rows;
    file_start[1] = 0;
    file_count[0] = rows;
    file_count[1] = cols;
    //  printf("MPI rank=%d Hyperslab operation on filespace using offset  %llux%llu and count %llux%llu \n",
    //            mpi_rank, file_start[0], file_start[1], file_count[0], file_count[1]);

    ret = H5Sselect_hyperslab(file_space_id, H5S_SELECT_SET, file_start, NULL, file_count, NULL);
    assert(ret >= 0);

    /* Select all elements in the memory buffer */
    mem_start[0] = 0;
    mem_count[0] = cols*rows;
    
    ret = H5Sselect_hyperslab(mem_space_id, H5S_SELECT_SET, mem_start, NULL, mem_count, NULL);
    assert(ret >= 0);

    /* Initialize each column with mpi rank value */
    write_buf = (int *)malloc(sizeof(int)*cols*rows);
    for (i=0; i<cols*rows; i++) {
        write_buf[i] = mpi_rank;
    }
    
  //printf("cols*rows=%d\n",cols*rows);
    /* Write data independently */
    // if(mpi_rank==1)
    // {

    //      for (i=0; i<cols*rows; i++) {
    //          printf("%.1lf ", write_buf[i]);
    //      }
    // }
    ret = H5Dwrite(dset_id, H5T_NATIVE_INT, mem_space_id, file_space_id,
	    dxpl_id, write_buf);
    assert(ret >= 0);

    MPI_Barrier(comm);
    time2 = MPI_Wtime();
    if (mpi_rank == 0) {
        duration = time2 - time1;
        printf(" rank size=%d count=%d size=%d Runtime is  %f \n",mpi_size,rows*mpi_size,size, duration);

    }
    
    free(write_buf);
    

//    
/*-----------------------------*/
/*-----------------------------*/
    /* Close memory dataspace */
    ret = H5Sclose(mem_space_id);
    assert(ret >= 0);

    /* Close file dataspace */
    ret = H5Sclose(file_space_id);
    assert(ret >= 0);

    /* Close dataset collectively */
    ret = H5Dclose(dset_id);
    assert(ret >= 0);

   

    

    /* MPI_Finalize must be called AFTER any HDF5 call which may use MPI calls */
    MPI_Finalize();

    return(0);
}

