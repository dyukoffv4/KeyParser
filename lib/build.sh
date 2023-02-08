#!/usr/bin/bash
if [ $# != 2 ]; then exit 1; fi
LNAME=$1; PNAME=$2; ELNAME=lib$1.so


# LIBRARY ASSEMBLY
echo "Library assembly ..."

if [[ $(make NAME=$LNAME) == "return 0" && -e $PNAME.deb ]]
then echo "Current package is actual"; exit 1
fi

# PACKAGE CREATING
echo "Package creating ..."

PKG_DEB=$PNAME/DEBIAN
PKG_LIB=$PNAME/usr/local/lib
PKG_INC=$PNAME/usr/local/include/$LNAME

DEB_MAIN="Maintainer: dyukoffv4 <net998art@gmail.com>"
DEB_DESC="Description: Key Parsing DL for C++ applications"

mkdir -p $PKG_DEB
mkdir -p $PKG_LIB
mkdir -p $PKG_INC

echo "Package: $LNAME"   > $PKG_DEB/control
echo "Version: 1.0"         >> $PKG_DEB/control
echo "Section: unknown"     >> $PKG_DEB/control
echo "Priority: optional"   >> $PKG_DEB/control
echo "Architecture: amd64"  >> $PKG_DEB/control
echo "Essential: no"        >> $PKG_DEB/control

# DEB_DEPN="Depends: "
# for i in $(objdump -p bin/$LIBRARY | awk '/NEEDED/{print $2}')
# do DEB_DEPN="$DEB_DEPN$(echo $i | sed 's/.so.//g'), "
# done
# echo $DEB_DEPN >> $PKG_DEB/control

DEB_SIZE=$(du -k bin/$ELNAME | awk '{print $1}')
for i in $(ls src | awk '/.hpp$/')
do DEB_SIZE=$(( $(du -k src/$i | awk '{print $1}') + $DEB_SIZE ))
done
echo "Installed-Size: $DEB_SIZE" >> $PKG_DEB/control

echo $DEB_MAIN >> $PKG_DEB/control
echo $DEB_DESC >> $PKG_DEB/control

cp bin/$ELNAME $PKG_LIB
cp src/terminal.hpp $PKG_INC
cp src/key.hpp $PKG_INC
cp src/defines.hpp $PKG_INC

dpkg-deb --build ./$PNAME
rm -rf $PNAME

exit 0
