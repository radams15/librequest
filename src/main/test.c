#include <request.h>

int main(){
	Res_t* res = req_get("https://google.com", 0, "", 0);
	
	printf("%s\n", res->data);
}
