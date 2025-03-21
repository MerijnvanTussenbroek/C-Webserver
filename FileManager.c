//path to the webserver files

#include <stdio.h>

#include <string.h>

char* openFile(char* path);

char* openFile(char* path)
{
    FILE *file;

    //We try to open the file in reading mode
    file = fopen(path, "r");

    if(file == NULL)
    {
        //the file doesn't exist
        printf("The file doesn't exist.");
        return NULL;
    }

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

    return fileInput;
}