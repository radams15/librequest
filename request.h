/*
 *  request.h
 *  libreq
 *
 *  Created by Rhys Adams on 11/03/2022.
 *  Copyright 2022. All rights reserved.
 *
 */

#include <res.h>
#include <header.h>

#define EXPORT comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)

#ifdef __cplusplus
extern "C" {
#endif

Res_t* req_get(const char* url, int use_proxy, const char* proxy, Headers_t* headers);
Res_t* req_post_auth(const char* url, int use_proxy, const char* proxy, const char* data, Headers_t* headers, const char* username, const char* password);

#ifdef __cplusplus
}
#endif
