#!/bin/bash
SAVED=$(echo $1 | cut -d '/' -f2)
SAVED=$(echo $SAVED | cut -d '.' -f1)
SAVED="${SAVED}.txt"
./compile $1 > "output/"$SAVED

