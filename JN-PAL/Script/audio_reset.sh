#! /bin/sh
echo 174 > /sys/class/gpio/export

echo "out" > /sys/class/gpio/gpio174/direction

val=$(cat /sys/class/gpio/gpio174/value)
if [[ $val -eq 0 ]];then
	echo 1 > /sys/class/gpio/gpio174/value
else
	echo 0 > /sys/class/gpio/gpio174/value
fi
