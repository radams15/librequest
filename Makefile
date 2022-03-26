
OS=darwin

ARCH=''

ifeq "${OS}" 'darwin' 
OBJ=o
LIB=dylib
#ARCH=-arch x86_64 -arch ppc
#TARGET=-mmacosx-version-min=10.5 -isysroot /Developer/SDKs/MacOSX10.5.sdk
CC=gcc
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

all: test_darwin

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



darwin/request.o:
	${CC} -fPIC ${ARCH} ${TARGET} -c darwin/request.m -o darwin/request.o -I.

res.o:
	${CC} -fPIC ${ARCH} ${TARGET} -c res.c -o res.o -I.

librequest.dylib: src/main/backends/darwin res.o
	${CC} -framework Foundation ${ARCH} ${TARGET} -dynamiclib darwin/request.o res.o -o librequest.dylib

test_darwin: librequest.dylib
	${CC} test.c -o test_darwin ${ARCH} ${TARGET} -L. -I. -lrequest

clean:
	rm -rf *.o *.so *.dll *.dylib test */*.o */*.obj
