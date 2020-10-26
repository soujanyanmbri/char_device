#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/ioctl.h>

#define MAJOR_NUM 239

#define IOCTL_DEC _IO(MAJOR_NUM, 0)
#define IOCTL_ENC _IO(MAJOR_NUM, 1)

int8_t write_buffer[1024];
int8_t read_buffer[1024];

int main(int argc, char **argv){
	int fdesc;
	int c;
	
	fdesc = open("/dev/my_device",O_RDWR);
	if(fdesc < 0){
		printf("Cannot open device file\n");
		return 0;	
	}
	int t;
	switch(atoi(argv[1]))
	{
		case 0:
			t = ioctl(fdesc,IOCTL_DEC,0);
			break;
		case 1:
			t = ioctl(fdesc,IOCTL_ENC,0);
			break;
	}
    int temp;
	while(1){
		printf("Choose:\n");
		printf("1. Write\n");
		printf("2. Read\n");
		printf("3. Exit\n");
		scanf("%d ", &c);
		
		
		switch(c){
				case 1:
				{
					 printf("Enter the string to write into the driver\n");
					 scanf("%[^\t\n]s ",write_buffer);
					 printf("Data Written\n");
					 write(fdesc, write_buffer , strlen(write_buffer)+1);
					 printf("Done\n");
					 break;
				}
					 
				case 2:
				{
					 printf("Reading\n");
					 read(fdesc,read_buffer,1024);
					 printf("Finished reading\n");
					 printf("Data = %s\n\n",read_buffer);
					 break;
				}
				case 3:
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
