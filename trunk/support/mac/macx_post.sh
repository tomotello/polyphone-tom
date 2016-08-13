#!/bin/sh

# Polyphone 
# QMAKE_POST_LINK script to automate post build deployment tasks
# 

command_exists () {

 which "$1" &> /dev/null;

}

: ${1?"Usage: $0 DESTDIR where DESTIR is either \"DEBUG\" or \"RELEASE\""}

export DESTDIR="$1"


if !  command_exists macdeployqt ; then
  echo "Qt environment not set up correctly, macdeployqt not found"
  exit 2 
fi


echo "Deploying qt dependencies ..."
macdeployqt "$DESTDIR"/Polyphone.app

echo "Deploying Jack framework ..."
cp -rp lib/mac/Jackmp.framework "$DESTDIR"/Polyphone.app/Contents/Frameworks
echo "Requesting admin password..."
/usr/bin/osascript -e 'do shell script "support/mac/macx_configure_jack_framework.sh $DESTDIR args 2>&1" with administrator privileges'
echo "all done"
