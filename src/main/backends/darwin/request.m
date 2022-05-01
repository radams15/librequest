//
//  request
//  libreq
//
//  Created by Rhys Adams on 11/03/2022.
//  Copyright 2022. All rights reserved.
//

#include <Foundation/Foundation.h>

#include "../../../../lib/b64.c/b64.h"

#include <request.h>

Res_t* req_get(const char* url, int use_proxy, const char* proxy, Headers_t* headers){
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSMutableURLRequest* mutreq = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:[NSString stringWithUTF8String:url]]];

    if(headers != NULL) {
        unsigned int i;
        for (i=0; i < headers->length; i++) {
            [mutreq addValue:[NSString stringWithUTF8String:headers->values[i]] forHTTPHeaderField:[NSString stringWithUTF8String:headers->keys[i]]];
        }
    }

    NSURLRequest* req = [mutreq copy];

    NSURLResponse* res = nil;
	
	NSError* err = nil;
	
	NSData* data = [NSURLConnection sendSynchronousRequest:req returningResponse:&res error:&err];
	
	Res_t* out_var = malloc(sizeof(struct Res));
	out_var->length = 0;
	
	if(err == nil){
		out_var->err = 0;
		
		out_var->length = [data length];
		
		const void* bytes = [data bytes];
		
		out_var->data = malloc(out_var->length);
		
		memcpy((char*) out_var->data, bytes, out_var->length);

        [pool release];
		
		return out_var;
	}
	
	out_var->err = 1;
	
	NSLog(@"%@", [err localizedDescription]);

    [pool release];
	
	return out_var;
}

Res_t* req_post_auth(const char* url, int use_proxy, const char* proxy, const char* data, Headers_t* headers, const char* username, const char* password){
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSMutableURLRequest* mutreq = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:[NSString stringWithUTF8String:url]]];
    if(headers != NULL) {
        unsigned int i;
        for (i=0; i < headers->length; i++) {
            [mutreq addValue:[NSString stringWithUTF8String:headers->values[i]] forHTTPHeaderField:[NSString stringWithUTF8String:headers->keys[i]]];
        }
    }

    [mutreq setValue:@"application/x-www-form-urlencoded" forHTTPHeaderField:@"Content-Type"];

    NSString *authStr = [NSString stringWithFormat:@"%@:%@", [NSString stringWithUTF8String:username], [NSString stringWithUTF8String:password]];
    NSData* authData = [authStr dataUsingEncoding:NSASCIIStringEncoding];
    const char* as = b64_encode([authStr UTF8String], [authStr length]);
    NSString *authValue = [NSString stringWithFormat:@"Basic %@", [NSString stringWithUTF8String: as]];
    [mutreq addValue:authValue forHTTPHeaderField:@"Authorization"];


    NSString* postData = [NSString stringWithUTF8String:data];
    [mutreq setHTTPMethod: @"POST"];
    [mutreq setHTTPBody: [postData dataUsingEncoding:NSUTF8StringEncoding]];

    NSURLRequest* req = [mutreq copy];

    NSURLResponse* res = nil;

    NSError* err = nil;

    NSData* resdata = [NSURLConnection sendSynchronousRequest:req returningResponse:&res error:&err];

    free(as);

    Res_t* out_var = malloc(sizeof(struct Res));
    out_var->length = 0;

    if(err == nil){
        out_var->err = 0;

        out_var->length = [resdata length];

        const void* bytes = [resdata bytes];

        out_var->data = malloc(out_var->length);

        memcpy((char*) out_var->data, bytes, out_var->length);

        [pool release];

        return out_var;
    }

    out_var->err = 1;

    NSLog(@"%@", [err localizedDescription]);

    [pool release];

    return out_var;
}