#ifndef MODELTRAIN_H
#define MODELTRAIN_H
#include"Picture.h"
#include"math.h"
#include <time.h>
#include <list> 
using std::to_string;
//该类负责图片样本的训练，最终生成模型文件
//由于需要读取大量的文件，内存不能同时存储如此多的数据，所以需要通过文件来临时保存计算过程中的数据（暂未处理）


class Haar_No_value{

public:int haar_id;

public:int rows;
public:int cols;
public:int len;
public:int wid;

public:Haar_No_value(){

}

public:Haar_No_value(int haar_id, int rows, int cols, int len, int wid){
		   this->haar_id = haar_id;
		   this->rows = rows;
		   this->cols = cols;
		   this->len = len;
		   this->wid = wid;
}
};


struct  StrongClasssify
{

	void init(int sampleNums){ 
		weights = new double[sampleNums];
	}

	int haarNo;

	double argmin;

	double *weights;

	//阙值数组
	short int haar_compares;

	//Sj参数{1，-1}
	short int p_s;

	//系数数组-对应弱分类器的权值
	double coefficient;
};

//保存排序后的haar值，从中选取最优的特征值
struct  Features
	{
	short int *haarEigenvalue;

	short int *picture_No;

	//初始化picture_No，haarEigenvalue
	//并将picture_No赋值
	void init(int num){
		haarEigenvalue = new short int[num];
		picture_No = new short int[num];
		for (int i = 0; i < num; i++){
			picture_No[i] = i;
		}
	}
};

class ModelTrain{  

	        clock_t start, finish;

	    	Haar_No_value *haar_No_value;

			//迭代次数
			int iteration_nums = 300;

			Features *features;
	 

			//Picture *pictures; 

			//样本总数量
			int sample_num=0;

			//正样本数量
			int pos_sample_num = 0;

			//负样本数量
			int neg_sample_num = 0;

			//权值和 数组（计算需要）
			double weight_sums=1; 

			//强分类器
			StrongClasssify *strongClasssify; 

			//@pos_filePath 正样本文件的路径列表文件
			//@neg_filePath 负样本文件的路径列表文件
			public:ModelTrain(string pos_filePath, string neg_filePath){ 
					start = clock();
					initHaarNoValue();
					initPictures(pos_filePath, neg_filePath);  
					initStrongClasssify(); 
					computerMain(); 
					finish = clock();
					cout << "\t用时：" << (double)(finish - start) / CLOCKS_PER_SEC / 60 << "分钟" << endl;
			}

			void initStrongClasssify(){
				cout << "MSG:----强分类器初始化开始！------\n" << endl;
				strongClasssify = new StrongClasssify[iteration_nums];
				for (int i = 0; i < iteration_nums; i++){
					strongClasssify[i].init(sample_num);
				} 
				cout << "MSG:----强分类器初始化完成！------\n" << endl;
			}



			void initHaarNoValue(){
				haar_No_value = new Haar_No_value[haarNum];
				ifstream inFile("haar_no_new.csv", ios::in);
				string lineStr;
				vector<vector<string>> strArray;
				int i = 0;
				while (getline(inFile, lineStr))
				{
					// 存成二维表结构
					stringstream ss(lineStr);
					string str;
					getline(ss, str, ',');
					int haar_id = stoi(str);
					//haar_id
					getline(ss, str, ',');
					int rows = stoi(str);
					getline(ss, str, ',');
					int cols = stoi(str);
					getline(ss, str, ',');
					int len = stoi(str);
					getline(ss, str, ',');
					int wid = stoi(str);
					Haar_No_value h(haar_id, rows, cols, len, wid);
					haar_No_value[i] = h;
					i++;
				}
			} 
			 
			//将得到的参数保存在txt中
			public:void printMsg(int s){
			   
					   printMsg1(s);
			}
 

			public :void printMsg1(int s){
						String ss;
						ss.append("F:\\数据参数\\printMsg1\\model300.csv"); 
						FileHelper f1(ss, 1);
						for (int i = 0; i < s + 1; i++){
							f1.write(strongClasssify[i].haarNo);//haarNo
							f1.write(',');
							f1.write(strongClasssify[i].p_s);//p_s
							f1.write(',');
							f1.write(strongClasssify[i].haar_compares);//阀值
							f1.write(',');
							f1.write(strongClasssify[i].coefficient);//权值 
							f1.write(',');
							f1.write(haar_No_value[strongClasssify[i].haarNo].haar_id);
							f1.write(',');
							f1.write(haar_No_value[strongClasssify[i].haarNo].rows);
							f1.write(',');
							f1.write(haar_No_value[strongClasssify[i].haarNo].cols);
							f1.write(',');
							f1.write(haar_No_value[strongClasssify[i].haarNo].len);
							f1.write(',');
							f1.write(haar_No_value[strongClasssify[i].haarNo].wid);
							f1.nextLine();
						}
						f1.close(); 
			}

