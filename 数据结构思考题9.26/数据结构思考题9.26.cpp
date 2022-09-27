#include <iostream>

using namespace std;

class SparseMatrix;
//前面的声明
class MatrixTerm {
	friend class SparseMatrix;
public:
	MatrixTerm(int r = -1, int c = -1, int v = -1) :row(r), col(c), value(v) {}//临时加入的带初始值的构造，用于程序测试，没有数据检查
private:
	int row, col, value;
};

class SparseMatrix
{
public:
	SparseMatrix(int r, int c, int t);
	SparseMatrix(const SparseMatrix& sm);
	SparseMatrix(int r, int c, int t, MatrixTerm sm[]);//临时加入的带初始值的构造，用于程序测试，没有数据检查
	~SparseMatrix();

	void print();

	SparseMatrix MyTranspose();
	SparseMatrix FastTranspose();

	//void enLarge();暂时不需要扩大容量


private:
	int rows, cols, terms, capacity;
	MatrixTerm* smArray;

};

SparseMatrix::SparseMatrix(int r, int c, int t)
	:rows(r), cols(c), terms(t), capacity(t)
{
	if (t > r * c)throw out_of_range("Terms out of range!");
	this->smArray = new MatrixTerm[capacity];
}

SparseMatrix::SparseMatrix(const SparseMatrix& sm)
	:rows(sm.rows), cols(sm.cols), terms(sm.terms), capacity(sm.capacity)
{
	this->smArray = new MatrixTerm[capacity];
	for (size_t i = 0; i < terms; i++)
		this->smArray[i] = sm.smArray[i];
}

SparseMatrix::SparseMatrix(int r, int c, int t, MatrixTerm sm[])//临时加入的带初始值的构造，用于程序测试，没有数据检查
	:rows(r), cols(c), terms(t), capacity(t)
{
	if (t > r * c)throw out_of_range("Terms out of range!");
	this->smArray = new MatrixTerm[capacity];
	for (size_t i = 0; i < t; i++)
		smArray[i] = sm[i];
}

SparseMatrix::~SparseMatrix()
{
	if (smArray != NULL)
	{
		delete[] smArray;
		smArray = NULL;
	}
}

void SparseMatrix::print()
{
	cout << "rows = " << rows << endl;
	cout << "cols = " << cols << endl;
	cout << "terms = " << terms << endl;
	cout << "row\tcol\tvalue\n";
	for (size_t i = 0; i < terms; i++)
		cout << smArray[i].row << "\t" << smArray[i].col << "\t" << smArray[i].value << endl;

}

SparseMatrix SparseMatrix::MyTranspose()
{
	SparseMatrix b(cols, rows, terms);
	if (terms > 0)
	{
		int* rowStart = new int[cols];//多加一个位置
		fill(rowStart, rowStart + cols, 0);//全部置零

		for (size_t i = 0; i < terms - 1; i++) rowStart[smArray[i].col + 1]++;//把行大小放到要加的位置上
		for (size_t i = 1; i < cols; i++) rowStart[i] += rowStart[i - 1];//每一个位置加上之前的数据

		for (size_t i = 0; i < terms; i++)
		{
			int j = rowStart[smArray[i].col];
			b.smArray[j].row = smArray[i].col;
			b.smArray[j].col = smArray[i].row;
			b.smArray[j].value = smArray[i].value;
			rowStart[smArray[i].col]++;
		}
		delete[] rowStart;
	}
	return b;
}

SparseMatrix SparseMatrix::FastTranspose()
{//Return the transpose of * this in 0 (terms+cols) time
	SparseMatrix b(cols, rows, terms);
	if (terms > 0)
	{//nonzero matrix
		int* rowSize = new int[cols];
		int* rowStart = new int[cols];
		//compute rowsize[i]=number of terms in row i of b
		fill(rowSize, rowSize + cols, 0); //initialize
		for (size_t i = 0; i < terms; i++) rowSize[smArray[i].col]++;
		//rowStart[i]=starting position of row i in b
		rowStart[0] = 0;
		for (size_t i = 1; i < cols; i++) rowStart[i] = rowStart[i - 1] + rowSize[i - 1];
		for (size_t i = 0; i < terms; i++)
		{//copy from * this to b
			int j = rowStart[smArray[i].col];
			b.smArray[j].row = smArray[i].col;
			b.smArray[j].col = smArray[i].row;
			b.smArray[j].value = smArray[i].value;
			rowStart[smArray[i].col]++;
		}//end of for
		delete[] rowSize;
		delete[] rowStart;
	}//end of if

	return b;
}


int main()
{
	const int size = 20;//测试数据大小

	//构建测试数据
	MatrixTerm sm[size];
	for (size_t i = 0; i < size; i++)
		sm[i] = MatrixTerm(i / 3, i % 4, i);
	SparseMatrix test(10, 10, size, sm);

	cout << endl << "原矩阵：\n";

	test.print();

	cout << endl << endl << "快速转置：\n";

	test.FastTranspose().print();

	cout << endl << endl << "我的快速转置：\n";

	test.MyTranspose().print();


	return 0;
}
