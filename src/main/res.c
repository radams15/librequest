#include <request.h>

#ifndef __APPLE__
#include <stdlib.h>
#endif

LIBRARY_API void res_free(Res_t* res){
	free((void*) res->data);
	free(res);
}
