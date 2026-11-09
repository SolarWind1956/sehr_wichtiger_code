#pragma once
/*
	IO_stream_in_file.hpp
*/ 


#include <iostream>
using std::iostream; using std::ios; 

using std::istream; using std::ostream; 
using std::cin; using std::cout; using std::cerr; using std::endl;

#include <fstream>
using std::fstream; 

using std::ifstream; using std::ofstream; using std::fstream;

#include <streambuf>
using std::streambuf; 

#include <string>
using std::string;

class IO_stream_in_file
{
public:
	
	istream&	assign_I_to_file(istream &I_stream, string full_file_name)
	{
		file_IO_stream.open (full_file_name, std::ifstream::in );
			if(!file_IO_stream){
				cerr << "Error with opening" << endl;
				exit(1);
			}
		IO_stream_ptr_backup 	= I_stream.rdbuf();  			// back up stream's streambuf
		IO_stream_ptr			= file_IO_stream.rdbuf();		// get file's streambuf
		I_stream.rdbuf(IO_stream_ptr);         				// assign streambuf to stream
		
		return I_stream;
	}
	ostream&	assign_O_to_file(ostream &O_stream, string full_file_name)
	{
		file_IO_stream.open (full_file_name, std::ifstream::out );
			if(!file_IO_stream){
				cerr << "Error with opening" << endl;
				exit(1);
			}
		IO_stream_ptr_backup 	= O_stream.rdbuf();  			// back up stream's streambuf
		IO_stream_ptr			= file_IO_stream.rdbuf();		// get file's streambuf
		O_stream.rdbuf(IO_stream_ptr);         				// assign streambuf to stream
		
		return O_stream;
	}
	
	void 		restore_IO_stream()
	{
		std::cin.rdbuf(IO_stream_ptr_backup);        			// restore stream's original streambuf
		file_IO_stream.close();
	}
private:
	string 			full_file_name;					// Полное имя файла с файловым потоком
	streambuf 		*IO_stream_ptr; 				// Указатель на стандартный поток ввода-вывода
	streambuf 		*IO_stream_ptr_backup;			// Бэкап указателя на стандартный поток ввода-вывода
	fstream 		file_IO_stream;					// Файловый поток ввода-вывода
};