			//主要的算法核心，
			public:void computerMain(){

					   cout << "开始计算：" << iteration_nums << endl;

					   //1.赋予haar特征一个初始的权值,正负样本的权值初始化根据正负样本的数量
					   for (int i = 0; i < sample_num; i++){
						   if (i < pos_sample_num){

							   strongClasssify[0].weights[i] = 1.0 / (2 * pos_sample_num);
						   }
						   else{
							   strongClasssify[0].weights[i] = 1.0 / (2 * neg_sample_num); 
						   }

					   }
					   //2.进行迭代;For t=1,....T;----------------------------------------------------------------------------第一层循环：iteration 200
			    
					   for (int iteration = 0; iteration < iteration_nums; iteration++){  
						   //2.1 对权值进行归一化 
						   for (int i = 0; i < sample_num; i++){
							   strongClasssify[iteration].weights[i] = strongClasssify[iteration].weights[i] / weight_sums;
						   }  
				   
						   //2.2 选择最优的分类器-----------------------------------------------------------------------------第二层循环：haarNum 60000
						   double argmin=1;
						   for (int haar_no = 0; haar_no < haarNum; haar_no++){ //遍历所有的haar特征寻求最优参数 
							   //错误率 
							   double value = evaluate_h(iteration, haar_no, features[haar_no].haarEigenvalue[0]); 
							   //遍历所有可取的参数寻求最优参数---------------------------------------------------------------第三层循环：sample_num 1000
					   
							   for (int feature_no = 1; feature_no < sample_num; feature_no++){
								   int pictureNo = features[haar_no].picture_No[feature_no-1]; 
								   if (getLabel(pictureNo) == 1){                                    //错误率减少
									   value = value - strongClasssify[iteration].weights[pictureNo];  
								   }
						    
								   else{                                                                   //错误率增加
							   
									   value = value + strongClasssify[iteration].weights[pictureNo]; 
								   } 
								   if (argmin > value){ 
							   
									   argmin = value;
									   strongClasssify[iteration].haar_compares = features[haar_no].haarEigenvalue[feature_no];
									   strongClasssify[iteration].p_s = 1;
									   strongClasssify[iteration].argmin = argmin;
									   strongClasssify[iteration].haarNo = haar_no;
								   }
								   else if (argmin > (1 - value)){
									   argmin = 1-value;
									   strongClasssify[iteration].argmin = argmin;
									   strongClasssify[iteration].haar_compares = features[haar_no].haarEigenvalue[feature_no];
									   strongClasssify[iteration].p_s = -1;
									   strongClasssify[iteration].haarNo = haar_no;
								   } 
							   }
					    
						   } 
						   double bate = argmin / (1 - argmin); 
						   //2.3 Define 弱分类器 已经更新
						   //2.4 更新权值
			   
						   weight_sums = 0;
						   int d = 0;
				  
						   for (int i = 0; i < sample_num; i++)
						   { 
							   int pictureNo = features[strongClasssify[iteration].haarNo].picture_No[i];
							   //若此图片分类正确根据所确定的最优参数判断结果与原结果一致
							   int e_i = -1;
							   if (weak_classifier(strongClasssify[iteration].p_s, features[strongClasssify[iteration].haarNo].haarEigenvalue[i],
								   strongClasssify[iteration].haar_compares) == getLabel(pictureNo)){
								   e_i = 0;
								   d++;
							   }
							   else{
								   e_i = 1;
							   }
							   if (iteration + 1 == iteration_nums)
								   break;
							   strongClasssify[iteration + 1].weights[pictureNo] =
								   strongClasssify[iteration].weights[pictureNo] * pow(bate, 1 - e_i);
							   weight_sums = weight_sums + strongClasssify[iteration + 1].weights[pictureNo];

						   }

						   cout<< iteration <<"\targmin:" 
								<< argmin << "\tcompares:" << strongClasssify[iteration].haar_compares
								<< "\thaarNo:" << strongClasssify[iteration].haarNo << "\tps:" << strongClasssify[iteration].p_s 
								<< "\tbate:" << bate << "\t准确率：" << 1.0*d / sample_num << "\tweight_sums:" << weight_sums<<endl;
						   strongClasssify[iteration].coefficient = log10(1.0/bate); 

						   printMsg(iteration);
					   }
			} 

