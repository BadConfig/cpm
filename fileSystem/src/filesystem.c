#include "../hdrs/API.h"
str pwd(){  
    char buff[1024];
    getcwd(buff,sizeof(buff));
    return s(buff); 
}
int makeFile(str path){
    uv_loop_t *loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
    uv_fs_t* req    = (uv_fs_t*)malloc(sizeof(uv_fs_t));

    uv_loop_init(loop); 
    int t = uv_fs_open(loop,req,path.str,O_RDWR,0777,NULL);
    uv_run(loop,UV_RUN_DEFAULT);
    uv_loop_close(loop);

    free(loop);
    free(req);
    return t;
}
str readFile(FILE* desk, int64_t offset, int64_t bytesToRead){
    char buff[bytesToRead];
    char c = 'a';
    int i = 0;
    for(i = 0; i < bytesToRead && c != EOF; ++i){
        char c  = fgetc(desk);
        buff[i] = c; 
    }
    buff[i+1] = '\0';
    return s(buff);
}
void makeDir(str path){
    uv_loop_t *loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
    uv_fs_t* req    = (uv_fs_t*)malloc(sizeof(uv_fs_t));

    uv_loop_init(loop); 
    uv_fs_mkdir(loop,req,path.str,0777,NULL);
    uv_run(loop,UV_RUN_DEFAULT);
    uv_loop_close(loop);

    free(loop);
    free(req);
    return;
}


