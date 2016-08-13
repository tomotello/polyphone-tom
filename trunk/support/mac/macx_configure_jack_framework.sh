#!/bin/sh


: ${1?"Usage: $0 DESTDIR where DESTIR is either \"DEBUG\" or \"RELEASE\""}

export DESTDIR="$1"

echo "Configuring Jack framework  ..."
install_name_tool -change /System/Library/Frameworks/Jackmp.framework/Versions/A/Jackmp @executable_path/../Frameworks/Jackmp.framework/Versions/A/Jackmp "$DESTDIR"/Polyphone.app/Contents/MacOS/Polyphone 
echo "done"
