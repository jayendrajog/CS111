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
#include "md5.h"
#include <linux/kernel.h>

int main(int argc, char ** argv)
{
	int diskfd = open("test/test.txt", O_RDWR | O_CREAT, 0666);
	if(diskfd < 0) fprintf(stderr, "ERROR: open failed\n");
	if(atoi(argv[1]) < -1){
		fprintf(stderr, "ERROR: argument cannot be less than -1");
		return 1;
	}
	ioctl(diskfd, CRASH_TEST, atoi(argv[1]));
	close(diskfd);
	return 0;
}
