

#include <iostream>
#include <math.h>
using namespace std;
#define zero 1e-10
#define N 4   //系数矩阵的纬度

/****************************************************************
**************高斯消元法解方程组**************
    Ref: https://blog.csdn.net/pengwill97/article/details/77200372
         https://blog.csdn.net/pengwill97/article/details/77282328

    Step:
    1. 构造增广阵：即把方程的系数矩阵和方程等号后边的值，构造成一个增广阵。
    2. 消元：
        2.1 遍历第1列，找到绝对值最大的那个元素，将其对应的行和第1行对调。
        2.2. 化第1行的第1个系数为1.
        2.3. 将第1行下方的所有行的第1个元素消为0.
        2.4. 仿照step 2.1-2.3, 遍历第n列，对调最大元素对应的行，消第n行下边对应元素为0。
    3. 回代求解。 


**************高斯消元法求逆矩阵**************
    Ref:https://zhuanlan.zhihu.com/p/130810766
        https://blog.csdn.net/qq_36691249/article/details/88603823

    Step:
    1. 构造增广阵：即把方程的系数矩阵和同尺寸的单位阵，构造成一个增广阵。
    2. 消元：
        2.1 遍历第1列，找到绝对值最大的那个元素，将其对应的行和第1行对调。
        2.2. 化第1行的第1个系数为1.
        2.3. 将第1行下方的所有行的第1个元素消为0.
        2.4. 仿照step 2.1-2.3, 遍历第n列，对调最大元素对应的行，消第n行下边对应元素为0。
    3. 右半部分就是逆矩阵。
/****************************************************************


/****************************************************************
 * @brief 高斯消元法解方程。
 * 
 * num: 方程系数矩阵和方程等号后边的值，构造的增广阵数组。
 * 
 * n: 系数矩阵的尺寸。
 * 
 * x: 方程解。
 * 
 * return: 
 ****************************************************************/
int gauss(double num[N][N+1],int n,double x[])
{
	for(int i = 0; i < n; i++)
    {
        //循环n次,第i轮循环行为i~n-1，列为i~n
		int maxRow = i;//maxRow记录系数最大的行，作为被减行减小误差
		for(int j = i+1; j < n; j++)
        {
			if(abs(num[j][i]) > abs(num[maxRow][i]))
            {
                maxRow = j;
            }
		}

		if(abs(num[maxRow][i]) < zero) 
        {
            x[i] = 0; // 若该列全0, 则将对应的解设为0.
            continue;
        }

		if(maxRow != i)
        {
            //交换最大行到i行,使之保持为上三角矩阵
			for(int j = i; j < n+1; j++)
            {
				swap(num[maxRow][j],num[i][j]);
			}
		}
		for(int j = n; j >= i; j--)
        {
            //化最大行第一个系数为1
			num[i][j] /= num[i][i];//从后向前除以系数,否则需要临时变量记录[i][i]的系数
		}

		for(int j = i+1; j < n; j++)
        {
            //被系数行减去
			for(int k = n; k >= i; k--)
            {
				num[j][k] -= num[j][i] * num[i][k];//减去了系数行乘以对应系数
			}
		}
	}

	for(int i = n-1; i >= 0; i--)
    {
        //逆向求解向量
		x[i] = num[i][n];//赋初值使得ax=b
		for(int j = i+1; j < n; j++)
        {
            x[i] -= num[i][j] * x[j];//减去其他解向量
        }
	}

	return 1;
}

// 测试高斯消元法解方程。
void test1()
{
	double x[N];//存储解向量x
	double num[N][N+1] = 
    {
        {0,-0.000030500000000,-0.999695000000000, -1, 0.999390093955250},
        {0,-0.258776800000000,-0.965759039999999, -1, 0.999655955559960},
        {0,-0.706893750000000,-0.706820550000000, -1, 0.999294063691365},
        {0,-0.865963950000001,-0.499749900000000, -1, 0.999643525249615}
    };

	if(gauss(num,N,x))
    {
		for(int i = 0; i < N;i++)
        {
			printf("%.10lf\n",x[i]);
		}
	}
    else
    {
		printf("No Solution");
	}
}

/****************************************************************
 * @brief 高斯消元法求逆矩阵。
 * 
 * arc: 输入矩阵。
 * 
 * n: 系数矩阵的尺寸。
 * 
 * ans: 待求的逆矩阵。
 * 
 * return: 
 ****************************************************************/
