#! /bin/sh
echo 133 > /sys/class/gpio/export
echo 137 > /sys/class/gpio/export

echo 132 > /sys/class/gpio/export
echo 134 > /sys/class/gpio/export

echo "out" > /sys/class/gpio/gpio133/direction
echo "in" > /sys/class/gpio/gpio137/direction

echo "out" > /sys/class/gpio/gpio132/direction
echo "in" > /sys/class/gpio/gpio134/direction

echo 0 > /sys/class/gpio/gpio132/value
echo 0 > /sys/class/gpio/gpio133/value

echo 1 > /sys/class/gpio/gpio133/value
cat /sys/class/gpio/gpio137/value

echo 1 > /sys/class/gpio/gpio132/value
cat /sys/class/gpio/gpio134/value


echo 132 > /sys/class/gpio/unexport
echo 133 > /sys/class/gpio/unexport
echo 134 > /sys/class/gpio/unexport
echo 137 > /sys/class/gpio/unexport
