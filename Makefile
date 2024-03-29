
OS=darwin

ifeq "${OS}" 'darwin' 
OBJ=o
LIB=dylib
ARCH ?= -arch ppc
TARGET ?= -mmacosx-version-min=10.4 -isysroot /Developer/SDKs/MacOSX10.4u.sdk
CC=gcc --std=gnu99
endif

ifeq "${OS}" 'win32'
OBJ=obj
LIB=dll
CC=i686-w64-mingw32-gcc
endif

ifeq "${OS}" 'linux'
CC=gcc
OBJ=o
LIB=so
endif

all: build/test_darwin

header:
	gcc -E include/request.h -Iinclude/  | sed 's/^#.*//g' > libreq.h

win32/request.obj:
	${CC} -fPIC -c win32/request.cpp -o win32/request.obj -I.

res.obj:
	${CC} -fPIC -c res.c -o res.obj -I.

librequest.dll: src/main/backends/win32 res.obj
	${CC} -shared win32/request.obj res.obj -o librequest.dll -lwininet


test_win32.exe: librequest.dll
	${CC} test.c -o test_win32.exe -L. -I. -lrequest

b64:
	mkdir -p build
	cd build && ${CC} -fPIC ${ARCH} ${TARGET} -c ../lib/b64.c/buffer.c ../lib/b64.c/encode.c ../lib/b64.c/decode.c -Ilib/b64.c
	#cd build && ${CC} -fPIC -c ../lib/b64.c/buffer.c ../lib/b64.c/encode.c ../lib/b64.c/decode.c -Ilib/b64.c

build/request.o:
	mkdir -p build
	${CC} -fPIC ${ARCH} ${TARGET} -c src/main/backends/darwin/request.m -o build/request.o -Iinclude -Ilib/b64.c
	#${CC} -fPIC -c src/main/backends/curl/request.c -o build/request.o -Iinclude -Ilib/b64.c `curl-config --libs --cflags` -DLOCAL_CERT

build/res.o:
	mkdir -p build
	cd build && ${CC} -fPIC  ${ARCH} ${TARGET} -c ../src/main/res.c ../src/main/header.c ../src/main/request_generic.c -I../include

build/librequest.dylib: build/res.o build/request.o b64
	cd build && ${CC} -framework Foundation ${ARCH} ${TARGET} -dynamiclib *.o -o librequest.dylib
	#cd build && ${CC} ${ARCH} ${TARGET} -dynamiclib *.o -o librequest.dylib `curl-config --libs --cflags`
	
build/librequest.so: build/res.o build/request.o b64
	cd build && ${CC} -shared *.o -o librequest.so `curl-config --libs --cflags`

build/librequest.a: build/res.o build/request.o b64
	cd build && ${CC} -framework Foundation ${ARCH} ${TARGET} *.o -o librequest.a


build/test_darwin: build/librequest.dylib
	cd build && ${CC} ../src/main/test.c -o test_darwin ${ARCH} ${TARGET} -L. -Isrc/main/ -I../include -lrequest
	
build/test_linux: build/librequest.so
	cd build && ${CC} ../src/main/test.c -o test_linux -L. -Isrc/main/ -I../include -lrequest  `curl-config --libs --cflags`

clean:
	rm -rf *.o *.so *.dll *.dylib test */*.o */*.obj build
