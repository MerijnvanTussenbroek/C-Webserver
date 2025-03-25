//path to the webserver files

#include <stdio.h>
#include <stdlib.h>

#include <string.h>



char* openFile(char* path)
{
    FILE *file;

    //We try to open the file in reading mode
    file = fopen(path, "r");

    if(file == NULL)
    {
        //the file doesn't exist
        printf("\nThe file doesn't exist.");
        return NULL;
    }

    printf("\nFile opened");

    //We get the size of the file
    fseek(file, 0, SEEK_END);

    long length = ftell(file);

    rewind(file);

    //We make the output and helper char array
    char* fileInput = malloc((length + 1) * sizeof(char));
    char* nextLine = malloc(length * sizeof(char));

    //We continuously get the next line in the file and concatonate it into the output char array
    while(fgets(nextLine, length, file))
    {
        strcat(fileInput, nextLine);
    }

    //We close off the output with the \0 symbol
    fileInput[length] = '\0';

    //We close the file and free the helper char array
    fclose(file);
    free(nextLine);

    printf("\nFile read");

    return fileInput;
}

void deleteFile(char* path)
{
    //this should only be allowed if the user has proper authentication
    printf("\ndeleted file");
    remove(path);
}

void postFile(char* path, char* data)
{
    FILE* file;
    file = fopen(path, "w");

    fprintf(file, data);

    fclose(file);
}

void putFile(char* path, char* data)
{
    FILE* file;
    file = fopen(path, "w");

    fprintf(file, data);

    fclose(file);
}