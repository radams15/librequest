#!/bin/bash

rm -rf build_univ
mkdir -p build_univ

ARCH='-arch x86_64 -arch i386' TARGET='-mmacosx-version-min=10.5 -isysroot /Developer/SDKs/MacOSX10.5.sdk' make build/librequest.dylib
mv build/librequest.dylib build_univ/librequest.dylib.x86

make clean

ARCH='-arch ppc' TARGET='-mmacosx-version-min=10.3.9 -isysroot /Developer/SDKs/MacOSX10.3.9.sdk' make build/librequest.dylib

mv build/librequest.dylib build_univ/librequest.dylib.ppc

lipo -create build_univ/librequest.dylib.x86 build_univ/librequest.dylib.ppc -output build_univ/librequest.dylib