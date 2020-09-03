#ifndef __Serial_H
#define __Serial_H
#include <stdio.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */ 
#include <errno.h>   /* ERROR Number Definitions           */
int Serial_Write(unsigned char X_Direct,unsigned char Y_Direct,unsigned char X_Speed,unsigned char Y_Speed)
{
    int fd;
    fd = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd == -1)
        return -1;

    struct termios SerialPortSettings;	/* Create the structure                          */
	tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */

	cfsetispeed(&SerialPortSettings,B115200); /* Set Read  Speed as 9600                       */
	cfsetospeed(&SerialPortSettings,B115200); /* Set Write Speed as 9600                       */

	SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
	SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
	SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
	SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */
	
	SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */ 
		
		
	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */

	SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/

	if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
		return -2;
	unsigned char write_cmd[]={0xFF,0xFE,X_Speed,X_Direct,Y_Speed,Y_Direct,0x00,0x00,0x00,0x00};
    int bytes_written = write(fd,write_cmd,10);
	close(fd);
    if(bytes_written==10)return 0;
    else return 1;
	
	
}
int Read_Serial(unsigned char *buffer)
{
	int fd;
	fd = open("/dev/ttyUSB0",O_RDONLY | O_NOCTTY);	/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
        if(fd == -1)return -1;
	struct termios SerialPortSettings;	/* Create the structure                          */
	tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */
	/* Setting the Baud rate */
	cfsetispeed(&SerialPortSettings,B115200);
	cfsetospeed(&SerialPortSettings,B115200);
	/* 8N1 Mode */
	SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
	SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
	SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
	SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */
	
	SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */ 
		
		
	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */

	SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/
	
	/* Setting Time outs */
	SerialPortSettings.c_cc[VMIN] = 255; /* Read at least 10 characters */
	SerialPortSettings.c_cc[VTIME] = 0; /* Wait indefinetly   */


	if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
		return -2;
	    /*------------------------------- Read data from serial port -----------------------------*/
	//tcflush(fd, TCIFLUSH);   /* Discards old data in the rx buffer            */
	int  bytes_read = 0;    /* Number of bytes read by the read() system call */
	bytes_read = read(fd,&buffer,1024); /* Read the data                   */			
	close(fd); /* Close the serial port */
	if(bytes_read==100)return 0;
	return 1;
}
#endif
