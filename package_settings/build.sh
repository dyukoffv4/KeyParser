#!/usr/bin/bash

PACKAGE_PATH=build/keyparser
INCLUDE_PATH=include/keyparser
LIB_FILEPATH=build/cmake/libkeyparser.so

cd $(dirname $(realpath $0))/..

if [[ ! -e $LIB_FILEPATH ]]
then
    echo "Shared library '${LIB_FILEPATH}' does not exist!"
    exit 1
fi

mkdir -p $PACKAGE_PATH/usr/local/include
mkdir -p $PACKAGE_PATH/usr/local/lib
mkdir -p $PACKAGE_PATH/DEBIAN

cp -r $INCLUDE_PATH $PACKAGE_PATH/usr/local/include
cp $LIB_FILEPATH $PACKAGE_PATH/usr/local/lib
cp package_settings/control $PACKAGE_PATH/DEBIAN

for i in $(objdump -p $LIB_FILEPATH | awk '/NEEDED/{print $2}')
do
    STRING=$(dpkg -S "$i" | awk '{print $1}' | head -1)
    DEPENDS="$DEPENDS, ${STRING%:*:}"
done

INSTALL_SIZE=$(($(du -k $INCLUDE_PATH | awk '{print $1}') + $(du -k $LIB_FILEPATH | awk '{print $1}')))

echo "Depends: ${DEPENDS:2}" >> $PACKAGE_PATH/DEBIAN/control
echo "Installed-Size: ${INSTALL_SIZE}" >> $PACKAGE_PATH/DEBIAN/control

dpkg-deb --build ./$PACKAGE_PATH
