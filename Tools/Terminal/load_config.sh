#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
CONFIG_FILE=$DIR/terminal-config
gnome-terminal --load-config=$CONFIG_FILE
