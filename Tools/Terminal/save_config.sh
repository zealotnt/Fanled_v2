#!/usr/bin/env bash

# [Ref](http://askubuntu.com/questions/310705/some-fast-way-to-save-and-restore-tabs-of-terminal)
# Prequisities:
# xdotool

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
DEST_FILE=$DIR/terminal-config
gnome-terminal --save-config=$DEST_FILE

LINES=($(grep -n '\[Terminal' $DEST_FILE | cut -d: -f1))
for ((i=0; i<$(grep '\[Terminal' $DEST_FILE | wc -l); i++))
do
    TITLE=$(xprop -id $WINDOWID WM_NAME | sed -e 's/WM_NAME(STRING) = "//' -e 's/"$//';xdotool key ctrl+Page_Down;)
    sed -ri "$((${LINES[$i]}+$i))s/.*/&\nTitle=$TITLE/" $DEST_FILE
done