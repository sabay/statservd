#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "flatFileStrem.h"
#include "snappyFileStream.h"
#include "tables/tizer_stat.h"
#include "nwaymerge.h"


int main()
{
char fileHeader[256];

table1 row;


snappyFileStream in(1024*1024);
in.open("data/out.bin.snap",blockStram::READ);
in.read(fileHeader,sizeof(fileHeader));

while(in.read((char *)&row,sizeof(row)) != -1){
 row.dump();
}            
in.close();

}
