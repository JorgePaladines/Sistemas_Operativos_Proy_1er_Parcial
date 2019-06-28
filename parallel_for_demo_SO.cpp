#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

/*
para compilar:
g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 parallel_for_demo_SO.cpp -o parallel_ej -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs

para ejecutar:
./parallel_ej lena.jpg

para medir el tiempo:
time ./parallel_ej lena.jpg

*/

using namespace cv;

class Parallel_code: public cv::ParallelLoopBody
{
private:
  Mat src,dst,kernel;

public:
  Parallel_code(Mat srcFile, Mat dstFile, Mat kernelOrg)
    : src(srcFile), dst(dstFile), kernel(kernelOrg){}

  void operator ()(const cv::Range &range) const{
    for (int r = range.start; r < range.end; r++)
    {
      //filter2D(src, dst, -1 , kernel, Point( -1, -1 ), 0, BORDER_DEFAULT);
    }
    //Mat kernel = getGaussianKernel(15,3);
    filter2D(src, dst, -1 , kernel, Point( -1, -1 ), 0, BORDER_DEFAULT);
    //imwrite("output_lena.jpg",dst);
  }
};

/** @function main */
int main ( int argc, char** argv )
{
  /// Load an image
  Mat src = imread( argv[1] );
  if( !src.data ){
    return -1;
  }
  //imshow( "imagen original", src );

  /// Declare variables
  Mat dst;
  //Mat kernel;
  //Point anchor;
  //double delta;
  //int ddepth;

  /// Initialize arguments for the filter
  //anchor = Point( -1, -1 );
  //delta = 0;
  //ddepth = -1;

  //Gaussian

  Mat kernel = getGaussianKernel(15,3);
  parallel_for_(cv::Range(0,6), Parallel_code(src, dst,kernel));

  /*
  Quizá la función getGaussianKernel se la puede hacer concurrente, sin embargo igual
  esta se nececita para la función filter2D.
  Aunque se la haga en hilos, todos deben de terminar antes de llamar a filter2D
  Así que se podría hacer concurrente la función getGaussianKernel para que termine más
  rápido, y luego hacer concurrente filter2D.
  */

  //kernel=getGaussianKernel(15,3);
  //filter2D(src, dst, -1 , getGaussianKernel(15,3), Point( -1, -1 ), 0, BORDER_DEFAULT);
  //bool isSuccess = imwrite("output_lena.jpg",dst);

  /*
  if (imwrite("output_lena.jpg",dst) == true) //remover
	  imshow( "imagen filtrada", dst );

  //Si se quita esta línea, hay un Segmentation Fault
  waitKey( 0 ); //remover
  */

  return 0;
}
