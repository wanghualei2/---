#ifndef PICTURE_H
#define PICTURE_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv.hpp>  
#include"FileHelper.h"
using namespace cv;
using namespace std;

class Picture{ 

				//ͼƬ�Ļ���ͼ��  ���ɻ���short int ���ݳ�����Χ
				public :int *integral;

				//ͼ���Haar����
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
						   Pic2Gray(camerFrame, mat);//��ͼ��ת��Ϊ�Ҷ�ͼ��  
						   initIntegral(mat);
						   initHaarEigenvalue();
						   free(integral);
				} 

				//����ɫ�ؾ����ʼ������ͼ��
				public:void initIntegral(Mat mat){
							rows = mat.rows;
							cols = mat.cols; 
							integral = new int[rows*cols];
							//���·���������Ϊ���㷨�Ŀ��٣������ж�  
							integral[0] = mat.at<uchar>(0, 0); 
							for (int i = 1; i < rows; i++){
								//ucharΪ��ͨ����ȡ����
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

				//���ݻ���ͼ�����haar����  ��ɫ��ȥ��ɫ
				public:void initHaarEigenvalue(){

						   haarEigenvalue = new short int[haarNum];
						   int count = 0;
						   //���������ˮƽ������  
						   for (int y = 0; y <=rows; y++){//��
							   for (int x = 0; x <= cols-1; x = x++){  //��
								   for (int len = 1; len <= rows - y; len++){//��
									   for (int wid = 2; wid <= cols - x; wid = wid + 2){//�� 
										   haarEigenvalue[count] = computerSumColor(x, y, wid/2, len)
											   -computerSumColor(x+wid/2,y,wid/2,len); 
										   count++; 
									   }
								   }
							   }
						   }  
						   //���������ֱ������
						   for (int x = 0; x < cols; x++){//��
							   for (int y = 0; y < rows-1; y = y++){  //��
								   for (int wid = 1; wid <= rows - x; wid++){//��
									   for (int len = 2; len <= cols - y; len = len + 2){//�� 
										   haarEigenvalue[count] = computerSumColor(x, y, wid, len/2)
											   -computerSumColor(x, y+len/2, wid, len/2);
										   count++;
									   }
								   }
							   }
						   }
						   //����������ˮƽ��
						   for (int y = 0; y < rows; y++){//��
							   for (int x = 0; x < cols-2; x = x++){  //��
								   for (int len = 1; len <= rows - y; len++){//��
									   for (int wid = 3; wid <= cols - x; wid = wid + 3){//�� 
										   haarEigenvalue[count] = computerSumColor(x, y, wid/3, len)
											   + computerSumColor(x + wid / 3*2, y, wid / 3, len)
											   -computerSumColor(x+wid/3, y, wid/3, len);
										   count++;
									   }
								   }
							   }
						   } 
						   //����������ֱ��
						   for (int x = 0; x < cols; x++){//��
							   for (int y = 0; y < rows-2; y = y++){  //��
								   for (int wid = 1; wid <= rows - x; wid++){//��
									   for (int len = 3; len <= cols - y; len = len + 3){//�� 
										   haarEigenvalue[count] = computerSumColor(x, y, wid, len / 3)
											   + computerSumColor(x , y+len/ 3 * 2, wid, len/3)
										   -computerSumColor(x , y+len/ 3, wid, len/3);
										   count++;
									   }
								   }
							   }
						   }
						   //�����ľ���ֻ��һ�����ͣ�
						   for (int y = 0; y <rows-1; y++){//��
							   for (int x = 0; x < cols-1; x = x++){  //��
								   for (int len = 2; len <= cols - y; len = len + 2){//��
									   for (int wid = 2; wid <= rows - x; wid = wid + 2){//�� 
										   haarEigenvalue[count] = computerSumColor(x+wid/2, y, wid/2, len / 2)
											   + computerSumColor(x , y+ len/ 2, wid / 2, len / 2)
											   - computerSumColor(x, y, wid/2, len / 2)
											   - computerSumColor(x+wid/2, y + len / 2, wid/2, len / 2);
										   count++;
									   }
								   }
							   }
						   } 
						   //����9����ֻ��һ�����ͣ�
						   for (int y = 0; y <rows - 3; y++){//��
							   for (int x = 0; x < cols - 3; x = x++){  //��
								   for (int len = 3; len <= cols - y; len = len + 3){//��
									   for (int wid = 3; wid <= rows - x; wid = wid + 3){//�� 

										   haarEigenvalue[count] = computerSumColor(x, y, wid, len) 
											   - 2*computerSumColor(x + wid / 3, y + len / 3, wid / 3, len / 3);
										   count++;
									   }
								   }
							   }
						   }


				}

				//���������������غ�
				//@x,y �������Ͻǵ�����
				//@wid,len���εĿ���볤��
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
	
				//��ͼ��ת��Ϊ�Ҷ�ͼ��
				void Pic2Gray(Mat camerFrame, Mat &gray)
				{
							//��̨ͨʽ��3ͨ��BGR,�ƶ��豸Ϊ4ͨ��
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


				//���㷨���������ض���С��ͼ�����haar����������
				//һ��ʹ�õ�ͼ��Ϊ24*24,����һ��ʹ��Ĭ�ϵ�162336
				public :int computerHaarNum(){ 
					int count = 0, count1 = 0;
					for (int i = 0; i < rows; i++){//��
						for (int rri = 0; rri < cols-1; rri = rri++){  //��
							for (int ri = 1; ri <= cols - i; ri++){//��
								for (int y = 2; y <= rows - rri; y = y + 2){//��
									count1++;

								}
							}
						}
					}
					count = count1 * 2;
					count1 = 0;
					for (int i = 0; i < rows; i++){//��
						for (int rri = 0; rri < cols-2; rri = rri++){  //��
							for (int ri = 1; ri <= cols - i; ri++){//��
								for (int y = 3; y <= rows - rri; y = y + 3){//��
									count1++;

								}
							}
						}
					}
					count = count + count1 * 2;
					count1 = 0;
					for (int i = 0; i <cols-1; i++){//��
						for (int rri = 0; rri < cols-1; rri = rri++){  //��
							for (int ri = 2; ri <= rows - i; ri = ri + 2){//��
								for (int y = 2; y <= cols - rri; y = y + 2){//��
									count++;

								}
							}
						}
					}
					return count;
				} 

};
#endif