int Matrix_inverse(double arc[N][N], int n, double ans[N][N])//计算矩阵的逆
{
	int i, j, k;//列
	double max, tempA, tempB, P;
	int max_num;
	double arcs[N][N];
	memcpy(arcs, arc, n*n*8);
	for (i = 0; i < n; i++)
	{
		ans[i][i] = 1;
	}
	for (i = 0; i < n; i++)//第i列
	{
		max = fabs(arcs[i][i]);
		max_num = i;
		for (j = i + 1; j < n; j++)//选出主元
		{
			if (fabs(arcs[j][i]) > max)
			{
				max = fabs(arcs[j][i]);
				max_num = j;
			}
		}

		if (max == 0)
		{
			printf("Error!"); // 矩阵不可逆。
			return -1;
		}

		for (k = 0; k < n; k++)//交换行
		{
			tempA = arcs[i][k];
			arcs[i][k] = arcs[max_num][k];
			arcs[max_num][k] = tempA;
			tempB = ans[i][k];
			ans[i][k] = ans[max_num][k];
			ans[max_num][k] = tempB;
		}
		for (k = i + 1; k < n; k++)
		{
			P = arcs[k][i] / arcs[i][i];
			for (j = 0; j < n; j++)
			{
				arcs[k][j] = arcs[k][j] - arcs[i][j] * P;
				ans[k][j] = ans[k][j] - ans[i][j] * P;
			}
		}
	}
	for (i = 0; i < n; i++)//行
	{
		P = arcs[i][i];
		for (j = i; j < n; j++)
		{
			arcs[i][j] = arcs[i][j] / P;
		}
		for (j = 0; j < n; j++)
		{
			ans[i][j] = ans[i][j] / P;
		}
	}
	for (i = n - 1; i > 0; i--)
	{
		for (j = i - 1; j >= 0; j--)
		{
			for (k = 0; k < n; k++)
			{
				ans[j][k] = ans[j][k] - ans[i][k] * arcs[j][i];
			}
		}
	}
    
    return 0;
}

// 测试高斯消元法求逆矩阵。
void test2()
{
	static double a[4][4]={{21,2,3,-4},  
						{5,6,27,-8},  
						{9,10,11,-12},  
						{33,14,15,16}};

    // static double a[N][N]=
    // {{0.000000000055815,-0.000000000139537,-0.000006097013898, 0.000012200000000},
    // {-0.000000000139537, 1.999640116874741,-0.000140294027795,-0.000030500000000},
    // {-0.000006097013898,-0.000140294027795, 1.999493876803320, 0.000725900000000},
    // {0.000012200000000,-0.000030500000000, 0.000725900000000, 4.000000000000000}};

	int i,j,k,m;  
     
	static double inversA[N][N];
    static double unitMatrix[N][N];
 
    //高斯消元法
	int rev = Matrix_inverse(a, N, inversA);   //计算矩阵的逆，结果在inversA中  
     
	//计算原矩阵与所求得的逆矩阵的乘积
	for (i = 0; i < N; i ++)
	{
		for (j = 0; j < N; j ++)
		{
			for(k = 0; k < N; k ++)
			{
			    unitMatrix[i][j] = unitMatrix[i][j] + a[i][k] * inversA[k][j];
			}
		}
	}

    printf("MAT A IS:\n");  //原矩阵
    for (i=0; i< N; i++)  
    {  
        for (j=0; j< N; j++)  
            printf("%13.4f",a[i][j]);  
        printf("\n");  
    }  

    printf("\nMAT A- IS:\n");  //逆矩阵
    for (i= 0; i< N; i++)  
    {  
        for (j=0; j< N; j++)  
            printf("%13.4f",inversA[i][j]);  
        printf("\n");  
    }  

    printf("\nMAT c- IS:\n");  //原矩阵与其逆矩阵的乘积
    for (i=0; i< N; i++)  
    {  
        for (j=0; j< N; j++)  
            printf("%13.4f",unitMatrix[i][j]);  
        printf("\n");  
    }         

	exit(EXIT_SUCCESS);
}


int main(int argc,char** argv)
{

    test1();
    test2();
    return 0;
}
