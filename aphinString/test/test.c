#include "../hdrs/API.h"
#include "../hdrs/tst.h"

int test_s(){
    str a;
    a.size = 5;
    a.str = "abcab";
    str b = s("abcab");   
    assert(a.size == b.size);
    for( size_t i = 0; i <= a.size; ++i ){
        assert(*(a.str+i) == *(b.str+i));
    }
    a.size = 0;
    a.str = "";
    b = s("");
    assert(a.size == b.size );
    for( size_t i = 0; i <= a.size; ++i ){
        assert(*(a.str+i) == *(b.str+i));
    }
    return 1;
}




