//path to the webserver files

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <time.h>

char* generateFileName();
char* timestampGenerator();

char* standardPath = "./serverFiles";

//This is for processing GET requests
char* openFile(char* path)
{
    FILE *file;

    //We first generate a string for the full path
    char* fullPath = malloc(100 * sizeof(char));

    strcpy(fullPath, standardPath);

    //We check if the path is the home page, which is index.html in standard webservers
    if(strcmp(path, "/") == 0)
    {
        strcat(fullPath, "/index.html");
        printf("\n%s", fullPath);
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
        perror("\nThis error happened in the openFile function");
        printf("\n%s \n", fullPath);

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
    //int length = sizeof(path);

    if(strcmp(path, "/") == 0)
    {
        //If the path is the homepage, we need to return html as this will be index.html
        char* fileType = malloc(5);
        fileType[0]= '\0';
        strcpy(fileType, "html");
        return fileType;
    }
    else
    {
        //We begin looking for the . in file extensions index(.)html
        char* input = path;
        while(*input != '.')
        {
            input++;
        }

        input++;

        //We save the position of the first character, which is 1 spot after the dot
        char* beginning = input;

        //We continue until we get to the end of the string
        while(*input != '\0')
        {
            input++;
        }

        //We calculate the size
        size_t length = input - beginning;

        char* fileType = malloc((length + 1) * sizeof(char));

        //And take it from the path
        strncpy(fileType, beginning, length);

        fileType[length] = '\0';

        return fileType;
    }
}

char* connectFileType(char* fileType)
{
    //Here, we connect the filetype to what needs to be in the HTTP response
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
    return "error";
}

int deleteFile(char* path)
{
    //this should only be allowed if the user has proper authentication
    printf("\ndeleted file");

    int removal = remove(path);

    if(removal == 0)
    {
        return 1;
    }

    //If the remove function didn't return 0, it means something went wrong
    perror("something went wrong in the deleteFile function");

    if(removal == ENOENT)
    {
        //The file doesn't exist
        return -1;
    }
    if(removal == EBUSY)
    {
        //The file is being used
        return -2;
    }
    if(removal == EACCES)
    {
        //permission denied
        return -3;
    }

    //If it is none of these, we return 0
    return 0;
}

int postFile(char* path, char* data)
{
    //We override anything that might have been in the file previously
    FILE* file;
    char* fullPath = malloc(100 * sizeof(char));
    strcpy(fullPath, standardPath);
    fullPath[strlen(standardPath)] = '\0';

    strcat(fullPath, path);

    //We generate a filename
    char* lastPart = generateFileName();

    strcat(fullPath, lastPart);

    file = fopen(fullPath, "w");

    if(file == NULL)
    {
        perror("\nAn error happened opening a file in the postFile function");
        return 0;
    }

    //We write the data to the created file
    fprintf(file, data);

    free(lastPart);
    free(fullPath);

    fclose(file);

    return 1;
}

int putFile(char* path, char* data)
{
    FILE* file;

    char* fullPath = malloc(100 * sizeof(char));
    strcpy(fullPath, standardPath);
    fullPath[strlen(standardPath)] = '\0';
    strcat(fullPath, path);

    //Here, we specifically go for a, as this will append/update the file with whatever we want
    file = fopen(fullPath, "a");

    if(file == NULL)
    {
        perror("\nAn error happened opening a file in the putFile function");
        return 0;
    }

    fprintf(file, data);

    free(fullPath);

    fclose(file);
    return 1;
}

char* generateFileName()
{
    //Here, we generate filenames for POST requests
    char* name = malloc(100 * sizeof(char));

    char* time = timestampGenerator();

    sprintf("upload_%s", time);

    free(time);

    return name;
}

char* timestampGenerator()
{
    //This function is used for when a POST request does not provide a name, and we must make one
    //The name of the file will be upload_<timestamp>

    time_t current_time = time(NULL); // Get current time
    struct tm *utc_time = gmtime(&current_time);
    char* time = malloc(100 * sizeof(char));
    strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", utc_time);

    return time;
}