//
// Created by Rhys on 23/03/2022.
//

#ifndef REQUEST_HEADER_H
#define REQUEST_HEADER_H

typedef struct Headers {
    unsigned int length;
    char** keys;
    char** values;
} Headers_t;

Headers_t* headers_append(Headers_t *headers, const char *key, const char* value);

void headers_free(Headers_t* headers);

#endif //REQUEST_HEADER_H
