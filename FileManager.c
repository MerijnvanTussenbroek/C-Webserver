//path to the webserver files
//C:\Users\merij\Desktop\webserverFiles

// C:\Users\merij\Desktop\webserverFiles\testdoc.txt

#include <stdio.h>

#include <string.h>

char* openFile(char* path);

char* openFile(char* path)
{
    FILE *file;

    file = fopen(path, "r");

    if(file == NULL)
    {
        //the file doesn't exist
        printf("The file doesn't exist.");
        return NULL;
    }

    fseek(file, 0, SEEK_END);

    long length = ftell(file);

    rewind(file);

    char* fileInput = malloc((length + 1) * sizeof(char));
    char* nextLine = malloc(length * sizeof(char));

    //fgets(fileInput, length, file);

    while(fgets(nextLine, length, file))
    {
        strcat(fileInput, nextLine);
    }

    fileInput[length] = '\0';

    fclose(file);

    free(nextLine);

    return fileInput;
}