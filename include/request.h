/*
 *  request.h
 *  libreq
 *
 *  Created by Rhys Adams on 11/03/2022.
 *  Copyright 2022. All rights reserved.
 *
 */

#include "res.h"
#include "header.h"

#if defined(_WIN32) && !NO_EXPORT
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

LIBRARY_API Res_t* req_get(const char* url, Headers_t* headers);
LIBRARY_API Res_t* req_post_auth(const char* url, const char* data, Headers_t* headers, const char* username, const char* password);
LIBRARY_API Res_t* req_post(const char* url, const char* data, Headers_t* headers);

LIBRARY_API size_t req_get_dl(const char* url, const char* path, Headers_t* headers);

#ifdef __cplusplus
}
#endif
