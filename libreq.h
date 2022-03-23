











typedef long int ptrdiff_t;

typedef long unsigned int size_t;

typedef int wchar_t;



typedef long double max_align_t;









typedef struct Res{
 const void* data;
 int err;
 size_t length;
} Res_t;

void res_free(Res_t* res);








Res_t* req_get(const char* url);
