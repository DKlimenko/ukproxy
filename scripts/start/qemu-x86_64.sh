#!/bin/sh

kernel="workdir/build/ukproxy_qemu-x86_64"
cmd="192.168.122.1 9999"

if test $# -eq 1; then
    kernel="$1"
fi

# Clean up any previous instances.
sudo pkill -f qemu-system
sudo pkill -f firecracker
sudo kraft stop --all
sudo kraft rm --all
#kill -9 `sudo lsof -t -i:8899`

# Remove previously created network interfaces.
#sudo ip link set dev tap0 down 2> /dev/null
#sudo ip link del dev tap0 2> /dev/null
#sudo ip link set dev virbr10 down 2> /dev/null
#sudo ip link del dev virbr10 2> /dev/null

# Create bridge interface for QEMU networking.
#sudo ip link add dev virbr10 type bridge
#sudo ip address add 172.44.0.1/24 dev virbr10
#sudo ip link set dev virbr10 up
#systemctl stop firewalld

#nc -l -s 172.44.0.1 -p 8899 > file.out &

sudo qemu-system-x86_64 \
    -kernel "$kernel" \
    -nographic \
    -m 64M \
    -netdev bridge,id=en0,br=virbr0 \
    -device virtio-net-pci,netdev=en0 \
    -append "netdev.ipv4_addr=192.168.122.32 netdev.ipv4_gw_addr=192.168.122.1 netdev.ipv4_subnet_mask=255.255.255.0 -- $cmd" \
    -cpu max
