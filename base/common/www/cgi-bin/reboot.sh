#!/bin/sh

if [ -f /tmp/curData ]
then 
	initRx=`cat /etc/ppp/data/initData | awk '{ print $1 }'`
	initTx=`cat /etc/ppp/data/initData | awk '{ print $2 }'`
	currentRx=`cat /tmp/curData | awk '{ print $1 }'`
	currentTx=`cat /tmp/curData | awk '{ print $2 }'`
	sumRx=`expr $initRx + $currentRx`
	sumTx=`expr $initTx + $currentTx`
	`echo $sumRx $sumTx > /etc/ppp/data/initData`
fi

`echo AT#SHDN > /dev/ttyS1; sleep 0.1`
sleep 5 
sync
reboot
