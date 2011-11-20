#include "blockStream.h"
#include <snappy.h> 
#include <unistd.h>
class snappyFileStream : public blockStram{

    int readBlock(){
        localCursor=0;
        uint32_t packedBlockSize=0;
        ::read(fd,(char *)&packedBlockSize,sizeof(packedBlockSize));
        size_t compressed_length=::read(fd,packedBuffer,packedBlockSize);
        if (!snappy::GetUncompressedLength(packedBuffer, compressed_length, &realSize)) {
            realSize= 0;
        } else if(! snappy::RawUncompress(packedBuffer, compressed_length, buffer)) {
            realSize=0;
        }
        return realSize;
    };

    int writeBlock(){

        size_t size=0;
        snappy::RawCompress(buffer,
                   localCursor,
                   packedBuffer+sizeof(uint32_t),
                   &size);
        *((uint32_t *)packedBuffer)=size;
        size_t offset = lseek(fd, 0, SEEK_CUR); 
        ::fwrite((char *)&offset,sizeof(offset),1,idxStream);
        size_t ret  = ::write(fd,packedBuffer,size+sizeof(uint32_t));

        localCursor=0;
        return ret;
    };


    char * packedBuffer;
    char * indexBuffer;
    FILE * idxStream;
public:
    snappyFileStream(int x):blockStram(x){
        packedBuffer = (char * ) malloc( snappy::MaxCompressedLength(blockSize)+sizeof(uint32_t)); 
        indexBuffer = NULL;
        idxStream = NULL;
    };

    ~snappyFileStream(){
        free(packedBuffer);
        if(indexBuffer != NULL ){
            free (indexBuffer);
            indexBuffer=NULL;
        }
    };

    int seek(size_t size){
        int blockNumber= size / blockSize;
        int offsetInBlock= size % blockSize;
        if(fseek(idxStream, blockNumber * sizeof(size_t) , SEEK_SET) != 0)  {
            // out of range
            return -1;
        }
        size_t offset=0;
        fread ((char *)&offset, sizeof(offset), 1, idxStream);
        lseek(fd, offset, SEEK_SET); 
        readBlock();
        if(realSize < offsetInBlock) {
            // out of file
            return -1;
        }
        localCursor=offsetInBlock;
        return 0;

    };

    int postOpen() {
        int len=strlen(filename)+10;
        char * idxname= (char * )malloc(len);
        snprintf(idxname,len,"%s.idx",filename);
        idxStream = ::fopen(idxname, type == READ ? "rb": "wb");
        if(indexBuffer != NULL ){
            free (indexBuffer);
            indexBuffer=NULL;
        }
    };
    int postClose() {
        if( idxStream != NULL) ::fclose(idxStream);
        idxStream = NULL;
    };

};

