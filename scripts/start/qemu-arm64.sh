#!/bin/sh

kernel="workdir/build/ukproxy_qemu-arm64"
cmd=""

if test $# -eq 1; then
    kernel="$1"
fi

# Clean up any previous instances.
sudo pkill -f qemu-system
sudo pkill -f firecracker
sudo kraft stop --all
sudo kraft rm --all

qemu-system-aarch64 \
    -machine virt \
    -kernel "$kernel" \
    -nographic \
    -m 64M \
    -append "$cmd" \
    -cpu max
