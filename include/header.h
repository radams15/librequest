//
// Created by Rhys on 23/03/2022.
//

#ifndef REQUEST_HEADER_H
#define REQUEST_HEADER_H

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

typedef struct Headers {
    unsigned int length;
    char** keys;
    char** values;
} Headers_t;

LIBRARY_API Headers_t* headers_append(Headers_t *headers, const char *key, const char* value);

LIBRARY_API void headers_free(Headers_t* headers);

#ifdef __cplusplus
}
#endif

#endif //REQUEST_HEADER_H
