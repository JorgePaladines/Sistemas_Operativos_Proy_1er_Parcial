#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

#include <omp.h>

/*
para compilar:

g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 concurrent_filter_demo_SO.cpp -o ejemplo_filtro -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -fopenmp

para ejecutar:
./ejemplo_filtro lena.jpg

para medir el tiempo:

time ./ejemplo_filtro lena.jpg

*/

using namespace cv;

/** @function main */
int main ( int argc, char** argv )
{
  Mat src;
  Mat dst;
  Mat kernel;
  #pragma omp parallel
  {
      src = imread( argv[1] );
  }
  if( !src.data ){
    return -1;
  }

  #pragma omp parallel sections
  {
    #pragma omp section
    {
      kernel=getGaussianKernel(15,3);
    }
    #pragma omp section
    {
      filter2D(src, dst, -1 , kernel, Point( -1, -1 ), 0, BORDER_DEFAULT);
    }
  }

  #pragma omp parallel
  {
    imwrite("output_lena.jpg",dst);
  }

  return 0;
}
