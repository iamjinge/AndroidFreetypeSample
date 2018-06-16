# build freetype for android 

## freetype version 2.9.1

### build toolchain first, assume toolchain in /tmp/ndk/17
```
/Users/elton/Library/Android/sdk/ndk-bundle/build/tools/make-standalone-toolchain.sh --platform=android-17 --install-dir=/tmp/ndk/17/ --arch=arm --force
```

### export some variable
```
export PATH=$PATH:/tmp/ndk/17/bin
export CC=arm-linux-androideabi-gcc
export CXX=arm-linux-androideabi-g++
```

### config freetype
```
./configure --host=arm-linux-androideabi --prefix=/freetype --without-zlib --with-png=no --with-harfbuzz=no
```

### make and install. output files are in ./freetype/*
```
make -j$(nproc)
make install DESTDIR=$(pwd)
```