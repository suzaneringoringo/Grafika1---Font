#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

int main() {
	int fbfd = 0;
  struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
  long int screensize = 0;
  char *fbp = 0;
  int x = 0, y = 0;
  long int location = 0;

  // Open the file for reading and writing
  fbfd = open("/dev/fb0", O_RDWR);
  if (fbfd == -1) {
  	perror("Error: cannot open framebuffer device");
		exit(1);
	}
	printf("The framebuffer device was opened successfully.\n");

	// Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
		perror("Error reading fixed information");
		exit(2);
	}

	// Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		perror("Error reading variable information");
		exit(3);
	}

	printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	// Map the device to memory
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
			fbfd, 0);
	if ((int)fbp == -1) {
		perror("Error: failed to map framebuffer device to memory");
		exit(4);
	}
	printf("The framebuffer device was mapped to memory successfully.\n");

	//read char
	int charlength = 60;
	int charheight = 60;

	char **pixelmap = (char **)malloc(charheight * sizeof(char *));
	for (int i=0; i<charheight; i++)
		pixelmap[i] = (char *)malloc(charlength * sizeof(char));
	
	FILE *charmap = fopen("J.txt", "r");

	for (int i = 0; i < charheight; i++) {
		fscanf (charmap, "%s", pixelmap[i]);
	}

	fclose;

	// Figure out where in memory to put the pixel
	for (y = 100; y < 160; y++)
		for (x = 100; x < 160; x++) {
			location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
					(y+vinfo.yoffset) * finfo.line_length;

			if (vinfo.bits_per_pixel == 32) {
				if (pixelmap[y-100][x-100] == '*') {
					*(fbp + location) = 255;        // Some blue
					*(fbp + location + 1) = 255;     // A little green
					*(fbp + location + 2) = 255;    // A lot of red
					*(fbp + location + 3) = 0;      // No transparency
				} else {
					*(fbp + location) = 0;        // Some blue
					*(fbp + location + 1) = 0;     // A little green
					*(fbp + location + 2) = 0;    // A lot of red
					*(fbp + location + 3) = 0;      // No transparency
				}
			}
		}
	
	charmap = fopen("M.txt", "r");

	for (int i = 0; i < charheight; i++) {
		fscanf (charmap, "%s", pixelmap[i]);
	}

	for (y = 100; y < 160; y++)
		for (x = 200; x < 260; x++) {
			location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
					(y+vinfo.yoffset) * finfo.line_length;

			if (vinfo.bits_per_pixel == 32) {
				if (pixelmap[y-100][x-200] == '*') {
					*(fbp + location) = 255;        // Some blue
					*(fbp + location + 1) = 255;     // A little green
					*(fbp + location + 2) = 255;    // A lot of red
					*(fbp + location + 3) = 0;      // No transparency
				} else {
					*(fbp + location) = 0;        // Some blue
					*(fbp + location + 1) = 0;     // A little green
					*(fbp + location + 2) = 0;    // A lot of red
					*(fbp + location + 3) = 0;      // No transparency
				}
			}
		}
	
	fclose;

	munmap(fbp, screensize);

	close(fbfd);
	
	return 0;

 }