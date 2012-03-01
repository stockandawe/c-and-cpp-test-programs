#include <iostream>
#include <new>
using namespace std ;

#if _QUESTION_1_
int main() {
    int *p = new(nothrow) int ;
    cout << *p << endl;
    
    if( p!=0 ) 
    {
        *p = 99 ;
        int *q = p ;
        delete q ;
        cout << *p << endl ; 
    }
}
#endif

int main() {
    int *p = new(nothrow) int ;
          
    if( p!=0 ) {
        delete p ;
        *p = 99 ;
        cout << 12 << endl ; 
    }
}
