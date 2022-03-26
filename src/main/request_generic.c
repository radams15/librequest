//
// Created by rhys on 3/26/22.
//

#include <request.h>

#include <stdio.h>

size_t req_get_dl(const char* url, const char* path, int use_proxy, const char* proxy, Headers_t* headers){
    Res_t* out = req_get(url, use_proxy, proxy, headers);

    FILE* fout = fopen(path, "wb");

    fwrite(out->data, out->length, sizeof(char), fout);

    fclose(fout);

    return out->err;
}