#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>


#define	UART_DEV 	"/dev/ttyS0" // 7


int uart_open(const char *dev_name)
{
	int ret = 0;
	struct termios uart_opt = { 0 };
	int fd = open(dev_name,O_RDWR | O_NOCTTY);
	if(fd < 0){
		perror("Open");
		return -1;
	}
	
	//enable recv 
	uart_opt.c_cflag |= CLOCAL | CREAD;
	
	//data 8bit
	uart_opt.c_cflag &= ~CSIZE;
	uart_opt.c_cflag |= CS8;

	/*Parity NONE*/
	uart_opt.c_cflag &=~PARENB;

	/*Stop bit 1*/
	uart_opt.c_cflag &=~CSTOPB;

	uart_opt.c_cflag &=~CRTSCTS;
	uart_opt.c_cc[VTIME] = 6;
	uart_opt.c_cc[VMIN] = 0;
	
	cfsetispeed(&uart_opt, B115200);
	cfsetospeed(&uart_opt, B115200);
	
	tcflush(fd, TCIOFLUSH);

	ret = tcsetattr(fd, TCSANOW, &uart_opt);
	if(ret < 0)
	{
		return -1;
	}
	return fd;
}


int main(int argc, const char *argv[])
{
	int ret = 0;
	unsigned char  buf[512] = { 0 };
	int uart_fd = uart_open(UART_DEV);
	
	if ( uart_fd < 0 )
	{
		return 1;
	}
	
	memset(buf, 0x00, sizeof(buf));
	write(uart_fd, "12345678900987654321", 20);		
	while (1)
	{	
		ret = read(uart_fd, buf, sizeof(buf));
		if (ret == 0 )
			continue;
		if ( ret < 0 )
		    break;
		if ( ret > 0 )
		{
			printf("%s\n", buf);
			break;
		}
	}
	close(uart_fd);
	return 0;

}
