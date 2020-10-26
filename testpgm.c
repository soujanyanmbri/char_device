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

int8_t write_buf[1024];
int8_t read_buf[1024];

int main(int argc, char **argv){
	int fd;
	char c;
	fd=open("/dev/my_device",O_RDWR);
	if(fd<0){
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
		scanf("%c", &c );
		
		
		switch(c){
        case 'a':{
           printf(" Enter the string to write into the driver\n ");
           scanf(" %[^\t\n]s ",write_buf);
           printf(" Data Written\n ");
           write(fd, write_buf , strlen(write_buf)+1);
           printf(" Done\n ");
           break;
        }

        case 'b':{
           printf("Reading\n");
           read(fd,read_buf,1024);
           printf("Finished reading\n");
           printf("Data = %s\n\n",read_buf);
           break;
        }
        case 'c':{
           close(fd);
           exit(1);
           break;
        }
        default:{
           printf("Enter a valid option.\n");
           continue;
        }
			}
		}
		close(fd);
}
