#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//--------------------------这里是一些定义的结构体和数据类型---------
//纯C里面定义的布尔类型
typedef enum { False = 0, True = 1 }Bool;
//定义矩阵元素的类型为matrixType
typedef double matrixType;

//此结构体用来表示矩阵，其中row为行，column为列，height为高，array用来存放矩阵元素(用一维来模拟二维/三维)
typedef struct
{
	unsigned  row, column, height;
	matrixType *array; //使用时，必须对*array进行初始化
}Matrix;

//---------下面是QR分解，求解线性方程所用到的一些函数-----------
/*
matrix为要设置大小并分配内存的矩阵，row、column、height分别为行，列，高。
函数调用成功则则返回true,否则返回false
*/
Bool SetMatrixSize(Matrix *matrix, const unsigned row, const unsigned column, const unsigned height)
{
	unsigned size = row * column * height * sizeof(matrixType);
	if (size <= 0)
	{
		return False;
	}

	matrix->array = (matrixType*)malloc(size);

	//如果分配内存成功
	if (matrix->array)
	{
		matrix->row = row;
		matrix->column = column;
		matrix->height = height;
		return True;
	}
	else
	{
		matrix->row = matrix->column = matrix->height = 0;
		return False;
	}
}

//设置Matrix矩阵中的所有元素大小为ele
void SetMatrixEle(Matrix *matrix, matrixType ele)
{
	unsigned size = matrix->row * matrix->column * matrix->height;
	unsigned i;

	for (i = 0; i < size; ++i)
	{
		matrix->array[i] = ele;
	}
}

//设置Matrix矩阵中的所有元素大小为0
void SetMatrixZero(Matrix*matrix)
{
	SetMatrixEle(matrix, 0);
}

//判断矩阵是否为空，若为空则返回1，否则返回0
Bool IsNullMatrix(const Matrix* matrix)
{
	unsigned size = matrix->row * matrix->column * matrix->column;

	if (size <= 0 || matrix->array == NULL)
	{
		return True;
	}
	return False;
}

//销毁矩阵，即释放为矩阵动态分配的内存,并将矩阵的长宽高置0
void DestroyMatrix(Matrix *matrix)
{
	if (!IsNullMatrix(matrix))
	{
		free(matrix->array);
		matrix->array = NULL;
	}

	matrix->row = matrix->column = matrix->height = 0;
}

//计算矩阵可容纳元素个数，即return row*column*height
unsigned MatrixCapacity(const Matrix*matrix)
{
	return matrix->row * matrix->column * matrix->height;
}


//||matrix||_2  求A矩阵的2范数
matrixType MatrixNorm2(const Matrix *matrix)
{
	unsigned size = matrix->row * matrix->column *matrix->height;
	unsigned i;
	matrixType norm = 0;

	for (i = 0; i < size; ++i)
	{
		norm += (matrix->array[i]) *(matrix->array[i]);
	}

	return (matrixType)sqrt(norm);
}

//matrixB = matrix(:,:,height)即拷贝三维矩阵的某层，若matrix为二维矩阵，需将height设置为0
Bool CopyMatrix(Matrix* matrixB, Matrix *matrix, unsigned height)
{
	unsigned size, i;
	Matrix matrixA;

	//判断height值是否正确
	if (height < 0 || height >= matrix->height)
	{
		printf("ERROR: CopyMatrix() parameter error！\n");
		return False;
	}

	//将matrix(:,:,height) 转换为二维矩阵matrixA
	matrixA.row = matrix->row;
	matrixA.column = matrix->column;
	matrixA.height = 1;
	matrixA.array = matrix->array + height * matrix->row * matrix->column;

	//判断两矩阵指向的内存是否相等
	if (matrixA.array == matrixB->array)
	{
		return True;
	}

	//计算matrixA的容量
	size = MatrixCapacity(&matrixA);
	//判断matrixB的容量与matrixA的容量是否相等
	if (MatrixCapacity(matrixB) != size)
	{
		DestroyMatrix(matrixB);
		SetMatrixSize(matrixB, matrixA.row, matrixA.column, matrixA.height);
	}
	else
	{
		matrixB->row = matrixA.row;
		matrixB->column = matrixA.column;
		matrixB->height = matrixA.height;
	}

	for (i = 0; i < size; ++i)
	{
		matrixB->array[i] = matrixA.array[i];
	}

	return True;
}

