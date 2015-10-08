#!/bin/sh

rm -rf /mnt/mtd/etc
sleep 3
cp -r /etc.init /mnt/mtd/etc
sleep 2
sync
reboot
