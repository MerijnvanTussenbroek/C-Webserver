
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

char* openFile(char* path);
int postFile(char* path, char* data);
int putFile(char* path,  char* data);
int deleteFile(char* path);

char* getFileType(char* path);
char* connectFileType(char* type);

char* timestampGenerator();

#endif