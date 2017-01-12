#!/bin/bash

make clean
DEBUGGING=true make build/x86_64/aura.efi
DEBUGGING=true make build/x86_64/debug.aura.efi

touch /tmp/osoutput
DEBUGGING=true make run
imageLocationPre=$(grep -m 1 -Po "(?<=(Image base: )).+" <( tail -f /tmp/osoutput ))
imageLocation=${imageLocationPre//$'\r'/}

relativeDataLocationPre=0x$(objdump build/x86_64/aura.efi -h -j .data | sed -n '6p' | awk -v N=4 '{print $N}')
relativeDataLocation=${relativeDataLocationPre//$'\r'/}
relativeTextLocationPre=0x$(objdump build/x86_64/aura.efi -h -j .text | sed -n '6p' | awk -v N=4 '{print $N}')
relativeTextLocation=${relativeTextLocationPre//$'\r'/}

dataLocation=$(($imageLocation + $relativeDataLocation))
textLocation=$(($imageLocation + $relativeTextLocation))

symbolCommand="add-symbol-file build/x86_64/debug.aura.efi $textLocation -s .data $dataLocation"

cat <<EOF > /tmp/debug.gdb
$symbolCommand
set architecture i386:x86-64:intel
target remote :1234
set variable wait = 0
EOF

gdb -x /tmp/debug.gdb

rm /tmp/debug.gdb
killall qemu-system-x86_64
