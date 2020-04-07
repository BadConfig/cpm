#include "../hdrs/API.h"

void upckErr(err e, void* data){
    if ( e.errCode == NO_ERR ){
        if( e.u.d.data == NULL || data == NULL){
            return;
        }
        uint8_t i;
        for(i = 0; i < e.u.d.size; ++i )
            *((uint8_t*)data+i) = *((uint8_t*)e.u.d.data+i);
        free(e.u.d.data);
        return;
    }
    e.u.reaction(e,data); 
    return;
}
