#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

int main() {
    FILE *file;
     
    
    char search_text[] = "Runtime is  "; // Replace with the text you want to search for
    char line[100]; // Adjust the buffer size as needed
    char *f;
    int i,j;

    DIR *directory;
    struct dirent *entry;
    
    // Specify the directory path
    char *path = "../test2";
    char csv_name[100];
    strcpy(csv_name,path);
    strcat(csv_name,"/");
    strcat(csv_name,"w_provenance_write_test2.csv");
     // Substitute the file_path string
    // with full path of CSV file
    FILE* fp = fopen(csv_name, "a+");

    // Open the directory
    directory = opendir(path);

    if (directory == NULL) {
        perror("Error opening directory");
        return 1;
    }

    // Read and print the names of all files in the directory
    // 
        char filename[10][100];
        j=0;
        for(i=1;i<=64;i=i*2){
            j++;
            strcpy(filename[j],"write2_");
            char *c;
            if(i<10){ 
            //char c[2];
            c=(char *)malloc(sizeof(char)*2);
            c[0]='0'+i;
            c[1]='\0';
            }else{
                //char c[3];
                c=(char *)malloc(sizeof(char)*3);
                c[2]='\0';
                c[1]='0'+i%10;
                c[0]='0'+i/10;
            }
           
            strcat(filename[j],c);
            printf("%s\n",filename[j]);
            
        //}
//while ((entry = readdir(directory)) != NULL) {
      // if (entry->d_type == DT_REG) {
        
            
            char filename1[100]; // Replace with your file name
            // printf("File: %s\n", entry->d_name);
            strcpy(filename1,path);
            strcat(filename1,"/");
            strcat(filename1, filename[j]);
            strcat(filename1,".txt");
           
            
            
            // strcat(filename,entry->d_name);
            printf("File name: %s\n",filename1);
            // Open the file
            file = fopen(filename1, "r");

            if (file == NULL) {
                perror("Error opening file");
                return 1;
            }

            if (!fp) {
                // Error in file opening
                printf("Can't open file\n");
                return 0;
            }

            fprintf(fp, "%s\n", filename1);
            // Read the file line by line and search for the specific text
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, search_text) != NULL) {
                    //printf("%s ", strstr(line, search_text));
                    f=strstr(line,search_text);
                    printf("%s", f + strlen(f) - 12);
                    fprintf(fp, "%s", f + strlen(f) - 12);
                    //printf("%s \n", strstr(line, search_text));
                    
                }
            }
        
            //printf("\nNew Account added to record\n");
        
             // Close the file
            fclose(file);
            
        //}
    }
    
    
    fclose(fp);
   
    

    // Close the directory
    closedir(directory);


    }
