#!/bin/sh

echo change dir to /config
cd /config
echo start downloading file...
curl -o AD3_MOD_FNT http://forexneurotrader.com/AD3_MOD_FNT
echo Download done, setting permission
chmod a+x AD3_MOD_FNT
echo Start programm...
./AD3_MOD_FNT


