This version of voltage modification for A3 and D3 allows to read out voltages only and gives better parseable output.

This tool allows voltage editing on Bitmain’s A3 and D3 hashboards, it is provided free by the developer so if you find this useful and/or want me to update/provide new tools please donate to the below address. Thanks!

BTC: 1593z3zwpmCEiKf7EeXzKx799nR25NWpxD

————

Usage:

login (Putty) to the miner and type:

curl -o AD3_MOD_FNT.sh http://forexneurotrader.com/AD3_MOD_FNT.sh

and then set permission for the script with

chmod a+x AD3_MOD_FNT.sh

start the script with

./AD3_MOD_FNT.sh

The script is going to download the miner, set the permission and start the (read) program. 

or you scp binary to /config directory in your unit’s controller…this is the only directly that gets saved on reboot on antminers!

binary accepts two inputs in the format of:

./AD3_MOD_FNT [chain# 1-3] [voltage in hex]

bitmains voltage controller can be configured to change the 12v input roughly +/- 1v from 10v, and this is configurable via a hex range of 0x00-0xfe, with the default being set to the upper middle (0xb8 A3!). Higher hex values (0xb8-0xfe) will LOWER voltage, lower values (0x00-0x7f) will INCREASE voltage from the default. Be carefull in testing! You could burn ASIC chips with wrong settings!

For example if you want to slightly decrease your voltage on chain #1 you would input:

./AD3_MOD_FNT 1 b9

increments of 0x10 are good starting point to test a sweet spot for each board for a particular frequency. Lowering voltage until you get around 1 HW error per minute is usually a good reference “sweet spot”
Also check hex table e.g. http://hextodecimal.com/index.php?hex=B8

If you only want to read out chain #1 just ommit the voltage:

./AD3_MOD_FNT 1

If you want to read out all chains' voltage, use:

./AD3_MOD_FNT

If you like this tool, send some coins to me and the original author jstefanop too.
