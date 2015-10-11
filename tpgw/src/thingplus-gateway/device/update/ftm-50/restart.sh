#!/bin/sh

if [ -x /etc/init.d/thingplus ]; then
  /etc/init.d/thingplus stop;
  sleep 5;
  /etc/init.d/thingplus start;
else
  sync; sleep 5; sync; reboot;
fi
