
#!/bin/sh

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 
   exit 1
fi

rm /run/media/luisky/rootfs/root/K2000_bbb /run/media/luisky/rootfs/root/counter /run/media/luisky/rootfs/root/gensig
cp K2000_bbb /run/media/luisky/rootfs/root/K2000_bbb
cp counter /run/media/luisky/rootfs/root/counter
cp gensig /run/media/luisky/rootfs/root/gensig
umount /run/media/luisky/boot /run/media/luisky/rootfs

if [ $? = 1 ]
then
        echo "error: is SD card plugged in ?"
        exit 1
fi

echo "K2000, counter, gensig updated on SD card, boot & rootfs partitions unmounted"

exit 0