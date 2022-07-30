/*
 *  res.h
 *  libreq
 *
 *  Created by Rhys Adams on 11/03/2022.
 *  Copyright 2022. All rights reserved.
 *
 */

#ifdef __APPLE__
#define size_t long
#else
#include <stddef.h>
#endif

#if defined(_WIN32)  && !NO_EXPORT
#    ifdef LIBRARY_EXPORTS
#        define LIBRARY_API __declspec(dllexport)
#    else
#        define LIBRARY_API __declspec(dllimport)
#    endif
#else
#    define LIBRARY_API
#endif


#ifdef __cplusplus
extern "C" {
#endif

typedef struct Res{
	char* data;
	int err;
	size_t length;
} Res_t;

LIBRARY_API void res_free(Res_t* res);

#ifdef __cplusplus
}
#endif
