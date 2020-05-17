#include <iostream>
#include<chrono>
#include <string>

using namespace std;
using namespace std::chrono;

template < class Container>
void timeExec(Container& container, int processNum, size_t accuracy = 10) {

	double timeSum = 0;

	for (auto i = 0; i < accuracy; ++i) {
		auto start = steady_clock::now();

		container.insert(processNum);

		auto stop = steady_clock::now();

		timeSum += static_cast<double>(duration_cast<microseconds>(stop - start).count());
	}

	cout << "Insert time: " << timeSum / accuracy << " mcSec" << endl;

	timeSum = 0;

	for (auto i = 0; i < accuracy; ++i) {
		auto start = steady_clock::now();

		container.erase(processNum);

		auto stop = steady_clock::now();

		timeSum += static_cast<double>(duration_cast<microseconds>(stop - start).count());
	}


	cout << "Erase time: " << timeSum / accuracy << " mcSec" << endl;


	timeSum = 0;

	for (auto i = 0; i < accuracy; ++i) {
		auto start = steady_clock::now();

		container.find(processNum);

		auto stop = steady_clock::now();

		timeSum += static_cast<double>(duration_cast<nanoseconds>(stop - start).count());
	}

	cout << "Find time: " << timeSum / accuracy << " nSec" << endl;

}


template< typename T >
class OrderedVector {
	T* _arr;
	int _size;


	int binSearch(const T& key)
	{
		int left = 0, right = _size - 1;
		int middle = 0;

		if (_arr == nullptr)
			return 0;

		if (_arr[_size - 1] < key)
			return _size;

		while (left < right)
		{
			int middle = (right + left) / 2;
			if (_arr[middle] == key)
				return middle;
			if (left >= right)
				break;
			if (_arr[middle] < key)
				left = middle + 1;
			else
				right = middle;
		}
		return right;
	}

public:
	OrderedVector() : _arr(nullptr), _size(0) {}

	T& operator[](int index) {
		return _arr[index];
	}
	int showBin(int value) {
		return binSearch(value);
	}
	~OrderedVector() {
		delete[] _arr;
		_size = 0;
	}

	void printVector() {
		for (auto i = 0; i < _size; ++i)
		{
			cout << _arr[i] << endl;
		}
	}

	void insert(const T& key) {
		const int place = binSearch(key);

		const int nsize = _size + 1;
		T* nptr = new T[nsize];
		if (place < nsize) {

			for (auto i = 0; i < place; ++i)
			{
				nptr[i] = _arr[i];
			}

			nptr[place] = key;

			for (auto i = place + 1; i < nsize; ++i)
			{
				nptr[i] = _arr[i - 1];
			}
		}
		delete[]_arr;
		_arr = nptr;
		nptr = nullptr;
		_size = nsize;

	}


	void erase(const T& key) {

		const int place = binSearch(key);


		if (_arr[place] != key)
			return;


		int nsize = _size - 1;
		T* nptr = new T[nsize];

		for (auto i = 0; i < place; ++i)
		{
			nptr[i] = _arr[i];
		}
		
			for (auto i = place + 1; i < _size; ++i)
			{
				nptr[i - 1] = _arr[i];
			}
			

		delete[]_arr;
		_arr = nptr;
		nptr = nullptr;
		_size = nsize;

	}


	void find(const T& key) {
		binSearch(key);
	}
};


template <typename T>
class Set {


	struct node
	{
		T key;
		unsigned char height;
		node* left;
		node* right;
		node(T k) { key = k; left = right = 0; height = 1; }
	};
	node* _nodeptr;

	unsigned char height(const node* p) const
	{
		return p ? p->height : 0;
	}

	int balanceFactor(const node* p) const
	{
		return height(p->right) - height(p->left);

	}
	void fixHeight(node* p)
	{
		unsigned char hl = height(p->left);
		unsigned char hr = height(p->right);
		p->height = (hl > hr ? hl : hr) + 1;
	}

	node* rotateRight(node* p) // правый поворот вокруг p (корень)
	{
		node* q = p->left;
		p->left = q->right;
		q->right = p;
		fixHeight(p);
		fixHeight(q);
		return q;
	}
	node* rotateLeft(node* q) // левый поворот вокруг q(корнень)
	{
		node* p = q->right;
		q->right = p->left;
		p->left = q;
		fixHeight(q);
		fixHeight(p);
		return p;
	}

	node* balance(node* p) // балансировка узла p
	{
		fixHeight(p);
		if (balanceFactor(p) == 2)
		{
			if (balanceFactor(p->right) < 0)
				p->right = rotateRight(p->right);
			return rotateLeft(p);
		}
		if (balanceFactor(p) == -2)
		{
			if (balanceFactor(p->left) > 0)
				p->left = rotateLeft(p->left);
			return rotateRight(p);
		}
		return p; // балансировка не нужна
	}

	node* findMin(node* p) // поиск узла с минимальным ключом в дереве p 
	{
		return p->left ? findMin(p->left) : p;
	}

	node* removeMin(node* p) // удаление узла с минимальным ключом из дерева p
	{
		if (p->left == 0)
			return p->right;
		p->left = removeMin(p->left);
		return balance(p);
	}

	node* insert_k(node* p, T k) // вставка ключа k в дерево с корнем p
	{
		if (!p) return new node(k);
		if (k < p->key)
			p->left = insert_k(p->left, k);
		else
			p->right = insert_k(p->right, k);
		return balance(p);
	}

