#include "pch.h"
#include <iostream>
#include <iomanip>

using namespace std;

struct Clickues
{
	int **matrix;
	int numOfVertex;
	Clickues *adress;
};

int** createMatrix(int &);
void fillMatrix(int **, int);
void showMatrix(int **, int);
Clickues* clickue(Clickues *, int **, int);
int** transformToInclude(int **, int &, int);
int** transformToNonInclude(int **, int &, int);
Clickues* popListOfClickues(Clickues *);
void transformMatrixToAdd(int **, int);
void showClick(int **, int);

int main()
{
	int numOfVertex;
	int **matrix = createMatrix(numOfVertex);
	cout << "Fill the matrix:" << endl;
	fillMatrix(matrix, numOfVertex);
	
	cout << "Enter:\n1 - clicks searching;\n2 - independent sets searching." << endl;
	int choice;
	cin >> choice;
	cin.ignore(32767, '\n');
	
	system("cls");
	
	cout << "Matrix:" << endl;
	showMatrix(matrix, numOfVertex);
	
	if (choice == 2)
		transformMatrixToAdd(matrix, numOfVertex);

	Clickues *listOfClickues = nullptr;
	listOfClickues = clickue(listOfClickues, matrix, numOfVertex);
	
	cout << "\nMax clicks:" << endl;
	listOfClickues = popListOfClickues(listOfClickues);

	matrix = nullptr;

	system("pause");
	return 0;
}

int** createMatrix(int &numOfVertex)
{
	int number;
	/*cout << "Enter number of vertexes:" << endl;
	cin >> number;
	cin.ignore(32767, '\n');*/
	number = 8;
	numOfVertex = ++number;
	int **matrix = new int*[numOfVertex];
	for (int i = 0; i < numOfVertex; i++)
		matrix[i] = new int[numOfVertex];
	return matrix;
}

void fillMatrix(int **matrix, int numOfVertex)
{
	for (int i = 0; i < numOfVertex; i++)
		matrix[i][0] = i;
	for (int j = 1; j < numOfVertex; j++)
		matrix[0][j] = j;
	for(int i = 1; i < numOfVertex; i++)
		for (int j = 1; j < numOfVertex; j++)
			{
				cin >> matrix[i][j];
				cin.ignore(32767, '\n');
			}
}

void showMatrix(int **matrix, int numOfVertex)
{
	for (int i = 0; i < numOfVertex; i++)
	{
		for (int j = 0; j < numOfVertex; j++)
			cout << setw(4) << matrix[i][j];
		cout << endl;
	}
}

void showClick(int **matrix, int numberOfV)
{
	for (int i = 1; i < numberOfV; i++)
		cout << setw(3) << matrix[i][0];
	cout << endl;
}

Clickues* clickue(Clickues *head, int **matrix, int numberOfV)
{
	bool сlickTest = true;
	for (int i = 1; i < numberOfV; i++)
	{
		for (int j = 1; j < numberOfV; j++)
			if (matrix[i][j] == 0 && i != j)
			{
				сlickTest = false;
				break;
			}
		if (!сlickTest)
			break;
	}
	if (сlickTest)
	{
		Clickues *tmp = new Clickues;
		tmp->matrix = matrix;
		tmp->numOfVertex = numberOfV;
		tmp->adress = head;
		return tmp;
	}

	int iMax, maxNumberOfZero = 0;
	for (int i = 1; i < numberOfV; i++)
	{
		int numberOfZero = 0;
		for (int j = 1; j < numberOfV; j++)
		{
			if (matrix[i][j] == 0)
				numberOfZero++;
		}
		if (numberOfZero > maxNumberOfZero)
		{
			maxNumberOfZero = numberOfZero;
			iMax = i;
		}
	}

	int numOfIncMatr = numberOfV;
	int **includeV = transformToInclude(matrix, numOfIncMatr, iMax);
	head = clickue(head, includeV, numOfIncMatr);
	
	int numOfNotIncMatr = numberOfV;
	int **nonIncludeV = transformToNonInclude(matrix, numOfNotIncMatr, iMax);
	head = clickue(head, nonIncludeV, numOfNotIncMatr);

	for (int i = 0; i < numberOfV; i++)
		delete[]matrix[i];
	delete matrix;
	return head;
}

int** transformToInclude(int **prevMatrix, int &numberOfV, int v)          //преобразование матрицы для поиска кликов, включающих данную вершину v
{
	int index = 0, nonAdjacent[20];
	for (int j = 1; j < numberOfV; j++)
		if (prevMatrix[v][j] == 0 && j != v)
			nonAdjacent[index++] = j;
	int length = index;
	index = 0;

	int prevNumberOfV = numberOfV;
	numberOfV -= length;
	int **newMatrix = new int*[numberOfV];
	for (int i = 0; i < numberOfV; i++)
		newMatrix[i] = new int[numberOfV];

	
	for (int i = 0; i < prevNumberOfV; i++)
	{
		if (nonAdjacent[index] == i)
		{
			index++;
			continue;
		}
		for (int j = 0, jIndex = 0; j < prevNumberOfV; j++)
		{
			if (nonAdjacent[jIndex] == j)
			{
				jIndex++;
				continue;
			}
			newMatrix[i - index][j - jIndex] = prevMatrix[i][j];
		}
	}

	return newMatrix;
}

int** transformToNonInclude(int **prevMatrix, int &numberOfV, int v)   //преобразование матрицы для поисков кликов, не включающих данную вершину v
{
	int prevNumberOfV = numberOfV;
	numberOfV--;
	int **newMatrix = new int*[numberOfV];
	for (int i = 0; i < numberOfV; i++)
		newMatrix[i] = new int[numberOfV];

	int index = 0;
	for (int i = 0; i < prevNumberOfV; i++)
	{
		if (v == i)
		{
			index++;
			continue;
		}
		for (int j = 0, jIndex = 0; j < prevNumberOfV; j++)
		{
			if (v == j)
			{
				jIndex++;
				continue;
			}
			newMatrix[i - index][j - jIndex] = prevMatrix[i][j];
		}
	}

	return newMatrix;
}

Clickues* popListOfClickues(Clickues *head)
{
	Clickues *tmp = head;
	while (head != nullptr)
	{
		tmp = head;
		showClick(tmp->matrix, tmp->numOfVertex);
		for (int i = 0; i < head->numOfVertex; i++)
			delete[]head->matrix[i];
		delete[]head->matrix;
		head = head->adress;
		delete tmp;
	}
	return nullptr;
}

void transformMatrixToAdd(int **matrix, int numberOfV)
{
	for (int i = 1; i < numberOfV; i++)
		for (int j = 1; j < numberOfV; j++)
		{
			if (!matrix[i][j] && i != j)
				matrix[i][j] = 1;
			else if (matrix[i][j])
				matrix[i][j] = 0;
		}
}