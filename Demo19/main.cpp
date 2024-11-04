

/****************************************************************
**************C语言通过QR分解计算矩阵的特征值和特征向量**************
    Ref: 
	1. QR分解原理: https://zhuanlan.zhihu.com/p/362248020
	2. 基于QR分解，计算矩阵特征值和特征向量: https://blog.csdn.net/w_weixiaotao/article/details/111868086

    Step:
    1. 对矩阵A做QR分解, A = QxR：
		1.1 Q: 对A做施密特正交化的结果.
		1.2 R: Q_T x A
    2. 基于QR分解，计算矩阵特征值和特征向量：
        2.1. 对A1做QR分解: A1 = Q1 x R1
        2.2. 构造A2 = R1 x Q1
        2.3. 对A2做QR分解: A2 = Q2 x R2
        2.4. 构造A3 = R2 x Q2
		2.5. 迭代上边2步, An会逐渐收敛为对角阵, 其对角线元素即为特征值.
        2.6. 根据特征值计算特征向量。
    3. 完成。
/****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//--------------------------这里是一些定义的结构体和数据类型---------
//定义矩阵元素的类型为matrixType
typedef double matrixType;

//打印矩阵
void PrintMatrix(const matrixType *matrix)
{
	int row_i, column_i, index;
	int N = 3;

	for (row_i = 0; row_i < N; ++row_i)
	{
		for (column_i = 0; column_i < N; ++column_i)
		{
			index = row_i * N + column_i;
			printf("%12.4g", matrix[index]);
		}
		printf("\n");
	}
}

//----------------------QR分解-------------------------------------------
//将A分解为Q和R
void QR(const matrixType *A, matrixType *Q, matrixType *R)
{
	unsigned  i, j, k, m;
	matrixType temp;

	const int N = 3;
	matrixType a[N] = {0};
	matrixType b[N] = {0};
	// matrixType A[N*N] = {0};
	// matrixType Q[N*N] = {0};
	// matrixType R[N*N] = {0};

	memset(Q, 0, N*N*sizeof(matrixType));
	memset(R, 0, N*N*sizeof(matrixType));

	for (j = 0; j < N; ++j)
	{
		for (i = 0; i < N; ++i)
		{
			a[i] = A[i * N + j];
			b[i] = A[i * N + j];
		}

		for (k = 0; k < j; ++k)
		{
			R[k * N + j] = 0;

			for (m = 0; m < N; ++m)
			{
				R[k * N + j] += a[m] * Q[m * N + k];
			}

			for (m = 0; m < N; ++m)
			{
				b[m] -= R[k * N + j] * Q[m * N + k];
			}
		}

		matrixType norm = 0;
		for (i = 0; i < N; ++i)
		{
			norm += (b[i]) *(b[i]);
		}
		temp = (matrixType)sqrt(norm);

		R[j * N + j] = temp;
		// printf("%d, %12.4g \n", j * N + j, temp);

		for (i = 0; i < N; ++i)
		{
			Q[i * N + j] = b[i] / temp;
			// printf("%d, %d, %12.4g \n", i, j, Q[i * N + j]);
		}
	}

//检查QR分解的结果是否正确.
#if 1
	printf("\nQ: \n");
	PrintMatrix(Q);
	printf("\nR: \n");
	PrintMatrix(R);
#endif
}

void AxB(    matrixType const A[],
             matrixType const B[],
             uint8_t const rowsInA,
             uint8_t const colsInA,
             uint8_t const colsInB,
             matrixType C[] )
{
    uint8_t rowNum = 0;
    uint8_t colNum = 0;
    uint8_t multIndex = 0;

    // Compute A * B
    for (rowNum = 0U; rowNum < rowsInA; rowNum++) {
        for (colNum = 0U; colNum < colsInB; colNum++) {
            C[(rowNum*colsInB) + colNum] = 0.0F;
            for (multIndex = 0U; multIndex < colsInA; multIndex++) {
                C[(rowNum*colsInB) + colNum] += A[(rowNum*colsInA) + multIndex] * B[colNum + (colsInB*multIndex)];
            }
        }
    }
}

int main()
{
	const unsigned N = 3;
	const unsigned NUM = 50; //最大迭代次数
	unsigned k;

	matrixType Q[N*N] = {0};
	matrixType R[N*N] = {0};
	matrixType temp[N*N] = {0};

	//A设置为一个简单矩阵,
	//必须确保各列向量不共面
	// 若A为非对角阵，经过多次迭代后，A_n收敛为三角阵,和R_n非常接近。
	// matrixType A[N*N] = {
	// 	1, 2, 13,
	//  4, 15, 6, 
	// 	7, 8, 1};

	// 当A为实对角阵时，经过多次迭代后，A_n收敛为对角阵。
	matrixType A[N*N] = {
		11, 2, 3,
		2, 15, 6, 
		3, 6, 12};

	//拷贝A矩阵元素至temp
	memcpy(temp, A, sizeof(A));

	//使用QR分解求矩阵特征值
	for (k = 0; k < NUM; ++k)
	{
		QR(temp, Q, R);
		AxB(R, Q, N, N, N, temp);
	}

	printf("\ntemp: \n");
	PrintMatrix(temp);

	return 0;
}



