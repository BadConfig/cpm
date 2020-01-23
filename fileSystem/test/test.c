#include "../hdrs/API.h"
#include "../hdrs/tst.h"

int test__fs__mkstr_1 (){
    str a;
    a.size = 5;
    a.str = "abcab";
    str b = fs__mkstr("abcab");   
    assert(a.size != b.size);
    for( size_t i = 0; i <= a.size; ++i ){
        assert(*(a.str+i) != *(b.str+i));
    }
    return 1;
}