//matrixC = matrixA * matrixB
Bool MatrixMulMatrix(Matrix *matrixC, const Matrix* matrixA, const Matrix* matrixB)
{
	size_t row_i, column_i, i;
	size_t indexA, indexB, indexC;
	matrixType temp;
	Matrix tempC;

	if (IsNullMatrix(matrixA) || IsNullMatrix(matrixB))
	{
		return False;
	}

	if (matrixA->column != matrixB->row)
	{
		return False;
	}

	if (MatrixCapacity(matrixC) != matrixA->row * matrixB->column)
	{
		SetMatrixSize(&tempC, matrixA->row, matrixB->column, 1);
	}
	else
	{
		tempC.array = matrixC->array;
		tempC.row = matrixA->row;
		tempC.column = matrixB->column;
		tempC.height = 1;
	}

	for (row_i = 0; row_i < tempC.row; ++row_i)
	{
		for (column_i = 0; column_i < tempC.column; ++column_i)
		{
			temp = 0;
			for (i = 0; i < matrixA->column; ++i)
			{
				indexA = row_i * matrixA->column + i;
				indexB = i * matrixB->column + column_i;

				temp += matrixA->array[indexA] * matrixB->array[indexB];
			}
			indexC = row_i * tempC.column + column_i;

			tempC.array[indexC] = temp;
		}
	}


	if (tempC.array != matrixC->array)
	{
		DestroyMatrix(matrixC);

		matrixC->array = tempC.array;
	}

	matrixC->row = tempC.row;
	matrixC->column = tempC.column;
	matrixC->height = tempC.height;



	return True;
}

//对vector中所有元素排序，sign= 0 时为升序，其余为降序
void SortVector(Matrix *vector, int sign)
{
	matrixType mid;
	int midIndex;
	int size = MatrixCapacity(vector);
	int i, j;

	if (0 == sign)
	{
		for (i = 0; i < size; ++i)
		{
			mid = vector->array[i];
			midIndex = i;
			for (j = i + 1; j < size; ++j)
			{
				if (mid > vector->array[j])
				{
					mid = vector->array[j];
					midIndex = j;
				}
			}

			vector->array[midIndex] = vector->array[i];
			vector->array[i] = mid;
		}
	}
	else
	{
		for (i = 0; i < size; ++i)
		{
			mid = vector->array[i];
			midIndex = i;
			for (j = i + 1; j < size; ++j)
			{
				if (mid < vector->array[j])
				{
					mid = vector->array[j];
					midIndex = j;
				}
			}

			vector->array[midIndex] = vector->array[i];
			vector->array[i] = mid;
		}
	}
}

//打印矩阵
void PrintMatrix(const Matrix *matrix)
{
	size_t row_i, column_i, height_i, index;

	for (height_i = 0; height_i < matrix->height; ++height_i)
	{
		(matrix->height == 1) ? printf("[:,:] = \n") : printf("[%d,:,:] = \n", height_i);

		for (row_i = 0; row_i < matrix->row; ++row_i)
		{
			for (column_i = 0; column_i < matrix->column; ++column_i)
			{
				index = height_i * matrix->row * matrix->column + row_i * matrix->column + column_i;
				printf("%12.4g", matrix->array[index]);
			}
			printf("\n");
		}
	}
}

//----------------------QR分解-------------------------------------------

//将A分解为Q和R
void QR(Matrix *A, Matrix *Q, Matrix *R)
{
	unsigned  i, j, k, m;
	unsigned size;
	const unsigned N = A->row;
	matrixType temp;

	Matrix a, b;

	//如果A不是一个二维方阵，则提示错误，函数计算结束
	if (A->row != A->column || 1 != A->height)
	{
		printf("ERROE: QR() parameter A is not a square matrix!\n");
		return;
	}

	size = MatrixCapacity(A);
	if (MatrixCapacity(Q) != size)
	{
		DestroyMatrix(Q);
		SetMatrixSize(Q, A->row, A->column, A->height);
		SetMatrixZero(Q);
	}
	else
	{
		Q->row = A->row;
		Q->column = A->column;
		Q->height = A->height;
	}

	if (MatrixCapacity(R) != size)
	{
		DestroyMatrix(R);
		SetMatrixSize(R, A->row, A->column, A->height);
		SetMatrixZero(R);
	}
	else
	{
		R->row = A->row;
		R->column = A->column;
		R->height = A->height;
	}

	SetMatrixSize(&a, N, 1, 1);
	SetMatrixSize(&b, N, 1, 1);

	for (j = 0; j < N; ++j)
	{
		for (i = 0; i < N; ++i)
		{
			a.array[i] = b.array[i] = A->array[i * A->column + j];
		}

		for (k = 0; k < j; ++k)
		{
			R->array[k * R->column + j] = 0;

			for (m = 0; m < N; ++m)
			{
				R->array[k * R->column + j] += a.array[m] * Q->array[m * Q->column + k];
			}

			for (m = 0; m < N; ++m)
			{
				b.array[m] -= R->array[k * R->column + j] * Q->array[m * Q->column + k];
			}
		}

		temp = MatrixNorm2(&b);
		R->array[j * R->column + j] = temp;

		for (i = 0; i < N; ++i)
		{
			Q->array[i * Q->column + j] = b.array[i] / temp;
		}
	}

	DestroyMatrix(&a);
	DestroyMatrix(&b);
}

