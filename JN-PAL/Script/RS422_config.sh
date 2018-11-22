#! /bin/sh
if [ ! -d "/sys/class/gpio/gpio167" ];then
echo 167 > /sys/class/gpio/export
fi

if [ ! -d "/sys/class/gpio/gpio61" ];then
echo 61 > /sys/class/gpio/export
fi

echo "out" > /sys/class/gpio/gpio167/direction
echo "out" > /sys/class/gpio/gpio61/direction

echo 1 > /sys/class/gpio/gpio167/value
echo 0 > /sys/class/gpio/gpio61/value

