#!/bin/sh
fdisk shk.img << EOF
n
p
1
2048

a
1
w
EOF
