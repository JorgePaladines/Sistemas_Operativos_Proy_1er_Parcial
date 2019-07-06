#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <mutex>

/*
para compilar:
g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -std=c++11 split_mat_thread.cpp -o split_mat_thread -lpthread -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs
para ejecutar:
./split_mat_thread lena.jpg
para medir el tiempo:

time ./split_mat_thread lena.jpg
*/

using namespace cv;
using namespace std;

mutex image_mutex;

void fun_par(vector<Mat> sec, vector<Mat> dst, Mat src, Mat kernel, int height, Mat3b res, int i){
  sec[i] = src(Rect(0,height*i,src.cols,height));
  filter2D(sec[i], dst[i], -1 , kernel, Point( -1, -1 ), 0, BORDER_DEFAULT);
  image_mutex.lock();
  dst[i].copyTo(res(Rect(0,dst[i].rows*i,src.cols,dst[i].rows)));
  image_mutex.unlock();
}

/** @function main */
int main ( int argc, char** argv )
{
  Mat src = imread( argv[1] );
  if( !src.data ){
    return -1;
  }

  int cores = std::thread::hardware_concurrency(); //toma el número de cores del cpu
  int height = src.rows/cores;
  vector<Mat> sec(cores);
  vector<Mat> dst(cores);

  Mat kernel=getGaussianKernel(15,3);

  Mat3b res(src.rows, src.cols, Vec3b(0,0,0));

  vector<thread> threads;

  for(int i = 0; i < cores; i++){
    threads.push_back(thread(fun_par,sec,dst,src,kernel,height,res,i));
  }
  for(int i = 0; i < cores; i++){
    threads[i].join();
  }

  imwrite("output_lena.jpg",res);

  return 0;
}
