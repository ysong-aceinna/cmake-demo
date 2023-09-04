#include <iostream>
using namespace std;

#include <string.h>
#include <time.h>
#include <chrono>  

#include <Eigen/Dense>
#include <Eigen/Geometry>
using namespace Eigen;


void EigenTest1()
{
	Eigen::Quaterniond q(2, 0, 1, -3);
	q = q.normalized(); //验证了下，不做归一化不影响后边结果。
	cout << q.w() << ", " << q.x() << ", " << q.y() << ", " << q.z() << endl;

	Eigen::Matrix3d R = q.toRotationMatrix();
	Eigen::Vector3d v(0, 0, 9.8);

	Eigen::Vector3d v1 = q * v;
	cout << v1[0] << ", " << v1[1] << ", " << v1[2] << endl;

	Eigen::Vector3d v2 = R * v;
	cout << v2[0] << ", " << v2[1] << ", " << v2[2] << endl;
}

void EigenTest2()
{
	// Matrix3d A;
    Eigen::MatrixXd A0 = Eigen::MatrixXd::Zero(3, 6); // [7, 14]

	A0 << 1, 1, 1, 1, 1, 1,
		2, -1, -1, 2, -1, -1,
		2, -4, 5, 2, -4, 5;
 
	//  矩阵的秩
	// JacobiSVD<Eigen::MatrixXd> svd(A0);
    // std::cout<<"A :\n"<<A0<<std::endl;
    // std::cout<<"rank:\n"<<svd.rank()<<std::endl;

	Eigen::MatrixXd A = A0.transpose();
    Eigen::HouseholderQR<Eigen::MatrixXd> qr;

	qr.compute(A);
	MatrixXd R = qr.matrixQR().triangularView<Upper>();
	MatrixXd Q = qr.householderQ();
	// MatrixXd Q1 = qr.householderQ() * MatrixXd::Identity(6, 3);
	cout << "HouseholderQR" << endl;
	cout << "A " << endl << A << endl << endl;
	cout << "qr.matrixQR()" << endl << qr.matrixQR() << endl << endl;
	cout << "Q " << endl << Q << endl << endl;
	cout << "Q*Q " << endl << Q * Q.transpose() << endl << endl;
	cout << "R" << endl << R << endl << endl;
	cout << "Q*R" << endl << Q * R << endl;

}

int main(int argc, char** argv)
{
	EigenTest2();
	return 0;
}
