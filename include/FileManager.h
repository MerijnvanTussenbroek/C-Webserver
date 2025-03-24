
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

char* openFile(char* path);
void deleteFile(char* path);
void postFile(char* path, char* data);
void putFile(char* path,  char* data);

#endif