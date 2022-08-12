//
// Created by rhys on 11/08/22.
//

#include <request.h>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QCoreApplication>
#include <iostream>

LIBRARY_API Res_t* req_get(const char* url, Headers_t* headers){
    int argc = 0;
    QCoreApplication a(argc, NULL);
    QNetworkAccessManager manager;

    QNetworkRequest request;
    request.setUrl(QString(url));

    if (headers != NULL) {
        for (int i = 0; i < headers->length; i++) {
            request.setRawHeader(headers->keys[i], headers->values[i]);
        }
    }

    QNetworkReply* reply = manager.get(request);
    while (!reply->isFinished()) {
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    }

    Res_t* out = (Res_t*) malloc(sizeof(Res_t));
    if(reply->error() == QNetworkReply::NoError){
        QByteArray data = reply->readAll();

        out->err = 0;
        out->length = data.length();
        out->data = strdup(data.toStdString().c_str());
    }else{
        out->err = 1;
        out->length = 0;
        out->data = NULL;
    }

    return out;
}

LIBRARY_API Res_t* req_post_auth(const char* url, const char* data, Headers_t* headers, const char* username, const char* password) {

}