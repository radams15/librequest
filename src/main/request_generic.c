//
// Created by rhys on 3/26/22.
//

#include <request.h>

#ifndef __APPLE__
#include <stdio.h>
#endif

LIBRARY_API size_t req_get_dl(const char* url, const char* path, int use_proxy, const char* proxy, Headers_t* headers){
    Res_t* out = req_get(url, use_proxy, proxy, headers);

    FILE* fout = fopen(path, "wb");
    if(fout == NULL) {
        printf("Cannot open file '%s'!\n", path);
        return 1;
    }

    fwrite(out->data, out->length, sizeof(char), fout);

    fclose(fout);

    return out->err;
}
