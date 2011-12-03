#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <unistd.h>

template <class TABLE >
int n_way_merge_date(blockStram **in, int inCount)
{
    uint32_t oldDate=0;
    blockStram *out=NULL;
    std::vector<std::string> fileNamesForTransaction;
    char filename[255];
    int i=0;
    TABLE *l = (TABLE  *) malloc(sizeof(TABLE) * (inCount +1) );
    for(i=0;i<inCount;i++) {
        if(in[i]->read((char *)&l[i],sizeof(TABLE)) == -1){
            if(inCount > 1){
                in[i]->close();
                delete(in[i]);
                in[i]=in[inCount-1];
            }
            --inCount;
            --i;
        } 
    }
    int *foundIndexes = (int *) malloc(sizeof(int) * (inCount + 1) );   // +1 for open day db file 
    int foundCount=0;
    int minRowId=0;

    while ( inCount > 0 ) {
        foundCount=0;
        foundIndexes[foundCount++]=0;
        for(i=1;i<inCount;i++) {
            int comp=l[i].cmp(l[foundIndexes[0]]);
            if( comp > 0 ) {
                foundCount=0;
                foundIndexes[foundCount++]=i;
            } else if( comp == 0) {
                foundIndexes[foundCount++]=i;
            }
        }


        if(l[foundIndexes[0]].date != oldDate) {
            if( out != NULL){
                out->close();
                delete out;
            }
            out = new snappyFileStream(1024*1024);
            sprintf(filename, "data/%d", l[foundIndexes[0]].date);
	    mkdir(filename, 0x777);
            sprintf(filename, "data/%d/out.bin.sn", l[foundIndexes[0]].date);
            fileNamesForTransaction.push_back(filename);
            out->open(filename,blockStram::WRITE);
            oldDate = l[foundIndexes[0]].date;
            // read and compare
            in[inCount]= new snappyFileStream(1024*1024);
            sprintf(filename, "data/%d/out.bin.snap", l[foundIndexes[0]].date);
            if(in[inCount]->open(filename,blockStram::READ) < 0 ){
                delete(in[inCount]);
            } else {
                if(in[inCount]->read((char *)&l[inCount],sizeof(TABLE)) == -1){
                    in[inCount]->close();
                    delete(in[inCount]);
                } else {
                    int comp=l[inCount].cmp(l[foundIndexes[0]]);
                    if( comp > 0 ) {
                        foundCount=0;
                        foundIndexes[foundCount++]=inCount;
                    } else if( comp == 0) {
                        foundIndexes[foundCount++]=inCount;
                    }
                    ++inCount;
                }
            } 
            //  end read and compare
        }

        for(i=1;i<foundCount;i++){
            l[foundIndexes[0]].merge(l[foundIndexes[i]]);
        }
        out->write((char *)& l[foundIndexes[0]],sizeof(l[foundIndexes[0]]));
        for(i=0;i<foundCount;i++){
            if(in[foundIndexes[i]]->read((char *)&l[foundIndexes[i]],sizeof(TABLE)) == -1){
                if(inCount > 1){
                    in[foundIndexes[i]]->close();
                    delete(in[foundIndexes[i]]);
                    in[foundIndexes[i]]=in[inCount-1];
                    l[foundIndexes[i]]=l[inCount-1];
                }
                --inCount;
            }

        }
    }
    if( out != NULL){
        out->close();
        delete out;
    }
    std::vector<std::string>::iterator  fileNamesForTransaction_i;
    for(fileNamesForTransaction_i=fileNamesForTransaction.begin();fileNamesForTransaction_i<fileNamesForTransaction.end(); fileNamesForTransaction_i++){
        sprintf(filename, "%sap", fileNamesForTransaction_i->c_str());
        rename(fileNamesForTransaction_i->c_str(), filename);
    }
    return 0;
};



template <class TABLE >
int n_way_merge(blockStram **in, int inCount, blockStram &out)
{
    int i=0;
    TABLE *l = (TABLE  *) malloc(sizeof(TABLE) * inCount );
    for(i=0;i<inCount;i++) {
        if(in[i]->read((char *)&l[i],sizeof(TABLE)) == -1){
            if(inCount > 1){
                in[i]->close();
                delete(in[i]);
                in[i]=in[inCount-1];
            }
            --inCount;
            --i;
        } 
    }
    int *foundIndexes = (int *) malloc(sizeof(int) * inCount );    
    int foundCount=0;
    int minRowId=0;

    while ( inCount > 0 ) {
        foundCount=0;
        foundIndexes[foundCount++]=0;
        for(i=1;i<inCount;i++) {
            int comp=l[i].cmp(l[foundIndexes[0]]);
            if( comp > 0 ) {
                foundCount=0;
                foundIndexes[foundCount++]=i;
            } else if( comp == 0) {
                foundIndexes[foundCount++]=i;
            }
        }
        for(i=1;i<foundCount;i++){
            l[foundIndexes[0]].merge(l[foundIndexes[i]]);
        }
        out.write((char *)& l[foundIndexes[0]],sizeof(l[foundIndexes[0]]));
        for(i=0;i<foundCount;i++){
            if(in[foundIndexes[i]]->read((char *)&l[foundIndexes[i]],sizeof(TABLE)) == -1){
                if(inCount > 1){
                    in[foundIndexes[i]]->close();
                    delete(in[foundIndexes[i]]);
                    in[foundIndexes[i]]=in[inCount-1];
                    l[foundIndexes[i]]=l[inCount-1];
                }
                --inCount;
            }

        }
    }
    return 0;
};
