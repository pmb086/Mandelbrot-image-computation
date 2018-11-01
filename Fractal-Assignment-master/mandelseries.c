#include "bitmap.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int iteration_to_color( int i, int max );
int iterations_at_point( double x, double y, int max );
void compute_image( struct bitmap *bm, double xmin, double xmax, double ymin, double ymax, int max );
void forker(int nprocesses);

int main(void){
/*Accept the number of processes from the command line*/
int n=0;
// These are the default configuration values used
	

	//char *outfile = "mandel.bmp";
	double xcenter = 0.265000;
	double ycenter = 0.003500;
	double scale = 2;
	int    image_width = 500;
	int    image_height = 500;
	int    max = 1000;
        pid_t pid;
        int status;
       // int fi=0; 
        int counter=0,j=0,i=0;
if(counter>49)
{
  printf("counter exceeded, program exiting");
  exit(0);
}

void mandel(){
if(counter>49)
  exit(0);
char outfile[50];
 sprintf(outfile,"mandel%d.bmp",counter+1);

printf("mandel: x=%lf y=%lf scale=%lf max=%d outfile=%s\n",xcenter,ycenter,scale,max,outfile);

// Create a bitmap of the appropriate size.
	struct bitmap *bm = bitmap_create(image_width,image_height);

// Fill it with a dark blue, for debugging
	bitmap_reset(bm,MAKE_RGBA(0,0,255,0));


// Compute the Mandelbrot image
compute_image(bm,xcenter-scale,xcenter+scale,ycenter-scale,ycenter+scale,max);

  

if(!bitmap_save(bm,outfile)) {
		fprintf(stderr,"mandel: couldn't write to %s: %s\n",outfile,strerror(errno));
		
	}
}
//fork n times
void forker(int nprocesses)
{
    pid_t pid;

    if(nprocesses > 0)
    {
        if ((pid = fork()) < 0)
        {
            perror("fork error");
        }
        else if (pid == 0)
        {
           
                if(counter>49)
                exit(0);
                if(counter>0)
                scale*=0.75;
                mandel();
                counter++;
               
            //Child stuff here
           
        }
        else if(pid > 0)
        {
           while(n>0){
            //parent
            wait(&status);
            n--;
          }

        }
         forker(nprocesses - 1);
    }
}
scanf("%d",&n);
for(i=0;i<50;i+=n)
 forker(n);

/*for(i=0;i<50;i+=n)
{  
  for(j=0;j<n;j++){
     
 if((pid=fork())<0){

    exit(1);
}
else if(pid==0){
  if(counter==50)
    exit(0);
  if(counter>0)
  
  scale*=0.75;
  mandel();
  counter++;

  
}
else{
     wait(&status);
     if(counter==50)
     exit(0);
  }

}
}*/


/*else{
     pid=wait(&status);
     fi++;
  }

 j++;

}*/

return 0;

}
/*
Compute an entire Mandelbrot image, writing each point to the given bitmap.
Scale the image to the range (xmin-xmax,ymin-ymax), limiting iterations to "max"
*/
void compute_image( struct bitmap *bm, double xmin, double xmax, double ymin, double ymax, int max )
{
	int i,j;

	int width = bitmap_width(bm);
	int height = bitmap_height(bm);

	// For every pixel in the image...

	for(j=0;j<height;j++) {

		for(i=0;i<width;i++) {

			// Determine the point in x,y space for that pixel.
			double x = xmin + i*(xmax-xmin)/width;
			double y = ymin + j*(ymax-ymin)/height;

			// Compute the iterations at that point.
			int iters = iterations_at_point(x,y,max);

			// Set the pixel in the bitmap.
			bitmap_set(bm,i,j,iters);
		}
	}
}

/*
Return the number of iterations at point x, y
in the Mandelbrot space, up to a maximum of max.
*/

int iterations_at_point( double x, double y, int max )
{
	double x0 = x;
	double y0 = y;

	int iter = 0;

	while( (x*x + y*y <= 4) && iter < max ) {

		double xt = x*x - y*y + x0;
		double yt = 2*x*y + y0;

		x = xt;
		y = yt;

		iter++;
	}

	return iteration_to_color(iter,max);
}

/*
Convert a iteration number to an RGBA color.
Here, we just scale to gray with a maximum of imax.
Modify this function to make more interesting colors.
*/

int iteration_to_color( int i, int max )
{
	int gray = 255*i/max;
	return MAKE_RGBA(gray,gray,gray,0);
}


