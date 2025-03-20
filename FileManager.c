//path to the webserver files
//C:\Users\merij\Desktop\webserverFiles

#include <stdio.h>

char* openFile(char* path);

char* openFile(char* path)
{
    FILE *file;

    file = fopen(path, "r");

    if(file == NULL)
    {
        //the file doesn't exist
        fclose(file);
        return "NULL";
    }

    char* fileInput = malloc(file->_bufsiz);

    fgets(fileInput, file->_bufsiz, file);

    fclose(file);

    return fileInput;
}