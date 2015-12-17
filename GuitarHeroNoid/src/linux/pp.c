#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <linux/ppdev.h>
#include <linux/parport.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(int argc, char** argv)
{
   printf("Sending %s...\n", argv[1]);
   int fd = open("/dev/parport0", O_WRONLY);
   if (fd == -1)
     printf("Error: %s\n", strerror(errno));

   if (ioctl (fd, PPCLAIM)) {
      perror ("PPCLAIM");
     return 1;
   }

   /* Set the data lines */
   unsigned char data = atoi(argv[1]);
   ioctl (fd, PPWDATA, &data);
}
