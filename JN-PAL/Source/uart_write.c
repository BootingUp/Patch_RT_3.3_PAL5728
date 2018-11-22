#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>


#define	RS232A 	"/dev/ttyS8" // 7

int rs232_open(const char *dev_name)
{
	int ret = 0;
	struct termios rs232_uart = { 0 };
	int fd = open(dev_name,O_RDWR | O_NOCTTY);
	if(fd < 0){
		perror("Open");
		return -1;
	}
	
	//enable recv 
	rs232_uart.c_cflag |= CLOCAL | CREAD;
	
	//data 8bit
	rs232_uart.c_cflag &= ~CSIZE;
	rs232_uart.c_cflag |= CS8;

	/*Parity NONE*/
	rs232_uart.c_cflag &=~PARENB;

	/*Stop bit 1*/
	rs232_uart.c_cflag &=~CSTOPB;

	rs232_uart.c_cflag &=~CRTSCTS;
	rs232_uart.c_cc[VTIME] = 0;
	rs232_uart.c_cc[VMIN] = 0;
	
	cfsetispeed(&rs232_uart, B115200);
	cfsetospeed(&rs232_uart, B115200);
	
	tcflush(fd, TCIOFLUSH);

	ret = tcsetattr(fd, TCSANOW, &rs232_uart);
	if(ret < 0)
	{
		return -1;
	}
	return fd;
}

int main(int argc, const char *argv[])
{
	int ret = 0;
	unsigned char data[] = {'C'};//0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
	int uart_fd = rs232_open(RS232A);
	
	if ( uart_fd < 0 )
	{
		return 1;
	}
		
	while ( 1 )
	{
	   write(uart_fd, data, sizeof(data));
	 	sleep(1);
	}
	close(uart_fd);
	return 0;

}
