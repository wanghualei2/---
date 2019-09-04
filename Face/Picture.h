#ifndef PICTURE_H
#define PICTURE_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv.hpp>  
#include"FileHelper.h"
using namespace cv;
using namespace std;

class Picture{ 

				//图片的积分图像  不可换成short int 数据超出范围
				public :int *integral;

				//图像的Haar特征
				public:short int *haarEigenvalue; 

				public:Picture(String filePath){
						   initMsg(filePath);
				}

				public:Picture(){
				}

				public:void pictureFree(){
						   free(haarEigenvalue);
				}

				public:void initMsg(String filePath){
						   Mat camerFrame, mat;
						   camerFrame = imread(filePath, 1);
						   Pic2Gray(camerFrame, mat);//将图像转化为灰度图像  
						   initIntegral(mat);
						   initHaarEigenvalue();
						   free(integral);
				} 

				//根据色素矩阵初始化积分图像
				public:void initIntegral(Mat mat){
							rows = mat.rows;
							cols = mat.cols; 
							integral = new int[rows*cols];
							//以下繁琐步骤是为了算法的快速，减少判断  
							integral[0] = mat.at<uchar>(0, 0); 
							for (int i = 1; i < rows; i++){
								//uchar为单通道读取方法
								integral[i*cols] = mat.at<uchar>(i, 0) + integral[cols*(i - 1)];
							}
							for (int i = 1; i < mat.cols; i++){
								integral[i] = mat.at<uchar>(0, i) + integral[(i - 1)];
							} 
							for (int i = 1; i < rows; i++){
								for (int ri = 1; ri < cols; ri++){
									integral[i*cols + ri] = integral[i*cols + ri - 1]
										+ integral[(i - 1)*cols + ri] - integral[(i - 1)*cols + ri - 1]
										+ mat.at<uchar>(i, ri);
								}
							}  
				} 

				//根据积分图像计算haar特征  白色减去黑色
				public:void initHaarEigenvalue(){

						   haarEigenvalue = new short int[haarNum];
						   int count = 0;
						   //计算二矩阵水平型特征  
						   for (int y = 0; y <=rows; y++){//行
							   for (int x = 0; x <= cols-1; x = x++){  //列
								   for (int len = 1; len <= rows - y; len++){//长
									   for (int wid = 2; wid <= cols - x; wid = wid + 2){//宽 
										   haarEigenvalue[count] = computerSumColor(x, y, wid/2, len)
											   -computerSumColor(x+wid/2,y,wid/2,len); 
										   count++; 
									   }
								   }
							   }
						   }  
						   //计算二矩阵垂直型特征
						   for (int x = 0; x < cols; x++){//列
							   for (int y = 0; y < rows-1; y = y++){  //行
								   for (int wid = 1; wid <= rows - x; wid++){//宽
									   for (int len = 2; len <= cols - y; len = len + 2){//长 
										   haarEigenvalue[count] = computerSumColor(x, y, wid, len/2)
											   -computerSumColor(x, y+len/2, wid, len/2);
										   count++;
									   }
								   }
							   }
						   }
						   //计算三矩阵水平型
						   for (int y = 0; y < rows; y++){//行
							   for (int x = 0; x < cols-2; x = x++){  //列
								   for (int len = 1; len <= rows - y; len++){//长
									   for (int wid = 3; wid <= cols - x; wid = wid + 3){//宽 
										   haarEigenvalue[count] = computerSumColor(x, y, wid/3, len)
											   + computerSumColor(x + wid / 3*2, y, wid / 3, len)
											   -computerSumColor(x+wid/3, y, wid/3, len);
										   count++;
									   }
								   }
							   }
						   } 
						   //计算三矩阵垂直型
						   for (int x = 0; x < cols; x++){//列
							   for (int y = 0; y < rows-2; y = y++){  //行
								   for (int wid = 1; wid <= rows - x; wid++){//宽
									   for (int len = 3; len <= cols - y; len = len + 3){//长 
										   haarEigenvalue[count] = computerSumColor(x, y, wid, len / 3)
											   + computerSumColor(x , y+len/ 3 * 2, wid, len/3)
										   -computerSumColor(x , y+len/ 3, wid, len/3);
										   count++;
									   }
								   }
							   }
						   }
						   //计算四矩阵（只有一种类型）
						   for (int y = 0; y <rows-1; y++){//行
							   for (int x = 0; x < cols-1; x = x++){  //列
								   for (int len = 2; len <= cols - y; len = len + 2){//长
									   for (int wid = 2; wid <= rows - x; wid = wid + 2){//宽 
										   haarEigenvalue[count] = computerSumColor(x+wid/2, y, wid/2, len / 2)
											   + computerSumColor(x , y+ len/ 2, wid / 2, len / 2)
											   - computerSumColor(x, y, wid/2, len / 2)
											   - computerSumColor(x+wid/2, y + len / 2, wid/2, len / 2);
										   count++;
									   }
								   }
							   }
						   } 
						   //计算9矩阵（只有一种类型）
						   for (int y = 0; y <rows - 3; y++){//行
							   for (int x = 0; x < cols - 3; x = x++){  //列
								   for (int len = 3; len <= cols - y; len = len + 3){//长
									   for (int wid = 3; wid <= rows - x; wid = wid + 3){//宽 

										   haarEigenvalue[count] = computerSumColor(x, y, wid, len) 
											   - 2*computerSumColor(x + wid / 3, y + len / 3, wid / 3, len / 3);
										   count++;
									   }
								   }
							   }
						   }


				}

