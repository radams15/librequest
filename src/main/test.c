#include <request.h>
#include <stdio.h>

int main(){
	Res_t* res = req_get("https://google.com", NULL);
	
	printf("%s\n", res->data);
}
