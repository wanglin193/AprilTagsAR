#ifndef __MAT_MATRIX_IO_HPP__
#define __MAT_MATRIX_IO_HPP__ 

#include "common.h"
 
////////////////////////////////////////////////////////////////
template <typename DerivedM>
bool readMatrix(const char *filename,Eigen::PlainObjectBase<DerivedM> & M)
{
	int cols = 0, rows = 0;
	datatype buff[1000];	

	// Read numbers from file into buffer. 	
	ifstream infile(filename);
	if(!infile.is_open()) return false; 

	string line;
	while( getline(infile, line) )
	{	
		int temp_cols = 0;
		stringstream stream(line);
		while(! stream.eof())
			stream >> buff[cols*rows+temp_cols++];

		if (temp_cols == 0) 	continue;
		if (cols == 0) 	cols = temp_cols;

		rows++;
	}
	infile.close();

	Map<DerivedM> M_(buff,rows,cols);
	M = M_.transpose();
	return true;
};

////////////////////////////////////////////////////////////////
template <typename DerivedM>
void saveMatrix(const char* name, Eigen::PlainObjectBase<DerivedM> & M)
{
	std::ofstream file(name);
	if(file.is_open()) file <<M;	
	file.close();
}

#endif