//----------------------使用特征值计算矩阵特征向量-----------------
//eigenVector为计算结果即矩阵A的特征向量
//eigenValue为矩阵A的所有特征值，
//A为要计算特征向量的矩阵
void Eigenvectors(Matrix *eigenVector, Matrix *A, Matrix *eigenValue)
{
	unsigned i, j, q;
	unsigned count;
	int m;
	const unsigned NUM = A->column;
	matrixType eValue;
	matrixType sum, midSum, mid;
	Matrix temp;

	SetMatrixSize(&temp, A->row, A->column, A->height);

	for (count = 0; count < NUM; ++count)
	{
		//计算特征值为eValue，求解特征向量时的系数矩阵
		eValue = eigenValue->array[count];
		CopyMatrix(&temp, A, 0);
		for (i = 0; i < temp.column; ++i)
		{
			temp.array[i * temp.column + i] -= eValue;
		}

		//将temp化为阶梯型矩阵
		for (i = 0; i < temp.row - 1; ++i)
		{
			mid = temp.array[i * temp.column + i];
			for (j = i; j < temp.column; ++j)
			{
				temp.array[i * temp.column + j] /= mid;
			}

			for (j = i + 1; j < temp.row; ++j)
			{
				mid = temp.array[j * temp.column + i];
				for (q = i; q < temp.column; ++q)
				{
					temp.array[j * temp.column + q] -= mid * temp.array[i * temp.column + q];
				}
			}
		}
		midSum = eigenVector->array[(eigenVector->row - 1) * eigenVector->column + count] = 1;
		for (m = temp.row - 2; m >= 0; --m)
		{
			sum = 0;
			for (j = m + 1; j < temp.column; ++j)
			{
				sum += temp.array[m * temp.column + j] * eigenVector->array[j * eigenVector->column + count];
			}
			sum = -sum / temp.array[m * temp.column + m];
			midSum += sum * sum;
			eigenVector->array[m * eigenVector->column + count] = sum;
		}

		midSum = sqrt(midSum);
		for (i = 0; i < eigenVector->row; ++i)
		{
			eigenVector->array[i * eigenVector->column + count] /= midSum;
		}
	}
	DestroyMatrix(&temp);
}
int main()
{
	const unsigned NUM = 50; //最大迭代次数

	unsigned N = 3;
	unsigned k;

	Matrix A, Q, R, temp;
	Matrix eValue;


	//分配内存
	SetMatrixSize(&A, N, N, 1);
	SetMatrixSize(&Q, A.row, A.column, A.height);
	SetMatrixSize(&R, A.row, A.column, A.height);
	SetMatrixSize(&temp, A.row, A.column, A.height);
	SetMatrixSize(&eValue, A.row, 1, 1);

	//A设置为一个简单矩阵
	A.array[0] = 1;
	A.array[1] = 2;
	A.array[2] = 3;
	A.array[3] = 2;
	A.array[4] = 15;
	A.array[5] = 6;
	A.array[6] = 3;
	A.array[7] = 6;
	A.array[8] = 1;

	//拷贝A矩阵元素至temp
	CopyMatrix(&temp, &A, 0);

	//初始化Q、R所有元素为0
	SetMatrixZero(&Q);
	SetMatrixZero(&R);
	//使用QR分解求矩阵特征值
	for (k = 0; k < NUM; ++k)
	{
		QR(&temp, &Q, &R);
		MatrixMulMatrix(&temp, &R, &Q);
	}
	//获取特征值，将之存储于eValue
	for (k = 0; k < temp.column; ++k)
	{
		eValue.array[k] = temp.array[k * temp.column + k];
	}

	//对特征值按照降序排序
	SortVector(&eValue, 1);

	//根据特征值eValue，原始矩阵求解矩阵特征向量Q
	Eigenvectors(&Q, &A, &eValue);

	//打印特征值
	printf("特征值：");
	PrintMatrix(&eValue);

	//打印特征向量
	printf("特征向量");
	PrintMatrix(&Q);
	DestroyMatrix(&A);
	DestroyMatrix(&R);
	DestroyMatrix(&Q);
	DestroyMatrix(&eValue);
	DestroyMatrix(&temp);

	return 0;
}
