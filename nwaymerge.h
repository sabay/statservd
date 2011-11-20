#include <vector>
#include <algorithm>
#include <utility>
#include <functional>

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
