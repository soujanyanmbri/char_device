#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

#define MAJOR_NUM 239

#define IOCTL_DEC _IO(MAJOR_NUM, 0)
#define IOCTL_ENC _IO(MAJOR_NUM, 1)

int8_t write_buffer[1024];
int8_t read_buffer[1024];

int main(int argc, char **argv){
	int fdesc;
	char c;
	
	fdesc = open("/dev/my_device",O_RDWR);
	if(fdesc < 0){
		printf("Cannot open device file\n");
		return 0;	
	}
	int t;
	switch(atoi(argv[1]))
	{
		case 0:
			t = ioctl(fd,IOCTL_DEC,0);
			break;
		case 1:
			t = ioctl(fd,IOCTL_ENC,0);
			break;
	}

	while(1){
		printf(" Choose:\n ");
		printf(" a. Write\n ");
		printf(" b. Read\n");
		printf(" c. Exit\n");
		scanf(" %c ", &c );
		
		
		switch(c){
				case 'a':
				{
					 printf(" Enter the string to write into the driver\n ");
					 scanf(" %[^\t\n]s ",write_buffer);
					 printf(" Data Written\n ");
					 write(fdesc, write_buffer , strlen(write_buffer)+1);
					 printf(" Done\n ");
					 break;
				}
					 
				case 'b':
				{
					 printf("Reading\n");
					 read(fdesc,read_buffer,1024);
					 printf("Finished reading\n");
					 printf("Data = %s\n\n",read_buffer);
					 break;
				}
				case 'c':
				{
					 close(fdesc);
					 exit(1);
					 break;
				}
				default:
				{
					printf("Enter a valid option!!\n");
					continue;
				}
			}
		}
		close(fdesc);
}
