//path to the webserver files

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

char* standardPath = "C:/Users/merij/Desktop/webserverFiles";



char* openFile(char* path)
{
    FILE *file;

    char* fullPath = malloc(200 * sizeof(char));

    strcpy(fullPath, standardPath);

    if(strcmp(path, "/") == 0)
    {
        strcat(fullPath, "/index.html");
        printf("%s", fullPath);
    }
    else
    {
        strcat(fullPath, path);
    }

    //We try to open the file in reading mode
    file = fopen(fullPath, "r");

    if(file == NULL)
    {
        //the file doesn't exist
        printf("\nThe file doesn't exist.");
        return NULL;
    }

    printf("\nFile opened");

    //We get the size of the file
    fseek(file, 0, SEEK_END);

    size_t length = ftell(file);

    rewind(file);

    //We make the output and helper char array
    char* fileInput = malloc((length + 1) * sizeof(char));
    fileInput[0] = '\0';
    char* nextLine = malloc(length * sizeof(char));

    //We continuously get the next line in the file and concatonate it into the output char array
    while(fgets(nextLine, length, file))
    {
        printf("\ngetting data");
        printf("%s", nextLine);
        strcat(fileInput, nextLine);
    }

    //We close off the output with the \0 symbol
    fileInput[length] = '\0';

    //We close the file and free the helper char array
    fclose(file);
    free(nextLine);
    free(fullPath);

    printf("\nFile read");

    return fileInput;
}

char* getFileType(char* path)
{

    if(strcmp(path, "/") == 0)
    {
        char* fileType = malloc(5);
        fileType[0]= '\0';
        strcpy(fileType, "html");
        return fileType;
    }
    else
    {
        char* input = path;
        while(*input != '.')
        {
            input++;
        }

        input++;

        char* beginning = input;

        while(*input != '\0')
        {
            input++;
        }

        //input--;

        size_t length = input - beginning;

        char* fileType = malloc((length + 1) * sizeof(char));

        strncpy(fileType, beginning, length);

        fileType[length] = '\0';

        return fileType;
    }
}

char* connectFileType(char* fileType)
{
    if(strcmp(fileType, "html") == 0)
    {
        char* type = malloc(11);
        strcpy(type, "text/html");
        type[10] = '\0';
        return type;
    }
    if(strcmp(fileType, "css") == 0)
    {
        char* type = malloc(10);
        strcpy(type, "text/css");
        type[9] = '\0';
        return type;
    }
    if(strcmp(fileType, "js") == 0)
    {
        char* type = malloc(24);
        strcpy(type, "application/javascript");
        type[23] = '\0';
        return type;
    }

    char* type = malloc(6);
    strcpy(type, "error");
    type[5] = '\0';
    return type;
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
    fseek(file, 0, SEEK_END);

    fprintf(file, data);

    fclose(file);
}

void putFile(char* path, char* data)
{
    FILE* file;
    file = fopen(path, "w");
    fseek(file, 0, SEEK_END);

    fprintf(file, data);

    fclose(file);
}