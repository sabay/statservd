#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "flatFileStrem.h"
#include "snappyFileStream.h"
#include "tables/tizer_stat.h"
#include "nwaymerge.h"

uint32_t tizer_id[]= { 2747, 698996, 786026, 1301094 };
uint32_t site_id[]= { 6318 };


uint32_t black_tizer_id[]= { 2747, 698996 };


struct compareFields inputFilt[] ={
    { table1Fields[1].offset, tableFields::INT32, sizeof(tizer_id)/sizeof(tizer_id[0]), sizeof(black_tizer_id)/sizeof(black_tizer_id[0]), (char *)tizer_id , (char *)black_tizer_id},
    { table1Fields[3].offset, tableFields::INT32, 1, 0, (char *)site_id, NULL }
};


int main()
{
char fileHeader[256];

blockStram * mergeFiles[100];
int mergeFilesCount=0;

mergeFiles[mergeFilesCount]= new flatFileStream(1024*1024);
mergeFiles[mergeFilesCount]->open("data/1.bin",blockStram::READ);
mergeFiles[mergeFilesCount]->read(fileHeader,sizeof(fileHeader));
++mergeFilesCount;


mergeFiles[mergeFilesCount]= new flatFileStream(1024*1024);
mergeFiles[mergeFilesCount]->open("data/3.bin",blockStram::READ);
mergeFiles[mergeFilesCount]->read(fileHeader,sizeof(fileHeader));
++mergeFilesCount;



snappyFileStream out(1024*1024);
out.open("data/out.bin.snap",blockStram::WRITE);
//flatFileStream out(1024*1024);
//out.open("data/out.bin",blockStram::WRITE);
out.write(fileHeader,sizeof(fileHeader));

n_way_merge<table1>(mergeFiles, mergeFilesCount, out);

out.close();

// search VVV

snappyFileStream snappyIn(1024*1024);
snappyIn.open("data/out.bin.snap",blockStram::READ);
snappyIn.read(fileHeader,sizeof(fileHeader));
printf(">>>>>>>>>>>>>> %s\n",fileHeader);

table1 row;       	
int i=0;

int inputFiltCount = sizeof(inputFilt) / sizeof(inputFilt[0]);
while(snappyIn.read((char *)&row,sizeof(row)) != -1){
    if(isValid((char *)&row, inputFilt , inputFiltCount)) row.dump();
}       

snappyIn.close();     


return 0;

}
