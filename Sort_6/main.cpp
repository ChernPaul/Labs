
#include<iostream>
#include<stdlib.h>
#include<stack>
#include<vector>
#include<chrono>

using namespace std;



template<typename T>
void fillArr(vector <T>& arr)
{
	for (size_t i = 0; i < arr.size(); ++i)
		arr[i] = rand() / 10;

}


template<typename T>
void printArr(vector <T> arr)
{
	for (size_t i = 0; i < arr.size(); ++i)
		cout << arr[i] << endl;
}


template<typename T>
void  selectionSort(vector <T>& arr) {

	for (size_t i = 0; i < arr.size(); ++i)
	{
		int jMin = i;
		for (size_t j = i + 1; j < arr.size(); ++j)
		{
			if (arr[j] < arr[jMin])
			{
				jMin = j;
			}

		}
		if (jMin != i)
		{
			swap(arr[i], arr[jMin]);
		}
	}
}


template<typename T>
void bubleSort(vector <T>& arr) {
	bool isSwap = false;
	for (size_t i = 0; i < arr.size(); ++i)
	{
		for (size_t j = 0; j < arr.size() - 1 - i; ++j)
		{
			if (arr[j] > arr[j + 1])
			{
				swap(arr[j], arr[j + 1]);
				isSwap = true;
			}
		}
		if (isSwap == false)
			break;
	}
}


/*
template<typename T>
void inputSort(vector <T>& arr)
{
	for (int i = 1; i < arr.size(); ++i) {
		bool itsSort = false;

		for (int j = i - 1; j >= 0; --j)
		{
			if (arr[j] > arr[j + 1])
			{
				swap(arr[j + 1], arr[j]);
			}
			else
				itsSort = true;

			if (itsSort)
				break;
		}
	}
}
*/


template<typename T>
void shellSort(vector <T>& arr) {
	int step = arr.size() / 2;
	while (step)
	{
		for (int i = step; i < arr.size(); ++i)
		{

			for (int j = i - step; (j >= 0) && (arr[j + step] < arr[j]); j -= step)
			{
				swap(arr[j + 1], arr[j]);
			}

		}
		step = step / 2;
	}
}




template<typename T>
void quickSort(vector <T>& arr) {

	int left = 0, right = arr.size() - 1;
	int middle;
	stack<T> stk;
	stk.push(left);
	stk.push(right);

	do
	{
		right = stk.top();
		stk.pop();
		left = stk.top();
		stk.pop();

		int i = left, j = right;
		middle = (i + j) / 2;
		T midValue = arr[middle];
		do {
			while (arr[i] < midValue)
			{
				++i;
			}

			while (arr[j] > midValue)
			{
				--j;
			}
			if (i <= j)
			{
				swap(arr[i], arr[j]);
				++i;
				--j;
			}

		} while (j >= i);

		if (j > left)
		{
			stk.push(left);
			stk.push(j);
		}
		if (i < right)
		{
			stk.push(i);
			stk.push(right);
		}





	} while (!stk.empty());

}



template<typename T>
void mergeSort(vector <T>& arr) {






	bool isMain = true;                   // first set
	bool needMoreVws;
	const int size = arr.size();

	arr.resize(size * 2);


	int i, j, k, l, outputDirect;
	do {
		// preparing for viewing
		if (isMain) {
			i = 0;
			j = size - 1;
			k = size;
			l = 2 * size - 1;
		}

		else {
			k = 0;
			l = size - 1;
			i = size;
			j = 2 * size - 1;
		}

		outputDirect = 1; needMoreVws = false;

		while (true)
		{


			if (arr[i] <= arr[j])
			{
				if (i == j)
				{

					arr[k] = arr[i];

					if (isMain)
						isMain = false;
					else
						isMain = true;
					break;
				}

				arr[k] = arr[i];
				k += outputDirect;
				++i;
				if (arr[i - 1] <= arr[i]) continue;

				do {
					arr[k] = arr[j];
					k += outputDirect;
					--j;
				} while (arr[j + 1] <= arr[j]);
			}

			else
			{

				arr[k] = arr[j];
				k += outputDirect;
				--j;
				if (arr[j + 1] <= arr[j]) continue;


				do {
					arr[k] = arr[i];
					k += outputDirect;
					++i;
				} while (arr[i - 1] <= arr[i]);
			}

			needMoreVws = true;
			outputDirect = outputDirect * (-1);      // change direction
			std::swap(k, l);
		}
	} while (needMoreVws);

	// define area
	if (!isMain) {
		for (auto i = 0; i < size; ++i)
		{
			arr[i] = arr[i + size];
		}
	}
	arr.resize(size);
}






int main() {
	srand(static_cast<unsigned int>(time(0)));
	size_t numElem = 10000;
	vector <int> arr;
	arr.resize(numElem);
	fillArr(arr);
	printArr(arr);
	cout << endl;
	auto start = chrono::high_resolution_clock::now();
	/*
	selectionSort(arr);
	bubleSort(arr);
	shellSort(arr);
	quickSort(arr);
	mergeSort(arr);
	*/
	
	auto end = chrono::high_resolution_clock::now();
	printArr(arr);
	chrono::duration<double> duration = end - start;
	cout << "Duration: " << duration.count() << " sec" << endl;
	cout << "Number of elements:" << numElem << endl;



}