				//计算矩形区域的像素和
				//@x,y 矩形右上角的坐标
				//@wid,len矩形的宽度与长度
				public:int computerSumColor(int x,int y,int wid,int len){

						   if (x == 0 && y == 0) 
							   return integral[(len-1)*cols+wid-1];
						   else if (x==0){
							   return integral[(y+len-1)*cols + wid-1] - integral[(y-1)*cols+wid-1];
						   }
						   else if (y == 0){
							   
							   return integral[(len - 1)*cols + x + wid - 1] - integral[(len - 1)*cols + x - 1];
						   }
						   else{
							   return integral[(y + len - 1)*cols + x + wid-1] - integral[(y-1)*cols + x + wid-1]
								   - integral[(y + len - 1)*cols + x-1] + integral[(y-1)*cols + x-1];
						   } 
				}
	
				//将图像转化为灰度图像
				void Pic2Gray(Mat camerFrame, Mat &gray)
				{
							//普通台式机3通道BGR,移动设备为4通道
							if (camerFrame.channels() == 3)
							{
								cvtColor(camerFrame, gray, CV_BGR2GRAY);
							}
							else if (camerFrame.channels() == 4)
							{
								cvtColor(camerFrame, gray, CV_BGRA2GRAY);
							}
							else
								gray = camerFrame;
				}


				//该算法用来计算特定大小的图像产生haar特征的数量
				//一般使用的图像为24*24,所以一般使用默认的162336
				public :int computerHaarNum(){ 
					int count = 0, count1 = 0;
					for (int i = 0; i < rows; i++){//行
						for (int rri = 0; rri < cols-1; rri = rri++){  //列
							for (int ri = 1; ri <= cols - i; ri++){//长
								for (int y = 2; y <= rows - rri; y = y + 2){//宽
									count1++;

								}
							}
						}
					}
					count = count1 * 2;
					count1 = 0;
					for (int i = 0; i < rows; i++){//行
						for (int rri = 0; rri < cols-2; rri = rri++){  //列
							for (int ri = 1; ri <= cols - i; ri++){//长
								for (int y = 3; y <= rows - rri; y = y + 3){//宽
									count1++;

								}
							}
						}
					}
					count = count + count1 * 2;
					count1 = 0;
					for (int i = 0; i <cols-1; i++){//行
						for (int rri = 0; rri < cols-1; rri = rri++){  //列
							for (int ri = 2; ri <= rows - i; ri = ri + 2){//长
								for (int y = 2; y <= cols - rri; y = y + 2){//宽
									count++;

								}
							}
						}
					}
					return count;
				} 

};
#endif