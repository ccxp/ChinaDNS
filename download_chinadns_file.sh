#!/bin/sh

curl 'http://ftp.apnic.net/apnic/stats/apnic/delegated-apnic-latest' | awk -F\| '/CN\|ipv4/ { printf("%s/%d\n", $4, 32-log($5)/log(2)) }' > /etc/chinadns_chnroute.txt1
wget -O /etc/gfwlist.txt1 --no-check-certificate "https://raw.githubusercontent.com/gfwlist/gfwlist/master/gfwlist.txt"

CNT1=`cat /etc/chinadns_chnroute.txt1 | wc -l`
CNT2=`cat /etc/gfwlist.txt1 | wc -l`
if [ $CNT1 -gt 4000 ]
then
  echo mv /etc/chinadns_chnroute.txt1 /etc/chinadns_chnroute.txt
  mv /etc/chinadns_chnroute.txt1 /etc/chinadns_chnroute.txt
fi

if [ $CNT2 -gt 1000 ]
then
  echo mv /etc/gfwlist.txt1 /etc/gfwlist.txt
  mv /etc/gfwlist.txt1 /etc/gfwlist.txt
fi

/etc/init.d/chinadns restart