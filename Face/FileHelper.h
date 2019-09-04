#ifndef FILEHELPER_H
#define FILEHELPER_H
#include <iostream>
#include <fstream> 
#include <cstdlib> 
#include"Msg.h"
#include <string> 
using namespace std;
using  std::string;


class FileHelper{

	string fileName;

public:ifstream input;
public:ofstream output;

	   //@filename文件名 
public:FileHelper(string  filename, int w_r){

		   this->fileName = filename;
		   if (w_r == 0){

			   input.open(fileName, ios::in);
			   if (!input)
			   {
				   cerr << "Open input file error!" << endl;
				   exit(-1);
			   }
		   }
		   else if (w_r == 1){
			   output.open(fileName, ios::out);
			   if (!input)
			   {
				   cerr << "Open output file error!" << endl;
				   exit(-1);
			   }
		   }

}

public:void write(int number){
		   output << number;
}

public:void write(long double number){
		   output << number;
}

public:void write(double number){
		   output << number;
}

public:void write(char s){
		   output << s;
}

public:void nextLine(){
		   output << endl;
}

	   //读取一行数据
public:string readline(){
		   string str;
		   char s;
		   while (((s = input.get()) != '\n') && !input.eof()){

			   str = str + s;
		   }
		   return str;
}



public:void close(){

		   if (input&&input.is_open())
			   input.close();
		   if (output&&output.is_open())
			   output.close();
}
};

#endif 