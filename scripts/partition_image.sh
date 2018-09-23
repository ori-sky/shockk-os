#!/bin/sh
fdisk shk-mbr.img << EOF
o
n
p
1
2048

a
1
w
EOF
