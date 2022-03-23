//
// Created by Rhys on 23/03/2022.
//

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <header.h>
#include <string.h>

Headers_t* headers_append(Headers_t *headers, const char *key, const char* value) {
    if(headers == NULL){
        headers = malloc(sizeof(Headers_t));

        headers->length = 0;

        headers->keys = malloc(sizeof(char*));
        headers->values = malloc(sizeof(char*));
    }

    headers->length++;

    headers->keys = realloc(headers->keys, headers->length);
    headers->values = realloc(headers->values, headers->length);

    headers->keys[headers->length-1] = strdup(key);
    headers->values[headers->length-1] = strdup(value);

    return headers;
}

void headers_free(Headers_t *headers) {
    printf("Free headers: TODO\n");
}
