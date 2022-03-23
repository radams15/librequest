/*
 *  res.h
 *  libreq
 *
 *  Created by Rhys Adams on 11/03/2022.
 *  Copyright 2022 __MyCompanyName__. All rights reserved.
 *
 */

#include <stddef.h>

#ifdef  REQUEST_EXPORTS 
/*Enabled as "export" while compiling the dll project*/
#define DLLEXPORT __declspec(dllexport)  
#else
/*Enabled as "import" in the Client side for using already created dll file*/
#define DLLEXPORT __declspec(dllimport)  
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Res{
	const void* data;
	int err;
	size_t length;
} Res_t;

DLLEXPORT void res_free(Res_t* res);

#ifdef __cplusplus
}
#endif
