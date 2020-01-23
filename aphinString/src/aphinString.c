#include "../hdrs/API.h"

str aphinString__changeFormat(str path,str fmt){
    size_t i = path.size-1;
    for(; i >= 0 && path.str[i] != '/'; --i){
        if( path.str[i] == '.'){
            path.str[i] = '\0';
            path = aphinString__mkstr(path.str); 
            break;
        }
    }
    path = aphinString__append(2, path, fmt);
    return path;
}

str aphinString__cpyBytes(char* from, size_t size){
    str t;
    t.str = (char*)malloc(size);
    t.size = size;

    for(size_t i = 0; i < size; t.str[i] = from[i],++i);
    return t;
}

str aphinString__upDir(str dir){
    for(size_t i = dir.size-1; i >= 0; --i)
        if ( dir.str[i] == '/' ){
            dir.str[i] = '\0';
            str t = aphinString__mkstr(dir.str);
            aphinString__drop(dir);
            return t;
        }
    dir.size = 0;
    return dir;
}

int8_t aphinString__cmpstr(str a, str b){
    if ( a.size != b.size ) return -1;
    for (size_t i = 0; i < a.size; ++i)
        if ( a.str[i] != b.str[i] ) return -1;
    return 0;
}

str aphinString__append(size_t count, ...){
    va_list strings_pointer;
    va_start(strings_pointer,count);
    size_t total_size = 0;
    for(size_t i = 0; i < count;++i) {
        total_size+=va_arg(strings_pointer,str).size;
    }
    va_end(strings_pointer);
    va_start(strings_pointer,count);
    str t;
    t.size = total_size;
    t.str  = (char*)malloc(total_size+1); 
    for(size_t i = 0,a = 0; i < count;++i){
        str cu = va_arg(strings_pointer,str);
        for(size_t j = 0; j < cu.size; ++j,++a){
           *(t.str+a) = *(cu.str+j); 
        }
        aphinString__drop(cu);
    }
    va_end(strings_pointer);
    *(t.str+t.size) = '\0'; 
    return t;
}

str aphinString__mkstr(char* lit){
    size_t str_size = 0;
    for(;*(lit+str_size) != '\0';++str_size);
    str t;
    t.size = str_size;
    t.str = (char*)malloc(str_size+1);
    for(size_t i = 0; i < str_size; ++i){
        *(t.str+i) = *(lit+i);
    }
    *(t.str+t.size) = '\0';
    return t;
}
void aphinString__drop(str string){
    free(string.str);
    return;
}
