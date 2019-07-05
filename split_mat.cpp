#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
//#include <thread>

/*
para compilar:
g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 split_mat.cpp -o split_mat -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs
para ejecutar:
./split_mat lena.jpg
para medir el tiempo:

time ./split_mat lena.jpg
*/

using namespace cv;

//función que crea las secciones para filtrar
Mat fun_seccion(Mat src, int cont, int cores){
  int height = src.rows/cores;
  if(cont < (cores-1))
    return src(Rect(0,height*cont,src.cols,height));
  else
    return src(Rect(0,height*cont,src.cols,src.rows-(height*cont)));
}


/** @function main */
int main ( int argc, char** argv )
{
  /// Load an image
  Mat src = imread( argv[1] );
  if( !src.data ){
    return -1;
  }

  //Mat dst1,dst2,dst3,dst4;
  //Mat sec1,sec2,sec3,sec4;

  int cores = 4;
  int height = src.rows/cores;
  Mat sec[cores];
  Mat dst[cores];
  //memset(dst,0,cores);

  Mat kernel=getGaussianKernel(15,3);

  Mat3b res(src.rows, src.cols, Vec3b(0,0,0));
  //Rect region_of_interest = Rect(0, 0, src.cols, src.rows/2);

  //Rect(x, y, w, h);

  /*
  sec1 = src(Rect(0, 0, src.cols, src.rows/4));
  sec2 = src(Rect(0, sec1.rows, src.cols, sec1.rows));
  sec3 = src(Rect(0, sec1.rows+sec2.rows, src.cols, sec1.rows));
  sec4 = src(Rect(0, sec1.rows+sec2.rows+sec3.rows, src.cols, src.rows-sec1.rows-sec2.rows-sec3.rows));


  filter2D(sec[0], dst[0], -1 , kernel, Point( -1, -1 ), 0, BORDER_DEFAULT);
  filter2D(sec[1], dst[1], -1 , kernel, Point( -1, -1 ), 0, BORDER_DEFAULT);
  filter2D(sec[2], dst[2], -1 , kernel, Point( -1, -1 ), 0, BORDER_DEFAULT);
  filter2D(sec[3], dst[3], -1 , kernel, Point( -1, -1 ), 0, BORDER_DEFAULT);

  dst[0].copyTo(res(Rect(0,0,src.cols,dst[0].rows)));
  dst[1].copyTo(res(Rect(0,dst[0].rows,src.cols,dst[1].rows)));
  dst[2].copyTo(res(Rect(0,dst[0].rows+dst[1].rows,src.cols,dst[2].rows)));
  dst[3].copyTo(res(Rect(0,dst[0].rows+dst[1].rows+dst[2].rows,src.cols,dst[3].rows)));

  */

  for(int i = 0; i < cores; i++){
    sec[i] = src(Rect(0,height*i,src.cols,height));
    //sec[i] = fun_seccion(src,i,cores);
    filter2D(sec[i], dst[i], -1 , kernel, Point( -1, -1 ), 0, BORDER_DEFAULT);
    dst[i].copyTo(res(Rect(0,dst[i].rows*i,src.cols,dst[i].rows)));
  }

  bool isSuccess = imwrite("output_lena.jpg",res);

  /*
  if (imwrite("output_lena.jpg",dst) == true) //remover
	  imshow( "imagen filtrada", dst );

  //Si se quita esta línea, hay un Segmentation Fault
  waitKey( 0 ); //remover
  */

  return 0;
}
