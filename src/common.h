#pragma once

#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/Cholesky>

namespace wlib
{
	using namespace Eigen;

	typedef float datatype ;
	typedef Eigen::Matrix<datatype,Dynamic,4> Mat4;
	typedef Eigen::Matrix<datatype,Dynamic,3> Mat3;
	typedef Eigen::Matrix<datatype,Dynamic,3> VERT;
	typedef Eigen::Matrix<int,Dynamic,3> FACE;
	typedef Eigen::Matrix<int,Dynamic,3> RGB;
	typedef Eigen::SparseMatrix<datatype> SpMat; 

	typedef Eigen::Matrix<datatype,1,3> Vec3;
	typedef Eigen::Matrix<int,1,3> Vec3i;
	typedef Eigen::Matrix<int,1,3> Colori;
	typedef Eigen::Matrix<datatype,4,4> Mat4x4; 

	typedef Eigen::Matrix<datatype,Dynamic,1> VectorCol;
}

using namespace wlib;
 