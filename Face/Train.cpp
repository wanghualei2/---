#include"ModelTrain.h" 

 

void printHaar_No_Msg(string file_name){
		   FileHelper f(file_name, 1);
		   int count = 0, rows = 19, cols = 19;
		   //���������ˮƽ������  
		   for (int y = 0; y <= rows; y++){//��
			   for (int x = 0; x <= cols - 1; x = x++){  //��
				   for (int len = 1; len <= rows - y; len++){//��
					   for (int wid = 2; wid <= cols - x; wid = wid + 2){//�� 
						   f.write(1);
						   f.write(',');
						   f.write(y);
						   f.write(',');
						   f.write(x);
						   f.write(',');
						   f.write(len);
						   f.write(',');
						   f.write(wid);
						   f.write(',');
						   f.nextLine();
					   }
					   
				   }
			   }
		   }
		   //���������ֱ������
		   for (int x = 0; x < cols; x++){//��
			   for (int y = 0; y < rows - 1; y = y++){  //��
				   for (int wid = 1; wid <= rows - x; wid++){//��
					   for (int len = 2; len <= cols - y; len = len + 2){//�� 

						   f.write(2);
						   f.write(',');
						   f.write(y);
						   f.write(',');
						   f.write(x);
						   f.write(',');
						   f.write(len);
						   f.write(',');
						   f.write(wid);
						   f.write(',');
							f.nextLine();
					   }
				   }
			   }
		   }
		   //����������ˮƽ��
		   for (int y = 0; y < rows; y++){//��
			   for (int x = 0; x < cols - 2; x = x++){  //��
				   for (int len = 1; len <= rows - y; len++){//��
					   for (int wid = 3; wid <= cols - x; wid = wid + 3){//�� 

						   f.write(3);
						   f.write(',');
						   f.write(y);
						   f.write(',');
						   f.write(x);
						   f.write(',');
						   f.write(len);
						   f.write(',');
						   f.write(wid);
						   f.write(',');
					   f.nextLine();
					   }
				   }
			   }
		   }
		   //����������ֱ��
		   for (int x = 0; x < cols; x++){//��
			   for (int y = 0; y < rows - 2; y = y++){  //��
				   for (int wid = 1; wid <= rows - x; wid++){//��
					   for (int len = 3; len <= cols - y; len = len + 3){//�� 

						   f.write(4);
						   f.write(',');
						   f.write(y);
						   f.write(',');
						   f.write(x);
						   f.write(',');
						   f.write(len);
						   f.write(',');
						   f.write(wid);
						   f.write(',');
					   f.nextLine();
					   }
				   }
			   }
		   }
		   //�����ľ���ֻ��һ�����ͣ�
		   for (int y = 0; y <rows - 1; y++){//��
			   for (int x = 0; x < cols - 1; x = x++){  //��
				   for (int len = 2; len <= cols - y; len = len + 2){//��
					   for (int wid = 2; wid <= rows - x; wid = wid + 2){//�� 

						   f.write(5);
						   f.write(',');
						   f.write(y);
						   f.write(',');
						   f.write(x);
						   f.write(',');
						   f.write(len);
						   f.write(',');
						   f.write(wid);
						   f.write(',');
					   f.nextLine();
					   }
				   }
			   }
		   }

		   //����9����ֻ��һ�����ͣ�
		   for (int y = 0; y <rows - 3; y++){//��
			   for (int x = 0; x < cols - 3; x = x++){  //��
				   for (int len = 3; len <= cols - y; len = len + 3){//�� 

						   f.write(6);
						   f.write(',');
						   f.write(y);
						   f.write(',');
						   f.write(x);
						   f.write(',');
						   f.write(len);
						   f.write(',');
						   f.write(len);
						   f.write(',');
					   f.nextLine();
					   }
				   
			   }
		   }
		   f.close();
};

void main(){

	//ModelTrain m("pos.txt", "neg.txt"); 

	//printHaar_No_Msg("haar_no_new.csv");
}

