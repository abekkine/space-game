 echo "#ifndef VERSION_H_" > version.h
 echo "#define VERSION_H_" >> version.h
 echo >> version.h
 VSTR=$(git describe --dirty=! --always --tags)
 echo "#define VERSION_STR \"$VSTR\"" >> version.h
 echo >> version.h
 echo "#endif // VERSION_H_" >> version.h

