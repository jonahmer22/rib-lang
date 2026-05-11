#!/bin/sh
set -e

INSTALL_DIR=/usr/local/bin
BINARY=rib-lang

if [ -f "$INSTALL_DIR/$BINARY" ]; then
    rm "$INSTALL_DIR/$BINARY"
    echo "$BINARY removed from $INSTALL_DIR."
else
    echo "$BINARY is not installed at $INSTALL_DIR."
fi
