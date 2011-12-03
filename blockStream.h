#ifndef COMMON_BLOCK_STREAM_H
#define COMMON_BLOCK_STREAM_H

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

class blockStram{

protected:
    char *buffer;
    size_t blockSize;
    size_t realSize;
    size_t localCursor;
    char *filename;
    int fd;


    virtual int readBlock() = 0;
    virtual int writeBlock() = 0;

    virtual int postOpen() {};
    virtual int postClose() {};
public:
    enum TYPE{READ,WRITE} type;

    blockStram(int bSize) {
       blockSize = bSize;
       buffer    = (char *) malloc(blockSize);
       filename = NULL;
       fd = 0;
    };

    virtual ~blockStram() {
        free(buffer);
    }

    int open(char * filename, TYPE type=READ);
    int close();    
    int read(char *buff,size_t maxsize);
    int write(char * buff,size_t size);
    virtual int seek(size_t size) = 0;
};



int blockStram::close(){
    if(fd >0 ){
        if(type == WRITE) {
            writeBlock();
        }
        ::close(fd);
    }
    fd=0;
    postClose();
};

int blockStram::open(char * filename_, TYPE type_){
    type = type_;
    if(filename != NULL) {
        free(filename);
    }
    filename = strdup(filename_);
    fd = ::open(filename, type == READ ? O_RDONLY: O_WRONLY | O_CREAT);
    realSize = 0;
    localCursor =0;
    postOpen();
    return fd;
};

int blockStram::read(char *buff,size_t maxsize){
    do {
        size_t tail = realSize - localCursor;
        if(tail == 0) {
            tail = readBlock();
            if(tail == 0) {
                break;
            }
        }
        if(maxsize > tail ) {
            memcpy(buff,&buffer[localCursor],tail);
            buff+=tail;
            maxsize-=tail;
            localCursor+=tail;
        } else {
            memcpy(buff,&buffer[localCursor],maxsize);
            localCursor+=maxsize;
            maxsize=0;
        }
    } while( maxsize > 0 );
    if(maxsize>0) return -1;
    return 0;
};

int blockStram::write(char * buff,size_t size){
    do{
        size_t tail = blockSize - localCursor;
        if(tail == 0) {
            tail = writeBlock();
            if(tail <= 0) break;
        }
        if(size > tail ){
            memcpy(&buffer[localCursor],buff,tail);
            buff+=tail;
            size-=tail;
            localCursor+=tail;
        } else {
            memcpy(&buffer[localCursor],buff,size);
            localCursor+=size;
            size=0;
        }
    } while( size >0 );
    if( size > 0 ) return -1;
    return 0;
};

#endif
