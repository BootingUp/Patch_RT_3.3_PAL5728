#! /bin/bash

##define log file and turn off kernel print
LOG=$HOME/mksd-am57xx.log
echo 3 > /proc/sys/kernel/printk
print_failure ()
{
    echo "mksd-am57xx failure !!!!!!"
    exit 1
}

print_success ()
{
    echo "mksd-am57xx success"
    exit 0
}

if ! [[ -e $1 ]]; then
    echo "$1 is not exist"
    print_failure
fi
if ! [[ -e $2 ]]; then
    echo "$2 is not exist"
    print_failure
fi
if ! [[ -e $3 ]]; then
    echo "$3 is not exist"
    print_failure
fi
if ! [[ -e $4 ]]; then
    echo "$4 is not exist"
    print_failure
fi
if ! [[ -e $5 ]]; then
    echo "$5 is not exist"
    print_failure
fi
if ! [[ -e $6 ]]; then
    echo "$6 is not exist"
    print_failure
fi

echo "All data will be cleared [y/n]"
read ANSWER
if [[ $ANSWER == "y" ]]; then
    sleep 2
else
    print_failure
fi

## step 1
echo "Upload the partition"
DISK=$1
PARTITION1=$(df | grep $DISK | awk '{if(NR==1) print $1}')
PARTITION2=$(df | grep $DISK | awk '{if(NR==2) print $1}')
PARTITION3=$(df | grep $DISK | awk '{if(NR==3) print $1}')
#echo $PARTITION1
#echo $PARTITION2
#echo $PARTITION3
if [[ -b $PARTITION1 ]]; then
    umount $PARTITION1
fi
if [[ -b $PARTITION2 ]]; then
    umount $PARTITION2
fi
if [[ -b $PARTITION3 ]]; then
    umount $PARTITION3
fi

## step 2
echo "Create the partition"
dd if=/dev/zero of=$DISK bs=1024 count=1024 >>$LOG 2>&1
if ! [[ $? -eq 0 ]]; then
    echo "dd $DISK failure"
    print_failure
fi
parted -s $DISK mklabel msdos
if ! [[ $? -eq 0 ]]; then
    echo "parted $DISK failure"
    print_failure
fi
parted -s $DISK unit cyl mkpart primary fat32 -- 0 10
if ! [[ $? -eq 0 ]]; then
    echo "parted $DISK failure"
    print_failure
fi
parted -s $DISK set 1 boot on
if ! [[ $? -eq 0 ]]; then
    echo "parted $DISK failure"
    print_failure
fi
parted -s $DISK unit cyl mkpart primary ext4 -- 10 -2
if ! [[ $? -eq 0 ]]; then
    echo "parted $DISK failure"
    print_failure
fi

## step 3
echo "Format the partition"
BOOT_PARTITION=$(ls $DISK* | grep -v "boot" | awk '{if(NR==2) print}')
ROOTFS_PARTITION=$(ls $DISK* | grep -v "boot" | awk '{if(NR==3) print}')
#echo $BOOT_PARTITION
#echo $ROOTFS_PARTITION
mkfs.vfat -F 32 -n boot $BOOT_PARTITION >>$LOG 2>&1
if ! [[ $? -eq 0 ]]; then
    echo "mkfs $BOOT_PARTITION failure"
    print_failure
fi
mkfs.ext4 -L rootfs $ROOTFS_PARTITION >>$LOG 2>&1
if ! [[ $? -eq 0 ]]; then
    echo "mkfs $ROOTFS_PARTITION failure"
    print_failure
fi

## step 4
echo "Fill up the partition"
mkdir -p /mnt/boot
mkdir -p /mnt/rootfs
mount $BOOT_PARTITION /mnt/boot
if ! [[ $? -eq 0 ]]; then
    echo "mount $BOOT_PARTITION failure"
    print_failure
fi
mount $ROOTFS_PARTITION /mnt/rootfs
if ! [[ $? -eq 0 ]]; then
    echo "mount $ROOTFS_PARTITION failure"
    print_failure
fi

cp $2 /mnt/boot
cp $3 /mnt/boot
tar -xvf $6 -C /mnt/rootfs >>$LOG 2>&1
if ! [[ $? -eq 0 ]]; then
    echo "Fill up the partition $ROOTFS_PARTITION failure"
    print_failure
fi
cp $4 /mnt/rootfs/boot
cp $5 /mnt/rootfs/boot

sync
umount $BOOT_PARTITION
if ! [[ $? -eq 0 ]]; then
    echo "umount $BOOT_PARTITION failure"
    print_failure
fi
umount $ROOTFS_PARTITION
if ! [[ $? -eq 0 ]]; then
    echo "umount $ROOTFS_PARTITION failure"
    print_failure
fi
rm -rf /mnt/boot
rm -rf /mnt/rootfs
print_success
