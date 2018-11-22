#! /bin/sh 
DEV="/sys/bus/platform/devices/48990000.vip/video4linux"
if [ ! -d $DEV ];then
    exit 1
fi

videoname=$(ls $DEV)
gst-launch-1.0 -v v4l2src device=/dev/$videoname io-mode=4 ! 'video/x-raw, format=(string)YUY2, width=(int)720, height=(int)288, framerate=(fraction)60/1' ! vpe num-input-buffers=8 ! 'video/x-raw, format=(string)NV12, width=(int)1280, height=(int)720, framerate=(fraction)60/1'! queue ! waylandsink
