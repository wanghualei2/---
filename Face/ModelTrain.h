#ifndef MODELTRAIN_H
#define MODELTRAIN_H
#include"Picture.h"
#include"math.h"
#include <time.h>
#include <list> 
using std::to_string;
//���ฺ��ͼƬ������ѵ������������ģ���ļ�
//������Ҫ��ȡ�������ļ����ڴ治��ͬʱ�洢��˶�����ݣ�������Ҫͨ���ļ�����ʱ�����������е����ݣ���δ����


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

	//��ֵ����
	short int haar_compares;

	//Sj����{1��-1}
	short int p_s;

	//ϵ������-��Ӧ����������Ȩֵ
	double coefficient;
};

//����������haarֵ������ѡȡ���ŵ�����ֵ
struct  Features
	{
	short int *haarEigenvalue;

	short int *picture_No;

	//��ʼ��picture_No��haarEigenvalue
	//����picture_No��ֵ
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

			//��������
			int iteration_nums = 300;

			Features *features;
	 

			//Picture *pictures; 

			//����������
			int sample_num=0;

			//����������
			int pos_sample_num = 0;

			//����������
			int neg_sample_num = 0;

			//Ȩֵ�� ���飨������Ҫ��
			double weight_sums=1; 

			//ǿ������
			StrongClasssify *strongClasssify; 

			//@pos_filePath �������ļ���·���б��ļ�
			//@neg_filePath �������ļ���·���б��ļ�
			public:ModelTrain(string pos_filePath, string neg_filePath){ 
					start = clock();
					initHaarNoValue();
					initPictures(pos_filePath, neg_filePath);  
					initStrongClasssify(); 
					computerMain(); 
					finish = clock();
					cout << "\t��ʱ��" << (double)(finish - start) / CLOCKS_PER_SEC / 60 << "����" << endl;
			}

			void initStrongClasssify(){
				cout << "MSG:----ǿ��������ʼ����ʼ��------\n" << endl;
				strongClasssify = new StrongClasssify[iteration_nums];
				for (int i = 0; i < iteration_nums; i++){
					strongClasssify[i].init(sample_num);
				} 
				cout << "MSG:----ǿ��������ʼ����ɣ�------\n" << endl;
			}



			void initHaarNoValue(){
				haar_No_value = new Haar_No_value[haarNum];
				ifstream inFile("haar_no_new.csv", ios::in);
				string lineStr;
				vector<vector<string>> strArray;
				int i = 0;
				while (getline(inFile, lineStr))
				{
					// ��ɶ�ά��ṹ
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
			 
			//���õ��Ĳ���������txt��
			public:void printMsg(int s){
			   
					   printMsg1(s);
			}
 

			public :void printMsg1(int s){
						String ss;
						ss.append("F:\\���ݲ���\\printMsg1\\model300.csv"); 
						FileHelper f1(ss, 1);
						for (int i = 0; i < s + 1; i++){
							f1.write(strongClasssify[i].haarNo);//haarNo
							f1.write(',');
							f1.write(strongClasssify[i].p_s);//p_s
							f1.write(',');
							f1.write(strongClasssify[i].haar_compares);//��ֵ
							f1.write(',');
							f1.write(strongClasssify[i].coefficient);//Ȩֵ 
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

			//��Ҫ���㷨���ģ�
			public:void computerMain(){

					   cout << "��ʼ���㣺" << iteration_nums << endl;

					   //1.����haar����һ����ʼ��Ȩֵ,����������Ȩֵ��ʼ��������������������
					   for (int i = 0; i < sample_num; i++){
						   if (i < pos_sample_num){

							   strongClasssify[0].weights[i] = 1.0 / (2 * pos_sample_num);
						   }
						   else{
							   strongClasssify[0].weights[i] = 1.0 / (2 * neg_sample_num); 
						   }

					   }
					   //2.���е���;For t=1,....T;----------------------------------------------------------------------------��һ��ѭ����iteration 200
			    
					   for (int iteration = 0; iteration < iteration_nums; iteration++){  
						   //2.1 ��Ȩֵ���й�һ�� 
						   for (int i = 0; i < sample_num; i++){
							   strongClasssify[iteration].weights[i] = strongClasssify[iteration].weights[i] / weight_sums;
						   }  
				   
						   //2.2 ѡ�����ŵķ�����-----------------------------------------------------------------------------�ڶ���ѭ����haarNum 60000
						   double argmin=1;
						   for (int haar_no = 0; haar_no < haarNum; haar_no++){ //�������е�haar����Ѱ�����Ų��� 
							   //������ 
							   double value = evaluate_h(iteration, haar_no, features[haar_no].haarEigenvalue[0]); 
							   //�������п�ȡ�Ĳ���Ѱ�����Ų���---------------------------------------------------------------������ѭ����sample_num 1000
					   
							   for (int feature_no = 1; feature_no < sample_num; feature_no++){
								   int pictureNo = features[haar_no].picture_No[feature_no-1]; 
								   if (getLabel(pictureNo) == 1){                                    //�����ʼ���
									   value = value - strongClasssify[iteration].weights[pictureNo];  
								   }
						    
								   else{                                                                   //����������
							   
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
						   //2.3 Define �������� �Ѿ�����
						   //2.4 ����Ȩֵ
			   
						   weight_sums = 0;
						   int d = 0;
				  
						   for (int i = 0; i < sample_num; i++)
						   { 
							   int pictureNo = features[strongClasssify[iteration].haarNo].picture_No[i];
							   //����ͼƬ������ȷ������ȷ�������Ų����жϽ����ԭ���һ��
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
								<< "\tbate:" << bate << "\t׼ȷ�ʣ�" << 1.0*d / sample_num << "\tweight_sums:" << weight_sums<<endl;
						   strongClasssify[iteration].coefficient = log10(1.0/bate); 

						   printMsg(iteration);
					   }
			} 

			//��ʼ��pictures
			public:void initPictures(string pos_Path, string neg_Path){

					   cout << "MSG:----ͼƬ��ʼ����ʼ��------\n" << endl; 
					    
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
					   //��ѵ�����������϶������ᷢ���ڴ������6000����������Ϊ���ޡ�
					   //��ѭ���Ĺ����Ƿ����ڴ�
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
					 cout << "\t��������ʼ����ɣ�\t";
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
					 cout << "��������ʼ����ɣ�\t"; 


					 for (int i = 0; i < haarNum; i++){

						 QuickSort(features[i], 0, sample_num - 1);
					 }

					 cout << "MSG:----ͼƬ��ʼ����ɣ�------\n" << endl;
			    
			}

			//�ú���������������Ҫ�����ĺû�
			//@index ��ʾ�����ı�ţ�����ָͼƬ��
			//@p, feature_compare Ϊ����Ĳ���
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
 

			//��������
			//@p,feature_compare Ϊ����Ĳ���
			//fΪͼ���haar����ֵ
			public:int weak_classifier(int p, int feature, int feature_compare){
					   if (p*feature < p*feature_compare)
						   return 1;//����
					   else
						   return 0;
			}  

		   //���������㷨
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
				   while (feature.haarEigenvalue[j] >= key1 && j>i)                       //���α���������
					   j--;
				   while (feature.haarEigenvalue[i] <= key1 && j>i)                       //���ұ���������
					   i++;


				   if (i<j)                                         //���������α��Ӧ��ֵ
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