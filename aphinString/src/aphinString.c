#include "../hdrs/API.h"

char* strSubstr(str substr, str string){
    for(size_t i = 0, j = 0; i < string.size; ++i ){
        if (substr.str[j] == string.str[i]){
            ++j;
            if ( j == substr.size){
                strDrop(substr);
                strDrop(string);
                return string.str+i-j;
            }
            continue;
        }
        else{
            j = 0;
        }
    }
    strDrop(string);
    strDrop(substr);
    return NULL;
}

str strChExt(str path,str ext){
    size_t i = path.size-1;
    for(; i >= 0 && path.str[i] != '/'; --i){
        if( path.str[i] == '.'){
            path.str[i] = '\0';
            path = s(path.str); 
            break;
        }
    }
    path = strAdd(2, path, ext);
    return path;
}

str strn(char* lit, size_t n){
    str t;
    t.str = (char*)malloc(n);
    t.size = n;
    for(size_t i = 0; i < n; t.str[i] = lit[i],++i);
    return t;
}

str strUpDir(str dir){
    for(size_t i = dir.size-1; i >= 0; --i)
        if ( dir.str[i] == '/' ){
            dir.str[i] = '\0';
            str t = s(dir.str);
            strDrop(dir);
            return t;
        }
    dir.size = 0;
    return dir;
}

int8_t strCmp(str a, str b){
    for (size_t i = 0; i < a.size && i < b.size; ++i)
        if ( a.str[i] != b.str[i] ) 
            return a.str[i]-b.str[i];
    return 0;
}

str strAdd(size_t count, ...){
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
        strDrop(cu);
    }
    va_end(strings_pointer);
    *(t.str+t.size) = '\0'; 
    return t;
}

str s(char* lit){
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
void strDrop(str string){
    free(string.str);
    return;
}

str strPath(uint32_t count, ...){
    va_list string;
    va_start(string,count);
    str res;
    res = s("");
    for(uint32_t i = 0; i < count; ++i){
        res = strAdd(3,res,s("/"),va_arg(string,str));
    }
    return res;
}