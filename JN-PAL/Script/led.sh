#! /bin/sh
echo 128 > /sys/class/gpio/export
echo 129 > /sys/class/gpio/export

echo 4 > /sys/class/gpio/export
echo 179 > /sys/class/gpio/export

echo "out" > /sys/class/gpio/gpio128/direction
echo "out" > /sys/class/gpio/gpio129/direction

echo "out" > /sys/class/gpio/gpio4/direction
echo "out" > /sys/class/gpio/gpio179/direction

echo 0 > /sys/class/gpio/gpio128/value
echo 0 > /sys/class/gpio/gpio129/value
echo 0 > /sys/class/gpio/gpio4/value
echo 0 > /sys/class/gpio/gpio179/value


sleep 5
echo 1 > /sys/class/gpio/gpio128/value
echo 1 > /sys/class/gpio/gpio129/value
echo 1 > /sys/class/gpio/gpio4/value
echo 1 > /sys/class/gpio/gpio179/value

echo 128 > /sys/class/gpio/unexport
echo 129 > /sys/class/gpio/unexport
echo 4 > /sys/class/gpio/unexport
echo 179 > /sys/class/gpio/unexport
