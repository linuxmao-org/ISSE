#
#Copyright 2013 Adobe Systems Incorporated
#Distributed under the terms of the Gnu General Public License
#
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.
#



export EXEC_FILE=$TARGET_BUILD_DIR/$TARGET_NAME.app/Contents/MacOS/$PRODUCT_NAME
export FRAMEWORKS_FOLDER=$TARGET_BUILD_DIR/$TARGET_NAME.app/Contents/Frameworks
export MKL_LIB="/opt/intel/mkl/lib"
export INTEL_LIB="/opt/intel/lib"
export USR_LOCAL_LIB="/usr/local/lib"
export USR_LIB="/usr/lib"
rm -rf $FRAMEWORKS_FOLDER
mkdir $FRAMEWORKS_FOLDER


export DYLIB=libiomp5.dylib
cp -f $INTEL_LIB/$DYLIB $FRAMEWORKS_FOLDER
install_name_tool -change $DYLIB @executable_path/../Frameworks/$DYLIB $EXEC_FILE



export DYLIB=libc++.1.dylib
export DYLIB2=libc++abi.dylib
cp -f /usr/lib/$DYLIB $FRAMEWORKS_FOLDER
chmod 755 $FRAMEWORKS_FOLDER/$DYLIB
install_name_tool -change /usr/lib/$DYLIB @executable_path/../Frameworks/$DYLIB $EXEC_FILE
install_name_tool -change /usr/lib/$DYLIB2 @executable_path/../Frameworks/$DYLIB2 $FRAMEWORKS_FOLDER/$DYLIB
chmod 555 $FRAMEWORKS_FOLDER/$DYLIB


export DYLIB=libstdc++.6.dylib
export DYLIB2=libc++abi.dylib
cp -f /usr/lib/$DYLIB $FRAMEWORKS_FOLDER
chmod 755 $FRAMEWORKS_FOLDER/$DYLIB
install_name_tool -change $USR_LIB/$DYLIB @executable_path/../Frameworks/$DYLIB $EXEC_FILE
install_name_tool -change /usr/lib/$DYLIB2 @executable_path/../Frameworks/$DYLIB2 $FRAMEWORKS_FOLDER/$DYLIB
chmod 555 $FRAMEWORKS_FOLDER/$DYLIB



#export DYLIB=libfftw3f.dylib
#cp -f $USR_LOCAL_LIB/$DYLIB $FRAMEWORKS_FOLDER
#chmod 555 $FRAMEWORKS_FOLDER/$DYLIB
#install_name_tool -change $USR_LOCAL_LIB/$DYLIB @executable_path/../Frameworks/$DYLIB $EXEC_FILE


export DYLIB=libc++abi.dylib
cp -f $USR_LIB/$DYLIB $FRAMEWORKS_FOLDER
chmod 755 $FRAMEWORKS_FOLDER/$DYLIB
install_name_tool -change $USR_LIB/$DYLIB @executable_path/../Frameworks/$DYLIB $EXEC_FILE
chmod 555 $FRAMEWORKS_FOLDER/$DYLIB

cp $PROJECT_DIR/../../resources/icons/FileIcon.icns $TARGET_BUILD_DIR/$TARGET_NAME.app/Contents/Resources




chmod 755 $EXEC_FILE

