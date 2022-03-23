#define WINVER 0x0400
#define _WIN32_WINDOWS 0x0400

#include <string>
#include <windows.h>
#include <wininet.h>

#include "request.h"

#define _CRT_SECURE_NO_WARNINGS

#pragma comment (lib, "Wininet.lib")

const char* inet_get(const wchar_t* _server, const wchar_t* _page, int flags = NULL, size_t len = 1024){
//const char* inet_get(const char* _server, const char* _page, int flags = 0, size_t len = 1024){
    char* szData = (char*) calloc(len, sizeof(char));

    // initialize WinInet
    HINTERNET hInternet = InternetOpen(TEXT("libreq"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternet != NULL) {
        // open HTTP session
        HINTERNET hConnect = InternetConnect(hInternet, _server, INTERNET_DEFAULT_HTTP_PORT, NULL,NULL, INTERNET_SERVICE_HTTP, flags, 0);
        if (hConnect != NULL) {
            // open request
            HINTERNET hRequest = HttpOpenRequest(hConnect, L"GET", _page ,NULL, NULL, 0, INTERNET_FLAG_KEEP_CONNECTION, 1);
            if (hRequest != NULL) {   
                // send request
                BOOL isSend = HttpSendRequest(hRequest, NULL, 0, NULL, 0);

                if (isSend){
                    for(;;){
                        // reading data
                        DWORD dwByteRead;
                        BOOL isRead = InternetReadFile(hRequest, szData, len - 1, &dwByteRead);

                        // break cycle if error or end
                        if (isRead == FALSE || dwByteRead == 0)
                            break;

                        // saving result
                        szData[dwByteRead] = 0;
                    }
                }

                // close request
                InternetCloseHandle(hRequest);
            }
            // close session
            InternetCloseHandle(hConnect);
        }
        // close WinInet
       InternetCloseHandle(hInternet);
    }

    return (const char*) szData;
}

Res_t* req_get(const char* url){
	#pragma EXPORT
	int https = 0;

	Res_t* out = (Res_t*) malloc(sizeof(Res_t));

	out->err = 0;
	out->length = 10;

	int flags = 0;

	if(strncmp(url, "https://", 8) == 0){
		https = 1;
		url += 8;

		flags |= INTERNET_FLAG_SECURE;
	}

	if(strncmp(url, "http://", 7) == 0){
		https = 0;
		url += 7;
	}

	wchar_t* url_mut = (wchar_t*) calloc(strlen(url)+1, sizeof(wchar_t));
	mbstowcs(url_mut, url, strlen(url)+1);

	wchar_t* base = wcstok(url_mut, L"/");
	wchar_t* path = wcstok(NULL, L"/");

	out->data = inet_get(base, path, flags, 102400);

	/*char* url_mut = (char*) calloc(strlen(url)+1, sizeof(char));
	strncpy(url_mut, url, strlen(url)+1);

	char* base = strtok(url_mut, "/");
	char* path = strtok(NULL, "/");

	out->data = inet_get(base, path, flags, 102400);*/

	return out;
}