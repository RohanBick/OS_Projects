#!/bin/bash


# Find the .c files (all) and archive them after running them
find . -type f -name "*.c" -print0 | xargs -0 tar -cvf allcfiles.tar --transform='s#.*/##'
