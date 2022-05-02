//
// Created by Rhys on 24/03/2022.
//

//
//  request
//  libreq
//
//  Created by Rhys Adams on 11/03/2022.
//  Copyright 2022. All rights reserved.
//

#include <request.h>

#include <stdlib.h>
#include <stdio.h>

#include <curl/curl.h>
#include <string.h>

static size_t writefunc(void *ptr, size_t size, size_t nmemb, Res_t* s) {
    size_t new_len = s->length + size*nmemb;
    s->data = realloc(s->data, new_len+1);
    if (s->data == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->data+s->length, ptr, size*nmemb);
    ((char*)s->data)[new_len] = '\0';
    s->length = new_len;

    return size*nmemb;
}

Res_t* req_get(const char* url, int use_proxy, const char* proxy, Headers_t* headers){
    CURL *curl;
    CURLcode res;

    Res_t* out = malloc(sizeof(Res_t));
    out->data = malloc(1);
    out->length = 0;

    char* real_url;
    if(use_proxy){
        unsigned long len = strlen(proxy) + strlen(url) + 8;
        real_url = calloc(len, sizeof(char));

        sprintf(real_url, "%s%s", proxy, url);
    }else{
        real_url = strdup(url);
    }

    curl = curl_easy_init();
    if(curl) {
        if(headers != NULL){
            struct curl_slist* curl_headers = NULL;
            for(int i=0 ; i<headers->length ; i++){
                char* together = calloc(strlen(headers->keys[i])+strlen(headers->values[i])+5, sizeof(char));
                sprintf(together, "%s: %s", headers->keys[i], headers->values[i]);

                curl_headers = curl_slist_append(curl_headers, together);
            }

            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);
        }

        curl_easy_setopt(curl, CURLOPT_URL, real_url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, out);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "curl-ca-bundle.crt");
        res = curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);

        free(real_url);

        return out;
    }

    return NULL;
}


Res_t* req_post_auth(const char* url, int use_proxy, const char* proxy, const char* data, Headers_t* headers, const char* username, const char* password){
    CURL *curl;
    CURLcode res = CURLE_OK;

    Res_t* out = malloc(sizeof(Res_t));
    out->data = malloc(1);
    out->length = 0;

    char* real_url;
    if(use_proxy){
        unsigned long len = strlen(proxy) + strlen(url) + 8;
        real_url = calloc(len, sizeof(char));

        sprintf(real_url, "%s%s", proxy, url);
    }else{
        real_url = strdup(url);
    }

    curl = curl_easy_init();
    if(curl) {
        if(headers != NULL){
            struct curl_slist* curl_headers = NULL;
            for(int i=0 ; i<headers->length ; i++){
                char* together = calloc(strlen(headers->keys[i])+strlen(headers->values[i])+5, sizeof(char));
                sprintf(together, "%s: %s", headers->keys[i], headers->values[i]);

                curl_headers = curl_slist_append(curl_headers, together);
            }

            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);
        }

        char* userpwd = calloc(strlen(username)+strlen(password)+5, sizeof(char));
        sprintf(userpwd, "%s:%s", username, password);

        //printf("\tUserpwd: %s\n", userpwd);

        curl_easy_setopt(curl, CURLOPT_URL, real_url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
        curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, out);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "curl-ca-bundle.crt");


        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        res = curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);

        free(real_url);

        //printf("Data (%s)/(%zu): %s\n", curl_easy_strerror(res), out->length, out->data);

        return out;
    }

    return NULL;
}