			//初始化pictures
			public:void initPictures(string pos_Path, string neg_Path){

					   cout << "MSG:----图片初始化开始！------\n" << endl; 
					    
					   FileHelper filehelper1(pos_Path, 0);
					   String fileName = filehelper1.readline();
					   while (!fileName.empty()){  
						   sample_num++;
						   pos_sample_num++;
						   fileName = filehelper1.readline();
					   }
					   filehelper1.close();
			  
					   FileHelper filehelper2(neg_Path, 0);
					   fileName = filehelper2.readline();
					   while (!fileName.empty()){  
						   sample_num++;
						   neg_sample_num++;
						   fileName = filehelper2.readline();
					   }  

					   filehelper2.close();
					   features = new Features[haarNum]; 
					   //*******************************************************************
					   //当训练样本数量较多后，这里会发生内存溢出，6000张样本左右为极限。
					   //该循环的功能是分配内存
					   //*******************************************************************
					   for (int i = 0; i < haarNum; i++){
						   features[i].init(sample_num);
					   }  

					 int i = 0;
					 FileHelper filehelper11(pos_Path, 0);
					 fileName = filehelper11.readline(); 
					 while (!fileName.empty()){   
						 Picture p;
						 p.initMsg(fileName);   
						 for (int ri = 0; ri < haarNum; ri++){
							 features[ri].haarEigenvalue[i] = p.haarEigenvalue[ri];
						 } 
						 p.pictureFree();
						 i++; 
						 fileName = filehelper11.readline(); 
					 }
					 cout << "\t正样本初始化完成！\t";
					 filehelper11.close(); 
					 FileHelper filehelper22(neg_Path, 0);
					 fileName = filehelper22.readline();
					 while (!fileName.empty()){  
				 
						 Picture p;
						 p.initMsg(fileName);
						 for (int ri = 0; ri < haarNum; ri++){
							 features[ri].haarEigenvalue[i] = p.haarEigenvalue[ri];
						 } 
						 p.pictureFree();
						 i++; 
						 fileName = filehelper22.readline();
					 }
					 filehelper2.close();
					 cout << "负样本初始化完成！\t"; 


					 for (int i = 0; i < haarNum; i++){

						 QuickSort(features[i], 0, sample_num - 1);
					 }

					 cout << "MSG:----图片初始化完成！------\n" << endl;
			    
			}

			//该函数用来评估所需要参数的好坏
			//@index 表示特征的标号（不特指图片）
			//@p, feature_compare 为所求的参数
			public:long double evaluate_h(int iteration_no, int haar_no, int feature_compare){
					   long double sum = 0;  
					   list<Picture>::iterator testiterator; 
					   for (int feature_id = 0; feature_id < sample_num; feature_id++){  
						   int classifier = weak_classifier(1, features[haar_no].haarEigenvalue[feature_id], feature_compare);  
						   sum = sum + strongClasssify[iteration_no].
							   weights[features[haar_no].picture_No[feature_id]] * abs(classifier - getLabel(features[haar_no].picture_No[feature_id]));
					   }   
					   return sum;
			} 
 

			//弱分类器
			//@p,feature_compare 为所求的参数
			//f为图像的haar特征值
			public:int weak_classifier(int p, int feature, int feature_compare){
					   if (p*feature < p*feature_compare)
						   return 1;//人脸
					   else
						   return 0;
			}  

		   //快速排序算法
		   void QuickSort(Features feature, int left, int right)
		   {

			   short int i, j, key1,key2, tmp1,tmp2;
			   if (left >= right)
				   return;
			   key1 = feature.haarEigenvalue[left];
			   key2 = feature.picture_No[left];

			   i = left;
			   j = right;


			   while (i != j)
			   {
				   while (feature.haarEigenvalue[j] >= key1 && j>i)                       //右游标往左搜索
					   j--;
				   while (feature.haarEigenvalue[i] <= key1 && j>i)                       //左右表往右搜索
					   i++;


				   if (i<j)                                         //交换左右游标对应的值
				   {
					   tmp1 = feature.haarEigenvalue[j];
					   tmp2 = feature.picture_No[j];
					   feature.haarEigenvalue[j] = feature.haarEigenvalue[i];
					   feature.picture_No[j] = feature.picture_No[i];
					   feature.haarEigenvalue[i] = tmp1;
					   feature.picture_No[i] = tmp2;

				   }
			   }
			   feature.haarEigenvalue[left] = feature.haarEigenvalue[i];
			   feature.picture_No[left] = feature.picture_No[i];
			   feature.haarEigenvalue[i] = key1;
			   feature.picture_No[i] = key2;
			   QuickSort(feature, left, i - 1);
			   QuickSort(feature, i + 1, right);

		   }


		   public :int getLabel(int picture_No){
			   if (picture_No < pos_sample_num){
				   return 1;
			   }
			   return 0;
		   }
		    

};

#endif