	node* remove(node* p, T k) // удаление ключа k из дерева p
	{
		if (!p) return 0;
		if (k < p->key)
			p->left = remove(p->left, k);
		else if (k > p->key)
			p->right = remove(p->right, k);
		else //  k == p->key 
		{
			node* q = p->left;
			node* r = p->right;
			delete p;
			if (!r) return q;
			node* min = findMin(r);
			min->right = removeMin(r);
			min->left = q;
			return balance(min);
		}
		return balance(p);
	}

	void print_Set(node* p)
	{
		if (p)
		{
			print_Set(p->left);
			cout << p->key << endl;
			print_Set(p->right);
		}
	}

	
	node* find_k(node* p, T k) {
		if (!p) {
			//	cout << "element doesn't exists" << endl;
			return nullptr;
		}
		else {

			if (k < p->key)
				p->left = find_k(p->left, k);
			else if (k > p->key)
				p->right = find_k(p->right, k);
			else //  k == p->key 
			{
				//		cout << "element exists" << endl;
				return p;
			}

			return nullptr;
		}


	}



public:

	Set() :_nodeptr(nullptr) {};

	Set(T value) {
		_nodeptr = new node(value);
	}

	~Set() {
		clear_src(_nodeptr);
	}

	node* clear_src(node* p) {

		if (p) {
			clear_src(p->left);
			clear_src(p->right);
			delete p;
			return nullptr;
		}
		else
			return nullptr;


	}

	void insert(T key) {
		_nodeptr = insert_k(_nodeptr, key);
	}
	void erase(T key) {
		_nodeptr = remove(_nodeptr, key);
	}
	void printSet() {
		if (_nodeptr)
			print_Set(_nodeptr);
		else
			cout << "Set is empty";

	}

	void find(T k) {
		find_k(_nodeptr, k);
	}





};




int hashInt(int value) {
	return value * value;
}


template <typename T>
class UnorderedSet
{
	const size_t STANDARD_SIZE = 17;
	size_t _size;

	struct cell {
		int key;
		T value;
		cell* ptrnext;

		cell(int _key, T _value, cell* _next = nullptr) : key(_key), value(_value), ptrnext(_next) {}
	};
	cell** _table;
	int (*ptrHash)(T obj);


	int hash(T obj) const {
		return ptrHash(obj) % _size;
	}


	cell* findPrevious(const T& v) const {
		int position = hash(v);
		cell* tmp = _table[position];
		if (!_table[position]) // if line is empty
		{
			return nullptr;
		}
		else
		{

			if ((_table[position]->value) != v)
			{


				while (tmp->ptrnext) {

					if ((tmp->ptrnext->value) == v)
					{
						return tmp;
					}
					tmp = tmp->ptrnext;

				}
				return nullptr;
			}
			else
			{
				return _table[position];
			}
		}





	}



public:
	UnorderedSet(int (*hashFunc)(T)) :_size(STANDARD_SIZE), _table(new cell* [_size]), ptrHash(hashFunc) {

		for (auto i = 0; i < static_cast<int>(_size); ++i) {
			_table[i] = nullptr;
		}
	}

	UnorderedSet(size_t size, int (*hashFunc)(T)) : _size(size), _table(new (cell*)[_size]), ptrHash(hashFunc) {
		for (auto i = 0; i < static_cast<int>(_size); ++i) {
			_table[i] = nullptr;
		}
	}

	~UnorderedSet() {
		clear();
		delete[] _table;
		_size = 0;


	}



	void insert(const T& v)
	{
		int position = hash(v);

		if (!find(v))  // if its first appearence in table       else do nothing
		{
			if (!_table[position]) // if line is empty
			{
				_table[position] = new cell(position, v);

			}
			else
			{
				cell* tmp = _table[position];
				_table[position] = new cell(position, v, tmp);
			}
		}
	}



	cell* find(const T& v) const {
		cell* prev = findPrevious(v);

		if (!prev) // if prev = nullptr
		{
			return prev;
		}

		if (prev != _table[hash(v)]) // if prev isn't first element
		{
			return prev->ptrnext;
		}
		else
		{
			if (prev->value == v) // if we get real position as previous
			{
				return prev;

			}
			else // if previous is really first element
				return prev->ptrnext;
		}

	}

	void erase(T v) {
		cell* tmp = findPrevious(v);
		int position = hash(v);
		if (tmp) //else do nothing because elment is absent
		{
			if (tmp->value == v) // if we get real position as previous
			{
				cell* tmp2 = _table[position]->ptrnext;
				delete _table[position];
				_table[position] = tmp2;
			}
			else // if we get previous position
			{
				cell* tmp2 = tmp->ptrnext;
				tmp->ptrnext = tmp2->ptrnext;
				delete tmp2;
			}

		}



	}

	void print() const {
		for (auto i = 0; i < _size; ++i)
		{
			auto tmp = _table[i];
			while (tmp) {
				cout << tmp->value;

				if (tmp->ptrnext) {
					cout << " -> ";
				}
				else
					cout << endl;

				tmp = tmp->ptrnext;
			}

		}
	}

	void clear() {
		for (auto i = 0; i < _size; ++i)
		{
			auto tmp = _table[i];
			while (_table[i]) {

				tmp = tmp->ptrnext;

				delete _table[i];
				_table[i] = tmp;
			}

		}

		for (auto i = 0; i < _size; ++i)
			_table[i] = nullptr;

	}




};



int main() {
	

}