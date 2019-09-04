#include"ModelTrain.h" 

 

void printHaar_No_Msg(string file_name){
		   FileHelper f(file_name, 1);
		   int count = 0, rows = 19, cols = 19;
		   //计算二矩阵水平型特征  
		   for (int y = 0; y <= rows; y++){//行
			   for (int x = 0; x <= cols - 1; x = x++){  //列
				   for (int len = 1; len <= rows - y; len++){//长
					   for (int wid = 2; wid <= cols - x; wid = wid + 2){//宽 
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
		   //计算二矩阵垂直型特征
		   for (int x = 0; x < cols; x++){//列
			   for (int y = 0; y < rows - 1; y = y++){  //行
				   for (int wid = 1; wid <= rows - x; wid++){//宽
					   for (int len = 2; len <= cols - y; len = len + 2){//长 

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
		   //计算三矩阵水平型
		   for (int y = 0; y < rows; y++){//行
			   for (int x = 0; x < cols - 2; x = x++){  //列
				   for (int len = 1; len <= rows - y; len++){//长
					   for (int wid = 3; wid <= cols - x; wid = wid + 3){//宽 

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
		   //计算三矩阵垂直型
		   for (int x = 0; x < cols; x++){//列
			   for (int y = 0; y < rows - 2; y = y++){  //行
				   for (int wid = 1; wid <= rows - x; wid++){//宽
					   for (int len = 3; len <= cols - y; len = len + 3){//长 

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
		   //计算四矩阵（只有一种类型）
		   for (int y = 0; y <rows - 1; y++){//行
			   for (int x = 0; x < cols - 1; x = x++){  //列
				   for (int len = 2; len <= cols - y; len = len + 2){//长
					   for (int wid = 2; wid <= rows - x; wid = wid + 2){//宽 

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

		   //计算9矩阵（只有一种类型）
		   for (int y = 0; y <rows - 3; y++){//行
			   for (int x = 0; x < cols - 3; x = x++){  //列
				   for (int len = 3; len <= cols - y; len = len + 3){//长 

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

