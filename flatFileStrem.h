#include "blockStream.h"
class flatFileStream : public blockStram{

    int readBlock(){
        localCursor=0;
        realSize=::read(fd,buffer,blockSize);
        if(realSize<0) realSize = 0;
        return realSize;
    };

    int writeBlock(){
        size_t ret = ::write(fd,buffer,localCursor);
        localCursor=0;
        return ret;
    };

public:
    flatFileStream(int x):blockStram(x){};

    int seek(size_t size){
        lseek(fd, size, SEEK_SET); 
        if( readBlock() >0)  return 0;
        return -1;
    };

};

