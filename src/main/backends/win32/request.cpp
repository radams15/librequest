#include <string>
#include <windows.h>
#include <wininet.h>

#include <b64.h>

#include <request.h>

#define _CRT_SECURE_NO_WARNINGS

#pragma comment (lib, "Wininet.lib")

//const char* inet_get(const wchar_t* _server, const wchar_t* _page, int flags = NULL, size_t len = 1024){
const char* inet_get(const char* _server, const char* _page, const char* headers, int flags = 0, size_t len = 1024){
    char* szData = (char*) calloc(len, sizeof(char));

    // initialize WinInet
    HINTERNET hInternet = InternetOpen(TEXT("libreq"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternet != NULL) {
        // open HTTP session
        HINTERNET hConnect = InternetConnect(hInternet, _server, INTERNET_DEFAULT_HTTP_PORT, NULL,NULL, INTERNET_SERVICE_HTTP, flags, 0);
        if (hConnect != NULL) {
            // open request
            HINTERNET hRequest = HttpOpenRequest(hConnect, "GET", _page ,NULL, NULL, 0, INTERNET_FLAG_KEEP_CONNECTION, 1);
            if (hRequest != NULL) {   
                // send request
                int headerlen = 0;
                if(headers){
                    headerlen = strlen(headers);
                }
                BOOL isSend = HttpSendRequest(hRequest, headers, headerlen, NULL, 0);

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

const char* inet_post(const char* _server, const char* _page, const char* data, const char* headers, int flags = 0, size_t len = 1024){
    char* szData = (char*) calloc(len, sizeof(char));

    // initialize WinInet
    HINTERNET hInternet = InternetOpen(TEXT("libreq"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternet != NULL) {
        // open HTTP session
        HINTERNET hConnect = InternetConnect(hInternet, _server, INTERNET_DEFAULT_HTTP_PORT, NULL,NULL, INTERNET_SERVICE_HTTP, flags, 0);
        if (hConnect != NULL) {
            // open request
            HINTERNET hRequest = HttpOpenRequest(hConnect, "POST", _page ,NULL, NULL, 0, INTERNET_FLAG_KEEP_CONNECTION, 1);
            if (hRequest != NULL) {
                // send request
                int headerlen = 0;
                if(headers){
                    headerlen = strlen(headers);
                }
                BOOL isSend = HttpSendRequestA(hRequest, headers, headerlen, (void*) data, strlen(data));

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

Res_t* req_post_auth(const char* url, int use_proxy, const char* proxy, const char* data, Headers_t* headers, const char* username, const char* password){
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

    /*wchar_t* url_mut = (wchar_t*) calloc(strlen(url)+1, sizeof(wchar_t));
    mbstowcs(url_mut, url, strlen(url)+1);

    wchar_t* base = wcstok(url_mut, L"/");
    wchar_t* path = wcstok(NULL, L"/");

    out->data = (char*) inet_get(base, path, flags, 102400);*/

    char* url_mut = (char*) calloc(strlen(url)+1, sizeof(char));
    strncpy(url_mut, url, strlen(url)+1);

    char* auth_str = (char*) calloc(strlen(username)+strlen(password)+64, sizeof(char));
    sprintf(auth_str, "%s:%s", username, password);
    const char* as = b64_encode((unsigned char*) auth_str, strlen(auth_str));
    char* auth_val = (char*) calloc(strlen(as)+64, sizeof(char));
    sprintf(auth_val, "Basic %s", as);
    free((char*) as);
    free(auth_str);

    headers = headers_append(headers, "Authorization", auth_val);

    char* base = strtok(url_mut, "/");
    char* path = (char*) url+strlen(base);

    char* headerstr = NULL;
    if(headers) {
        headerstr = (char *) calloc(2048, sizeof(char));
        for (int i = 0; i < headers->length; i++) {
            char *buf = (char *) calloc(strlen(headers->keys[i]) + strlen(headers->values[i]) + 10, sizeof(char));

            sprintf(buf, "%s: %s;", headers->keys[i], headers->values[i]);

            strcat(headerstr, buf);
        }
        strcat(headerstr, "\r\n");
    }
    out->data = (char*) inet_post(base, path, data, headerstr, flags, 102400);

    return out;
}

Res_t* req_get(const char* url, int use_proxy, const char* proxy, Headers_t* headers){
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

	/*wchar_t* url_mut = (wchar_t*) calloc(strlen(url)+1, sizeof(wchar_t));
	mbstowcs(url_mut, url, strlen(url)+1);

	wchar_t* base = wcstok(url_mut, L"/");
	wchar_t* path = wcstok(NULL, L"/");

	out->data = (char*) inet_get(base, path, flags, 102400);*/

	char* url_mut = (char*) calloc(strlen(url)+1, sizeof(char));
	strncpy(url_mut, url, strlen(url)+1);

    char* base = strtok(url_mut, "/");
    char* path = (char*) url+strlen(base);

    char* headerstr = NULL;
    if(headers) {
        headerstr = (char *) calloc(2048, sizeof(char));
        for (int i = 0; i < headers->length; i++) {
            char *buf = (char *) calloc(strlen(headers->keys[i]) + strlen(headers->values[i]) + 10, sizeof(char));

            sprintf(buf, "%s: %s;", headers->keys[i], headers->values[i]);

            strcat(headerstr, buf);
        }
        strcat(headerstr, "\r\n");
    }
    printf("%s\r\n", headerstr);


    out->data = (char*) inet_get(base, path, headerstr, flags, 102400);

	return out;
}