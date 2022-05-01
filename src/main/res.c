#include <request.h>

#include <stdlib.h>

LIBRARY_API void res_free(Res_t* res){
	free((void*) res->data);
	free(res);
}