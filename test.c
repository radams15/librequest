#include <stdio.h>
#include <stdlib.h>

#include "request.h"

int main(int argc, char** argv){
	Res_t* data;

	if(argc <= 1){
		data = req_get("https://oauth.reddit.com/hot", 0, NULL, NULL);
	}else{
		data = req_get(argv[1], 0, NULL, NULL);
	}

	printf("Data: %s\n", data->data);

	res_free(data);

	return 0;
}