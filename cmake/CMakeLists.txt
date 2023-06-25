cmake_minimum_required(VERSION 3.0)
project(LKeyParser VERSION 0.1.0)

set(Library lkeyparser)
set(SourceFile ../src/key.cpp ../src/tasks.cpp ../src/parser.cpp)
set(HeaderFile ../src/defines.hpp ../src/key.hpp ../src/tasks.hpp ../src/parser.hpp)
set(SharedDest /usr/local/lib)
set(HeaderDest /usr/local/include/${Library})

message("[${SourceFile}]")

add_library(${Library} SHARED ${SourceFile})

install(TARGETS ${Library} DESTINATION ${SharedDest})
install(FILES ${HeaderFile} DESTINATION ${HeaderDest})

set(CPACK_PACKAGE_NAME ${Library})
set(CPACK_PACKAGE_VENDOR "MyCompany")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "dyukoffv4 mycontacts@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION "The most stupid program ever written")

set(CPACK_GENERATOR "DEB")

include(CPack)

#     PKG_LIB=$PPATH/usr/local/lib
#     PKG_INC=$PPATH/usr/local/include/$LNAME
#     LIBRARY=$LPATH/lib$LNAME.so

#     mkdir -p $PPATH/DEBIAN
#     mkdir -p $PKG_LIB
#     mkdir -p $PKG_INC

#     cp $LIBRARY $PKG_LIB
#     cp src/defines.hpp $PKG_INC
#     cp src/key.hpp $PKG_INC
#     cp src/parser.hpp $PKG_INC
#     cp src/tasks.hpp $PKG_INC

#     for i in $(objdump -p $LIBRARY | awk '/NEEDED/{print $2}'); do
#         STRING=$(dpkg -S "$i" | awk '{print $1}' | head -1)
#         DEB_DEPN="$DEB_DEPN, ${STRING%:*:}"
#     done

#     {
#         echo "Package: $LNAME"
#         echo "Version: 1.0.0"
#         echo "Section: unknown"
#         echo "Priority: optional"
#         echo "Architecture: amd64"
#         echo "Essential: no"
#         echo "Installed-Size: $(($(du -k $PKG_INC | awk '{print $1}') + $(du -k $LIBRARY | awk '{print $1}')))"
#         echo "Depends: ${DEB_DEPN:2}"
#         echo "Maintainer: dyukoffv4 <net998art@gmail.com>"
#         echo "Description: Light Key Parsing DL for C++ applications"
#     } > $PPATH/DEBIAN/control

#     dpkg-deb --build ./$PPATH