
#include<iostream>
#include<ceres/ceres.h>
using namespace std;
using namespace ceres;


struct CostFuntorF1 {template <typename T>
    bool operator()(const T* const x1, const T* const x2, T* residuals) const {
        residuals[0] = x1[0] + T(10)*x2[0];
        return true;
    }
};
struct CostFuntorF2 {
    template <typename T>
    bool operator()(const T* const x3, const T* const x4, T* residuals) const {
        residuals[0] = sqrt(5)*(x3[0] - x4[0]);
        return true;
    }
};
struct CostFuntorF3 {
    template <typename T>
    bool operator()(const T* const x2, const T* const x3, T* residuals) const {
        residuals[0] = (x2[0] - T(2)*x3[0])*(x2[0] - T(2)*x3[0]);
        return true;
	}
};
struct CostFuntorF4 {
    template <typename T>
    bool operator()(const T* const x1, const T* const x4, T* residuals) const {
        residuals[0] = sqrt(10)*(x1[0] - x4[0])*(x1[0] - x4[0]);
        return true;
	}
};
int main(int argc, char** argv)
{
    const double initial_x1 = 10, initial_x2 = 5,
    initial_x3 = 2, initial_x4 = 1;
    double x1 = initial_x1, x2 = initial_x2,
    x3 = initial_x3, x4 = initial_x4;
    Problem problem;
    Solver::Options options;
    //control whether the log is output to STDOUT
    options.minimizer_progress_to_stdout = true;
    Solver::Summary summary;
    CostFunction* costfunction1 =
    	new AutoDiffCostFunction<CostFuntorF1,1,1,1>(new CostFuntorF1);
    CostFunction* costfunction2 =
    	new AutoDiffCostFunction<CostFuntorF2,1,1,1>(new CostFuntorF2);
    CostFunction* costfunction3 =
    	new AutoDiffCostFunction<CostFuntorF3,1,1,1>(new CostFuntorF3);
    CostFunction* costfunction4 =
    	new AutoDiffCostFunction<CostFuntorF4,1,1,1>(new CostFuntorF4);
    problem.AddResidualBlock(costfunction1,NULL,&x1,&x2);
    problem.AddResidualBlock(costfunction2,NULL,&x3,&x4);
    problem.AddResidualBlock(costfunction3,NULL,&x2,&x3);
    problem.AddResidualBlock(costfunction4,NULL,&x1,&x4);
    Solve(options,&problem,&summary);
    cout << "x1 : " << initial_x1 << "->" << x1 << endl;
    cout << "x2 : " << initial_x2 << "->" << x2 << endl;
    cout << "x3 : " << initial_x3 << "->" << x3 << endl;
    cout << "x4 : " << initial_x4 << "->" << x4 << endl;
    return 0;
}

