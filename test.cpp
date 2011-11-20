#include<stdio.h>

template <class X >int f( int  a){
   X t;
   printf("%ld",sizeof(t));
};

int main(){
    int k=13;
    f <double > (k);
    return 0;
}
