

//Gray Scale Image Convertion project for .bmp 800x800 colour image
//By Saurabh Kakade (sk2354@nau.edu)

#include <unistd.h>
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <sys/time.h>
#include <omp.h>

#define NTHREADS 4

int main()
{
    FILE *fIn = fopen("/media/saurabh/Python/UDEMY/C PROG/RGB_To_Grayscale/images/song3.bmp","rb"); //input path
    FILE  *fOut = fopen("/media/saurabh/Python/UDEMY/C PROG/RGB_To_Grayscale/images/song3_gray3.bmp","wb"); //output path

    unsigned char imgHeader[54];
    unsigned char  colorTable[1024];
    struct timeval start,end;
    double elapsed;
    omp_set_num_threads(NTHREADS);
    

    if(fIn == NULL)
    {
        printf("Unable to open image\n");
    }

    for(int i =0;i<54;i++)
    {
        imgHeader[i] = getc(fIn);
    }

    fwrite(imgHeader,sizeof(unsigned char),54,fOut);

    int height = *(int*)&imgHeader[22];
    int width  = *(int*)&imgHeader[18];
    int bitDepth = *(int*)&imgHeader[28];

    if(bitDepth <= 8)
    {
        fread(colorTable,sizeof(unsigned char),1024,fIn);
        fwrite(colorTable,sizeof(unsigned char),1024,fOut);
    }

    long int imgSize = height * width;

    printf("\nHeight: %d",height);
    printf("\nWidth: %d",width);
    printf("\nNumber of pixels: %ld",imgSize);
    unsigned char buffer[imgSize][3];
    long int temp2[imgSize];
    int i, temp;
    
    gettimeofday(&start, NULL);

    //parallelize code
    #pragma omp parallel for private(i) shared(buffer) schedule(static) 
    	for(i =0;i<imgSize;i++)
	    {
	        #pragma omp critical
	        {
		        buffer[i][0] = getc(fIn); //red
		        buffer[i][1] = getc(fIn); //green
		        buffer[i][2] = getc(fIn); //blue
	        }
		        temp =0;
	    	    temp = (buffer[i][0]*0.3 ) +
	    	    		(buffer[i][1]*0.59) +
	    	    		 (buffer[i][2]*0.11); 

	      	#pragma omp critical
            {
	      	   	putc(temp,fOut);
       			putc(temp,fOut);
       			putc(temp,fOut);
    	    }
	    }
		   
	    // for (int s = 0; s < imgSize; s++)
	    // {
	    // 	putc(temp2[s],fOut);
	    //    	putc(temp2[s],fOut);
	    //    	putc(temp2[s],fOut);
	    // }
	
	gettimeofday(&end, NULL);

    printf("\nSuccess!\n");

	// Print out elapsed time
	elapsed = ((end.tv_sec*1000000.0 + end.tv_usec) -
          (start.tv_sec*1000000.0 + start.tv_usec)) / 1000000.00;
	
	printf("Elapsed time in OpenMP sequential: %.3f\n",elapsed);

    fclose(fIn);
    fclose(fOut);

    return 0;
}
