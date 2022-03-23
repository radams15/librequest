#include <request.h>

#include <stdlib.h>

void res_free(Res_t* res){
	#pragma EXPORT
	free((void*) res->data);
	free(res);
}