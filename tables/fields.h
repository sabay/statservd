struct tableFields {
   enum TYPE{ INT32, INT64, DOUBLE};

   char * name;
   size_t offset;
   TYPE type;
};

struct compareFields{
   size_t offset;
   tableFields::TYPE type;
   int wcount;
   int bcount;
   char *witelist;
   char *blacklist;
};


template <class T>
int binarySearchFirst(T *a, int n ,T &x, int &position )
{
    size_t first = 0; /* Первый элемент в массиве */
    size_t last = n;    /* Элемент в массиве, СЛЕДУЮЩИЙ ЗА последним */
                                        /* Если просматриваемый участок непустой, first<last */
    size_t mid; 
 
    if (n == 0)
    {
	     position=0;
         goto not_found ; /* массив пуст */
    } 
    else if (x < a[0])
    {
	     position=0;
         goto not_found ;/* не найдено; если вам надо вставить его со сдвигом - то в позицию 0    */
    } 
    else if (a[n - 1] < x)
    {
	     position=n;
         goto not_found ; /* не найдено; если вам надо вставить его со сдвигом - то в позицию n    */
    }
 
    while (first < last)
    {
        /* ВНИМАНИЕ! В отличие от более простого (first+last)/2, этот код стоек к переполнениям.    */
        mid = first + (last - first) / 2;
 
        if ( !( a[mid]< x )) /* x <= a[mid] */
        {
            last = mid;
        }
        else
        {
            first = mid + 1;
        }
    }
 
    /* Если проверка n==0 в начале опущена - значит, тут раскомментировать!    */
    if (/*n!=0 &&*/ a[last] == x)
    {
		 position=last; /* Искомый элемент найден. last - искомый индекс */
    } else
    {
	    position=last;
		goto not_found ;
        /* Искомый элемент не найден. Но если вам вдруг надо его вставить со сдвигом, то его место - last.    */
    }

    return 1;	
not_found:
	return 0;
};


int isValid(char * row, compareFields *fields ,int fieldsCount){
    struct compareFields *field;
    for(int i=0;i<fieldsCount;i++){
        field = &fields[i];
        char * v = & row[field->offset];
        if( field->wcount == 1 ){
            switch(fields[i].type){
                case tableFields::INT32:
                    if( *(uint32_t *)v != *(uint32_t *)field->witelist ) return 0;
                    break;
                case tableFields::INT64:
                    if( *(uint64_t *)v != *(uint64_t *)field->witelist ) return 0;
                    break;
                case tableFields::DOUBLE:
                    if( *(double *)v != *(double *)field->witelist) return 0;
                    break;
            }
        } else if(field->wcount > 1 ) {
            int position;
            switch(fields[i].type){
                case tableFields::INT32:
                    if( binarySearchFirst((uint32_t *)field->witelist, field->wcount ,*(uint32_t *)v, position ) == 0 ) return 0;
                    break;
                case tableFields::INT64:
                    if( binarySearchFirst((uint64_t *)field->witelist, field->wcount ,*(uint64_t *)v, position ) == 0 ) return 0;
                    break;
                case tableFields::DOUBLE:
                    if( binarySearchFirst((double *)field->witelist, field->wcount ,*(double *)v, position ) == 0 ) return 0;
                    break;
            }
        }

        if( field->bcount == 1 ){
            switch(fields[i].type){
                case tableFields::INT32:
                    if( *(uint32_t *)v == *(uint32_t *)field->blacklist ) return 0;
                    break;
                case tableFields::INT64:
                    if( *(uint64_t *)v == *(uint64_t *)field->blacklist) return 0;
                    break;
                case tableFields::DOUBLE:
                    if( *(double *)v == *(double *)field->blacklist) return 0;
                    break;
            }
        } else if(field->bcount > 1 ) {
            int position;
            switch(fields[i].type){
                case tableFields::INT32:
                    if( binarySearchFirst((uint32_t *)field->blacklist, field->bcount ,*(uint32_t *)v, position ) == 1 ) return 0;
                    break;
                case tableFields::INT64:
                    if( binarySearchFirst((uint64_t *)field->blacklist, field->bcount ,*(uint64_t *)v, position ) == 1 ) return 0;
                    break;
                case tableFields::DOUBLE:
                    if( binarySearchFirst((double *)field->blacklist, field->bcount ,*(double *)v, position ) == 1 ) return 0;
                    break;
            }
        }
    }
    return 1;
};

