#! /bin/sh
#uart9  gpio6_15
if [ ! -d "/sys/class/gpio/gpio175" ];then
echo 175 > /sys/class/gpio/export
fi

#uart10  gpio1_5
if [ ! -d "/sys/class/gpio/gpio5" ];then
echo 5 > /sys/class/gpio/export
fi

echo "out" > /sys/class/gpio/gpio175/direction
echo "out" > /sys/class/gpio/gpio5/direction

#high txd low rxd
echo 1 > /sys/class/gpio/gpio175/value
echo 0 > /sys/class/gpio/gpio5/value

