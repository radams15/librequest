/*
 *  res.h
 *  libreq
 *
 *  Created by Rhys Adams on 11/03/2022.
 *  Copyright 2022. All rights reserved.
 *
 */

#include <stddef.h>

#define EXPORT comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Res{
	const void* data;
	int err;
	size_t length;
} Res_t;

void res_free(Res_t* res);

#ifdef __cplusplus
}
#endif
