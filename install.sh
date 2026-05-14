#!/bin/sh
set -e

INSTALL_DIR=/usr/local/bin
BINARY=rib

if [ ! -f /usr/local/lib/libcortex-vm.a ] || [ ! -f /usr/local/include/cortex-vm.h ]; then
    echo "cortex-vm not found, installing..."
    tmp=$(mktemp -d)
    git clone https://github.com/jonahmer22/cortex-vm/ "$tmp/cortex-vm"
    cd "$tmp/cortex-vm"
    ./install.sh
    cd -
    rm -rf "$tmp"
    echo "cortex-vm installed."
else
    echo "cortex-vm already installed."
fi

echo "Building $BINARY..."
make -C "$(dirname "$0")"
echo "Installing $BINARY to $INSTALL_DIR..."
cp "$(dirname "$0")/$BINARY" "$INSTALL_DIR/$BINARY"
echo "Done. Run '$BINARY' to get started."
