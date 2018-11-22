#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/videodev2.h>

struct v4l2_user_buffer {
    int index;
    int length;
    char *addr;
};
struct v4l2_user_buffer user_buffer[3];

int get_dev(char *video_5c, char *video_5d)
{                                                                                 
	char buf_5c[20] = { 0 };
        char buf_5d[20] = { 0 };
	if ( (video_5d != NULL) && (0 == access("/sys/bus/platform/devices/48970000.vip/video4linux/",F_OK)))
	{
	    FILE *fp_5d = popen("ls /sys/bus/platform/devices/48970000.vip/video4linux/", "r");    
	    if ( fp_5d != NULL )    
            {                                                                                      
		fread(buf_5d, sizeof(buf_5d), 1, fp_5d);                                       
		strncpy(video_5d, buf_5d, strlen(buf_5d) -1 );
		pclose(fp_5d);                                                                 
	   }                                                                                      
	}  
	if ( (video_5c != NULL ) && (0 == access("/sys/bus/platform/devices/48990000.vip/video4linux/",F_OK)))
	{                                                                                          
	    FILE *fp_5c = popen("ls /sys/bus/platform/devices/48990000.vip/video4linux/", "r");   
	    if ( fp_5c != NULL )   
	    {
	       fread(buf_5c, sizeof(buf_5c), 1, fp_5c);
	       strncpy(video_5c, buf_5c, strlen(buf_5c) -1);
	       pclose(fp_5c);     
	   }
       }

    if ( strlen(buf_5c) != 0 || strlen(buf_5d) != 0 )
    {
	return 0;
    }
    else
    {
	return -1;
    }

}


int save_file(char *addr, int length, int index)
{
    FILE *fp;
    char fname[256];

    memset(fname, 0x00, 256);
    sprintf(fname, "frame_%d.yuv", index);
    printf("save_file [%s]\n", fname);
    fp = fopen(fname, "wb");
    if (fp == NULL) {
        printf("%d: fopen error\n", __LINE__);
        return -1;
    }

    fwrite(addr, 1, length, fp);
    fflush(fp);
    fclose(fp);

    return 0;
}

int main(void)
{
    int i = 0;
    int status = 0;
    int v4l2_fd = 0;
    char videoname[20] = { 0 };
    char temp[10] = { 0 };
    struct v4l2_format format;
    struct v4l2_requestbuffers requestbuffers;
    struct v4l2_buffer buffer;
    status = get_dev(temp, NULL);
    if ( status < 0 )
    {
	return -1;
    }
    sprintf(videoname, "/dev/%s", temp);
    v4l2_fd = open(videoname, O_RDWR);
    if (v4l2_fd == -1) {
        printf("%d: open error \n", __LINE__);
        return -1;
    }

    memset(&format, 0x00, sizeof(struct v4l2_format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = 720;
    format.fmt.pix.height = 240;
    format.fmt.pix.field = V4L2_FIELD_ALTERNATE;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    status = ioctl(v4l2_fd, VIDIOC_S_FMT, &format);
    if (status < 0) {
        printf("%d: ioctl VIDIOC_S_FMT error\n", __LINE__);
        return -1;
    }

    memset(&format, 0x00, sizeof(struct v4l2_format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    status = ioctl(v4l2_fd, VIDIOC_G_FMT, &format);
    if (status < 0) {
        printf("%d: ioctl VIDIOC_G_FMT error\n", __LINE__);
        return -1;
    }
    printf("format width: [%d]\n", format.fmt.pix.width);
    printf("format height: [%d]\n", format.fmt.pix.height);

    memset(&requestbuffers, 0x00, sizeof(struct v4l2_requestbuffers));
    requestbuffers.count = 3;
    requestbuffers.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    requestbuffers.memory = V4L2_MEMORY_MMAP;
    status = ioctl(v4l2_fd, VIDIOC_REQBUFS, &requestbuffers);
    if (status < 0) {
        printf("%d: ioctl VIDIOC_REQBUFS error\n", __LINE__);
        return -1;
    }

    for (i = 0; i < requestbuffers.count; i++) {
        buffer.index = i;
        buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer.memory = V4L2_MEMORY_MMAP;
        status = ioctl(v4l2_fd, VIDIOC_QUERYBUF, &buffer);
        if (status < 0) {
            printf("%d: ioctl VIDIOC_QUERYBUF error\n", __LINE__);
            return -1;
        }

        user_buffer[i].index = i;
        user_buffer[i].length = buffer.length;
        user_buffer[i].addr = (char *)mmap(NULL,
                buffer.length,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                v4l2_fd,
                buffer.m.offset);
        if (user_buffer[i].addr == MAP_FAILED) {
            printf("%d: mmap error\n", __LINE__);
            return -1;
        }
        printf("user_buffer index: [%d]\n", user_buffer[i].index);
        printf("user_buffer length: [%d]\n", user_buffer[i].length);
        printf("user_buffer addr: [%p]\n", user_buffer[i].addr);
    }

    for (i = 0; i < requestbuffers.count; i ++) {
        buffer.index = i;
        buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer.memory = V4L2_MEMORY_MMAP;
        status = ioctl(v4l2_fd, VIDIOC_QBUF, &buffer);
        if (status < 0) {
            printf("%d: ioctl VIDIOC_QBUF error\n", __LINE__);
            return -1;
        }
    }

    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    status = ioctl(v4l2_fd, VIDIOC_STREAMON, &type);
    if (status < 0) {
        printf("%d: ioctl VIDIOC_STREAMOFF error\n", __LINE__);
        return -1;
    }

    for (i = 0 ; i < 10; i++) {
        buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer.memory = V4L2_MEMORY_MMAP;
        status = ioctl(v4l2_fd, VIDIOC_DQBUF, &buffer);
        if (status < 0) {
            printf("%d: ioctl VIDIOC_DQBUF error\n", __LINE__);
            return -1;
        }

        save_file(user_buffer[buffer.index].addr, user_buffer[buffer.index].length, i);

        buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer.memory = V4L2_MEMORY_MMAP;
        buffer.m.offset = (unsigned long)user_buffer[buffer.index].addr;
        status = ioctl(v4l2_fd, VIDIOC_QBUF, &buffer);
        if (status < 0) {
            printf("%d: ioctl VIDIOC_DQBUF error\n", __LINE__);
            return -1;
        }
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    status = ioctl(v4l2_fd, VIDIOC_STREAMOFF, &type);
    if (status < 0) {
        printf("%d: ioctl VIDIOC_STREAMOFF error\n", __LINE__);
        return -1;
    }

    close (v4l2_fd);

    return status;
}
