#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <linux/ioctl.h>
#include "ospfs.h"
#include <linux/kernel.h>
#include "md5.h"

int main(int argc, char** argv)
{
	int diskfd; //file descriptor
	//open a bs file to get the file descriptor
	diskfd = open("test/test.txt", O_RDWR | O_CREAT, 0666); 
	//check to make sure that no error
	if(diskfd < 0)  fprintf(stderr, "error: open failed\n");
	if(atoi(argv[1]) < -1)
	{
		fprintf(stderr, "error: argument cannot be less than -1\n");
		return 1;
	}
	//then do the ioctl command
	ioctl(diskfd, CRASH_TEST, atoi(argv[1]));
	close(diskfd);
	return 0;	
}
