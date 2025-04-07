/* HDF5 Dataset example, collective  I/O */

/* System header files */
#include <assert.h>
#include <stdlib.h>
#include <time.h>   // for time()
#include <unistd.h> // for sleep()
#include<string.h>

/* HDF5 header file */
#include "hdf5.h"

/* Predefined names and sizes */
#define FILENAME "/media/volume/data/file/test1.h5"
//#define FILENAME "../files/test1.h5"
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
    herr_t ret;         	/* Generic return value */
    double time1,time2,duration;
    // double after_hyperslab,after_read_data,after_write_buf;
    int i,j; 
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

    //collect output in a file
    FILE *file; // Declare a file pointer
    //char data[100];


    char filename[100];
    char filename1[10];
    strcpy(filename,"output_blockchain/read/test2/read2_");
    sprintf(filename1, "%d", mpi_size);
    strcat(filename,filename1);
    strcat(filename,".txt");

    // Open the file for writing
    file = fopen(filename, "a+");

    // Check if the file was opened successfully
    if (file == NULL) {
        printf("File could not be opened.\n");
        return 1;
    }

   /* Create an HDF5 file access property list */
    fapl_id = H5Pcreate (H5P_FILE_ACCESS);
    assert(fapl_id > 0);

    /* Set file access property list to use the MPI-IO file driver */
    ret = H5Pset_fapl_mpio(fapl_id, MPI_COMM_WORLD, MPI_INFO_NULL);
    assert(ret >= 0);

    
    
    // Open the HDF5 file
    file_id = H5Fopen(FILENAME, H5F_ACC_RDWR, fapl_id);

    if (file_id < 0) {
        // Handle error opening the file
        fprintf(stderr, "Failed to open file: %s\n", FILENAME);
        return 1;
    }
     /* Release file access property list */
    ret = H5Pclose(fapl_id);
    assert(ret >= 0);

    /* Set up the collective transfer properties list */
    dxpl_id = H5Pcreate(H5P_DATASET_XFER);
    assert(dxpl_id > 0);
    ret = H5Pset_dxpl_mpio(dxpl_id, H5FD_MPIO_COLLECTIVE);
    assert(ret >= 0);


    // Open a dataset within the file
    dset_id = H5Dopen(file_id, DATASETNAME, H5P_DEFAULT);
    assert(dset_id > 0);

    // Get the dataspace ID for the dataset
    file_space_id = H5Dget_space(dset_id);
    if (file_space_id < 0) {
        printf("Failed to get dataspace ID.\n");
    }

    // Get the dimensions of the dataspace.
    //H5Sget_simple_extent_dims
    H5Sget_simple_extent_dims(file_space_id, dims,NULL);

   
    if(mpi_rank==0){
        file_size=dims[0]*dims[1]*4;// bytes -- int is 4 bytes
        file_size/=1024; //for KB
        file_size/=1024; //for MB
        fprintf(file,"File dimensions %lldX%lld\n",dims[0],dims[1]);
        fprintf(file,"File size %lld MB\n",file_size);
        // printf("File dimensions %lldX%lld\n",dims[0],dims[1]);
        // printf("File size %lld MB\n",file_size);
    }
     
    

    rows=dims[0];
    cols=dims[1];
   

    count=rows;
    size=cols;
    //rows=rows*X; // X is a constant. int is 4 bytes. so 4 bytes*256=1024KB

    rows=rows/mpi_size;
    //int old_rows=dims[0];
    //int old_cols=dims[1];
    // Resize the memory dataspace to indicate the new size of our buffer
    // The second buffer is three lines long
    mem_dims[0]=rows*cols;
    mem_space_id = H5Screate_simple(1, mem_dims, NULL);
    assert(mem_space_id > 0);



time1 = MPI_Wtime();
    // printf("- Before file space id hyperslab selected\n");
// Select hyperslab on file dataset
    //file_space_id = H5Dget_space(dset_id);
    file_start[0] = rows*mpi_rank;
    file_start[1] = 0;
    file_count[0] = rows;
    file_count[1] = cols;
    H5Sselect_hyperslab(file_space_id, H5S_SELECT_SET, file_start, NULL, file_count, NULL);

//    /* Select all elements in the memory buffer */
    mem_start[0] = 0;
    mem_count[0] = rows*cols;
    // printf("rank=%d mem_start=%lld file_count=%lld\n",mpi_rank,mem_start[0],mem_count[0]);

    ret = H5Sselect_hyperslab(mem_space_id, H5S_SELECT_SET, mem_start, NULL, mem_count, NULL);
    assert(ret >= 0);
    // after_hyperslab=MPI_Wtime();
    // printf(" After hyperslab Runtime is  %f \n",after_hyperslab-time1);
    // printf("- After mem space id hyperslab selected\n");
//    /* Initialize each column with mpi rank value */
    write_buf = (int *)malloc(sizeof(int)*cols*rows);
    // after_write_buf=MPI_Wtime();
    // printf(" After write buf Runtime is  %f \n",after_write_buf-after_hyperslab);

//     // Write buffer from dataset
//     // mem_space and file_space should now have the same number of elements selected
//     // note that buffer and &b[0][0] are equivalent
    ret = H5Dread(dset_id, H5T_NATIVE_INT, mem_space_id, file_space_id,
	    dxpl_id, write_buf);
    assert(ret >= 0);
    // after_read_data=MPI_Wtime();
    // printf("After Read Data  Runtime is  %f \n",after_read_data-after_write_buf);
    time2 = MPI_Wtime();
    if (mpi_rank == 0) {
        duration = time2 - time1;
       // printf(" rank size=%d count=%d size=%d Runtime is  %f \n",mpi_size,count/mpi_size,size, duration);

        fprintf(file," rank size=%d count=%d size=%d Runtime is  %f \n",mpi_size,count/mpi_size,size, duration);

    }
    //if(mpi_rank==0){
        for(i=0;i<rows*cols;i++){
            if((i+mpi_rank*rows*cols)!=write_buf[i])
                printf("rank=%d data=%d \n",mpi_rank,write_buf[i]);
        }
        
    //}
    
    // Close the output file
    fclose(file);
    free(write_buf);
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

