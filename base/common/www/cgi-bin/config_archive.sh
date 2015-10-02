#!/bin/sh

tar czvpf config.tar.gz /etc/httpd.conf /etc/service/ /etc/passwd /etc/network/interfaces /etc/resolv.conf /etc/udhcpd.conf
#password_str="futuretek_01"
#echo -n "$password_str" | openssl aes-256-cbc -in config.tar.gz -out config.dat -pass stdin
#rm config.tar.gz
#mv config.dat /tmp
mv config.tar.gz /tmp
