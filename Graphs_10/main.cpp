
#include<iostream>
#include <stdexcept>

using namespace std;

template <typename T>
class Queue final {

	struct node {
		T item;
		node* next;

		node(T _item, node* _next = nullptr) : item(_item), next(_next) {}
	};

	node* _queue;

public:

	Queue() : _queue(nullptr) {}

	~Queue() {
		auto tmp = _queue;
		while (tmp) {
			tmp = tmp->next;
			delete _queue;
			_queue = tmp;
		}
	}

	void push(const T& elem) {

		// If element is first
		if (!_queue) {
			_queue = new node(elem);
			return;
		}

		node* tmp = _queue;

		// If element not first
		while (tmp->next) {
			tmp = tmp->next;
		}

		tmp->next = new node(elem);

	}

	void pop() {
		if (empty())
			return;

		node* tmp = _queue->next;
		delete _queue;
		_queue = tmp;
	}

	bool empty() const {
		return _queue ? false : true;
	}

	T top() {
		if (empty())
			throw std::runtime_error("Queue is empty");

		return _queue->item;
	}
};

template <typename T>
class Stack final {
	struct node {
		T item;
		node* next;

		node(T _item, node* _next = nullptr) : item(_item), next(_next) {}
	};

	node* _stack;

public:

	Stack() : _stack(nullptr) {}

	~Stack() {
		auto tmp = _stack;
		while (tmp) {
			tmp = tmp->next;
			delete _stack;
			_stack = tmp;
		}
	}

	void push(const T& elem) {
		_stack = new node(elem, _stack);
	}

	void pop() {
		if (empty())
			return;

		node* tmp = _stack->next;
		delete _stack;
		_stack = tmp;
	}

	bool empty() const {
		return _stack ? false : true;
	}

	T top() {
		if (empty())
			throw std::runtime_error("Queue is empty");

		return _stack->item;
	}
};

// Help to bypass
class Cities {
	struct name {
		const char* cityName;
		name* pnext;
		name(const char* _name, name* next = nullptr) :cityName(_name), pnext(next) {}
	};

	name* _namesList;

public:

	Cities() : _namesList(nullptr) {}

	bool isCityHere(const char* _name) {
		if (!_namesList)
			return false;

		name* tmp = _namesList;
		while (tmp) {
			if (strcmp(tmp->cityName, _name) == 0)
				return true;

			tmp = tmp->pnext;
		}
		return false;
	}



	void pushCity(const char* _name) {
		if (!isCityHere(_name))
			_namesList = new name(_name, _namesList);
	}


	~Cities() {
		if (_namesList)
		{
			name* tmp = _namesList;
			while (tmp) {
				tmp = tmp->pnext;
				delete _namesList;
				_namesList = tmp;
			}

		}

	}


};


class Settlement {
	char* _name;
	size_t _population;

public:
	Settlement() : _name(nullptr), _population(0) {}

	Settlement(const char* name, size_t population) : _name(new char[strlen(name) + 1]), _population(population) {
		strcpy(_name, name);
	}

	~Settlement() {
		delete[] _name;


		_population = 0;
	}

	const char* getName() {
		return _name;
	}

	size_t getPopulation() {
		return _population;
	}

	void setPopulation(int population) {
		if (population <= 0) {
			cout << "Wrong parameter: population" << endl;
			return;
		}

		_population = population;
	}

	void setName(const char* name) {
		strcpy(_name, name);
	}

};


class RoadNet {

	struct Vertex {

		Settlement* _city;
		int _distance;
		Vertex* _next;
		Vertex(Settlement* city, int distance, Vertex* next = nullptr) : _city(city), _distance(distance), _next(next) {}
	};

	
	Vertex** _vertexes;
	size_t _size;

	// Help functions
	Vertex* findVertexInEdges(const char* city, int index) {
		auto tmp = _vertexes[index]->_next;

		// While we not found this city
		while (tmp && strcmp(tmp->_city->getName(), city) != 0)
			tmp = tmp->_next;

		return tmp;
	}

	bool allCityVisited(bool* arr, int size) {
		for (auto i = 0; i < size; ++i)
			if (!arr[i])
				return false;
		return true;
	}

	bool isSettlementExists(const char* name) {
		for (auto i = 0; i < _size; ++i) {
			if (strcmp(_vertexes[i]->_city->getName(), name) == 0)
				return true;
		}
		return false;
	}

	int positionInMain(const char* name) {
		for (auto i = 0; i < _size; ++i) {
			if (strcmp(_vertexes[i]->_city->getName(), name) == 0)
				return i;
		}

		return -1;
	}

	bool hasBond(const char* src, const char* dst) {

		// If src or dst do not exists
		if (!isSettlementExists(src) || !isSettlementExists(dst)) {
			cout << "Settlement doesn't exist" << endl;
			return false;
		}

		auto indexOfSrc = positionInMain(src);

		auto tmp = _vertexes[indexOfSrc]->_next;
		while (tmp) {

			if (strcmp(tmp->_city->getName(), dst) == 0)
				return true;
			tmp = tmp->_next;
		}
		return false;
	}

	void helpRecBypassDepth(Vertex* current, Stack<const char*>& stack, Cities& cities) {



		if (!current) {
			if (stack.empty())
				return;

			current = _vertexes[positionInMain(stack.top())]->_next;
			std::cout << stack.top() << std::endl;
			stack.pop();
		}


		if (!cities.isCityHere(current->_city->getName())) {
			cities.pushCity(current->_city->getName());

			stack.push(current->_city->getName());


		}
		helpRecBypassDepth(current->_next, stack, cities);

	}

public:

	RoadNet() : _vertexes(nullptr), _size(0) {}

	RoadNet(size_t size) : _vertexes(nullptr), _size(0) {
		const char* names[] = {
			"Samara", "Astrahan", "Neapol", "Liverpool", "Lipetsk", "Kostroma", "Arzamas", "Siktivkar", "Rostov",
			"Vladivostok", "Kursk", "Kerch", "Chelyabinsk", "Kaliningrad", "Donetsk", "Kislovodsk", "Kiev", "Volgograd",
			"Dubai", "Irkutsk", "Krasnodar", "Ryazan", "Nirsk", "Krasnoyarsk"
		};

		const size_t MAX_CITIES = size;
		const size_t MAX_EDGES = size * 3;

		while (_size != MAX_CITIES) {
			addVertex(names[rand() % 24], rand());
		}

		if (_vertexes) {
			for (auto i = 0; i < MAX_EDGES; ++i) {

				auto index1 = rand() % _size;
				auto index2 = rand() % _size;
				if (!_vertexes[index1] || !_vertexes[index2])
					continue;
				addEdge(_vertexes[index1]->_city->getName(), _vertexes[index2]->_city->getName(), rand());
			}
		}
	}

	void addVertex(const char* name, size_t population) {

		if (isSettlementExists(name)) {
			cout << "Settlement already exist" << endl;
			return;
		}

		Settlement* city = new Settlement(name, population);
		Vertex** _tmp = new Vertex * [_size + 1];


		// Copy data
		for (auto i = 0; i < _size; ++i) {
			_tmp[i] = _vertexes[i];
		}
		_tmp[_size] = new Vertex(city, 0);

		if (_vertexes)
			delete[] _vertexes;

		_vertexes = _tmp;

		++_size;
	}

	void addEdge(const char* src, const char* dst, int distance) {

		if (strcmp(src, dst) == 0)
			return;

		if (distance <= 0) {
			cout << "Wrong distance" << endl;
			return;
		}

		// If src or dst do not exists
		if (!isSettlementExists(src) || !isSettlementExists(dst)) {
			cout << "Settlement doesn't exist" << endl;
			return;
		}

		if (hasBond(src, dst))
			return;

		auto indexOfSrc = positionInMain(src);
		auto indexOfDst = positionInMain(dst);

		_vertexes[indexOfSrc]->_next = new Vertex(_vertexes[indexOfDst]->_city, distance, _vertexes[indexOfSrc]->_next);

	}

	void eraseEdge(const char* src, const char* dst) {

		// If src or dst do not exists
		if (!isSettlementExists(src) || !isSettlementExists(dst)) {
			cout << "Settlement doesn't exist" << endl;
			return;
		}

		auto indexOfSrc = positionInMain(src);
		auto indexOfDst = positionInMain(dst);

		auto tmp = _vertexes[indexOfSrc];

		while (tmp) {
			if (tmp->_next && strcmp(tmp->_next->_city->getName(), dst) == 0)
			{
				auto nextFromRemove = tmp->_next->_next;
				delete tmp->_next;
				tmp->_next = nextFromRemove;
				return;
			}
			tmp = tmp->_next;
		}

		if (tmp == nullptr) {
			cout << "Edge is absent" << endl;
			return;
		}

		return;

	}

	void eraseVertex(const char* name) {

		if (!isSettlementExists(name)) {
			cout << "Settlement does not exists" << endl;
			return;
		}

		auto index = positionInMain(name);

		// Remove all external edges
		for (auto i = 0; i < _size; ++i)
		{
			if (i != index && hasBond(_vertexes[i]->_city->getName(), name))
				eraseEdge(_vertexes[i]->_city->getName(), name);
		}

		// Remove all internal edges
		auto tmp = _vertexes[index];
		while (tmp) {
			_vertexes[index] = _vertexes[index]->_next;
			delete tmp;
			tmp = _vertexes[index];
		}

		// Decrease array of main vertexes
		auto copy = new Vertex * [_size - 1];
		for (auto i = 0; i < index; ++i)
			copy[i] = _vertexes[i];
		for (auto i = index + 1; i < _size; ++i) {
			copy[i - 1] = _vertexes[i];
		}

		delete[] _vertexes;

		_vertexes = copy;

		--_size;

	}

	void editEdge(const char* src, const char* dst, size_t newDistance) {
		if (!hasBond(src, dst))
			return;

		auto index = positionInMain(src);

		auto tmp = _vertexes[index];
		while (tmp) {
			if (strcmp(tmp->_city->getName(), dst) == 0) {
				tmp->_distance = static_cast<int>(newDistance);
				return;
			}
			tmp = tmp->_next;
		}
	}

	void editVertex(const char* src, const char* newName, size_t newPopulation) {

		if (!isSettlementExists(src)) {
			cout << "Settlement doesn't exist" << endl;
			return;
		}

		if (isSettlementExists(newName)) {
			cout << "Can't rename: name is already exist" << endl;
			return;
		}

		editVertexPopulation(src, newPopulation);
		editVertexName(src, newName);

	}

	void editVertexPopulation(const char* src, size_t newPopulation) {

		if (!isSettlementExists(src)) {
			cout << "Settlement doesn't exist" << endl;
			return;
		}

		auto index = positionInMain(src);

		_vertexes[index]->_city->setPopulation(static_cast<int>(newPopulation));

		return;
	}

	void editVertexName(const char* src, const char* newName) {

		if (!isSettlementExists(src)) {
			cout << "Settlement doesn't exist" << endl;
			return;
		}

		if (isSettlementExists(newName)) {
			cout << "Can't rename: name is already exist" << endl;
			return;
		}

		auto index = positionInMain(src);

		_vertexes[index]->_city->setName(newName);

		return;
	}

	void print() {
		for (auto i = 0; i < _size; ++i) {
			cout << _vertexes[i]->_city->getName() << " (" << _vertexes[i]->_city->getPopulation() << ")" << endl;

			auto tmp = _vertexes[i]->_next;
			while (tmp) {
				cout << " - " << tmp->_city->getName() << " (" << tmp->_city->getPopulation() << ") - distance: " << tmp->_distance << endl;

				tmp = tmp->_next;
			}
		}
	}

	

	void byPassInWidth(const char* city) {

		Queue<const char*> cityToProcess;

		Cities processedCities;

		cityToProcess.push(city);
		processedCities.pushCity(city);

		while (!cityToProcess.empty()) {

			//  Processing of current vertex

			auto index = positionInMain(cityToProcess.top());
			cout << cityToProcess.top() << endl;
			cityToProcess.pop();

			Vertex* tmp = _vertexes[index]->_next;
			while (tmp)
			{
				if (!processedCities.isCityHere(tmp->_city->getName())) {
					cityToProcess.push(tmp->_city->getName());

					processedCities.pushCity(tmp->_city->getName());
				}
				tmp = tmp->_next;
			}


		}

	}

	void byPassInDepth(const char* city) {

		Stack<const char*> cityToProcess;

		Cities processedCities;

		cityToProcess.push(city);
		processedCities.pushCity(city);

		while (!cityToProcess.empty()) {

			// Processing of current vertex

			auto index = positionInMain(cityToProcess.top());
			cout << cityToProcess.top() << endl;
			cityToProcess.pop();

			Vertex* tmp = _vertexes[index]->_next;
			while (tmp)
			{
				if (!processedCities.isCityHere(tmp->_city->getName())) {
					cityToProcess.push(tmp->_city->getName());

					processedCities.pushCity(tmp->_city->getName());
				}
				tmp = tmp->_next;
			}


		}
	}

	void byPassInDepthRec(const char* city) {

		Cities processedCities;
		processedCities.pushCity(city);

		Stack<const char*> result;

		cout << city << endl;

		helpRecBypassDepth(_vertexes[positionInMain(city)], result, processedCities);
	}


	void methodDijkstra(const char* start, const char* end) {

		const int infinity = INT_MAX;

		// Array of distances
		int* arrDistances = new int[_size];

		// Array of vertexes states
		bool* arrVisited = new bool[_size];

		for (auto i = 0; i < _size; ++i)
		{
			arrDistances[i] = infinity;
			arrVisited[i] = 0;
		}
		int currentPosition = positionInMain(start);

		arrDistances[currentPosition] = 0;

		auto minDistanceIndex = currentPosition;


		// While we not visited all cities
		while (!allCityVisited(arrVisited, _size)) {

			auto minDistance = INT_MAX;

			// Choose unchecked city from all list of vertexes with minimal mark(distance from "start" to current)
			for (auto i = 0; i < _size; ++i)
				if (!arrVisited[i] && arrDistances[i] < minDistance) {
					minDistanceIndex = i;
					minDistance = arrDistances[i];
				}

			auto tmp = _vertexes[minDistanceIndex];

			while (tmp) {

				// Relaxation
				auto childIndex = positionInMain(tmp->_city->getName());
				if (arrDistances[minDistanceIndex] + tmp->_distance < arrDistances[childIndex])
					arrDistances[childIndex] = arrDistances[minDistanceIndex] + tmp->_distance;

				tmp = tmp->_next;
			}

			arrVisited[minDistanceIndex] = true;
		}

		// Debug print
		for (auto i = 0; i < _size; ++i)
			cout << "Minimal distance between " << start << " and " << _vertexes[i]->_city->getName() << " is " << arrDistances[i] << endl;

		// Search minimal way
		struct node {
			const char* _city;
			node* _next;

			node(const char* city, node* next = nullptr) : _city(city), _next(next) {}
		};

		// In this list we will save way from "start" to "end"
		node* way = new node(end);

		const char* currentLastCity = end;

		while (strcmp(currentLastCity, start) != 0) {

			// Looking for bounds are needed
			for (auto i = 0; i < _size; ++i) {
				if (hasBond(_vertexes[i]->_city->getName(), currentLastCity)) {
					auto tmp = findVertexInEdges(currentLastCity, i);

					// If it is right way
					if (arrDistances[i] + tmp->_distance == arrDistances[positionInMain(currentLastCity)]) {
						way = new node(_vertexes[i]->_city->getName(), way);
						currentLastCity = _vertexes[i]->_city->getName();
						break;
					}
				}
			}
		}

		cout << "Way: " << endl;
		auto tmp = way;
		while (tmp) {
			cout << tmp->_city;
			if (tmp->_next)
				cout << " -> ";
			tmp = tmp->_next;
		}
		cout << endl;

		delete[] arrVisited;
		delete[] arrDistances;
	}

	
	void methodBellmanFord(const char* start, const char* end) {

		const int infinity = INT_MAX;

		// Array of distances
		int* arrDistances = new int[_size];

		for (auto i = 0; i < _size; ++i)
		{
			arrDistances[i] = infinity;
		}

		// Special flag - all edges are relaxes
		bool allRelaxed = false;

		arrDistances[positionInMain(start)] = 0;

		while (!allRelaxed) {

			allRelaxed = true;

			for (auto i = 0; i < _size; ++i) {
				auto tmp = _vertexes[i]->_next;
				while (tmp) {
					if (arrDistances[i] != infinity && arrDistances[i] + tmp->_distance < arrDistances[positionInMain(tmp->_city->getName())]) {
						arrDistances[positionInMain(tmp->_city->getName())] = arrDistances[i] + tmp->_distance;

						allRelaxed = false;
					}

					tmp = tmp->_next;
				}
			}
		}

		// Debug print
		for (auto i = 0; i < _size; ++i)
			cout << "Minimal distance between " << start << " and " << _vertexes[i]->_city->getName() << " is " << arrDistances[i] << endl;

		// Search minimal way
		struct node {
			const char* _city;
			node* _next;

			node(const char* city, node* next = nullptr) : _city(city), _next(next) {}
		};

		// In this list we will save way from "start" to "end"
		node* way = new node(end);

		const char* currentLastCity = end;

		while (strcmp(currentLastCity, start) != 0) {

			//  Looking for bounds are needed
			for (auto i = 0; i < _size; ++i) {
				if (hasBond(_vertexes[i]->_city->getName(), currentLastCity)) 
				{
					auto tmp = findVertexInEdges(currentLastCity, i); // If we have bond fucnction never return nullptr

					// If it is right way
					if (arrDistances[i] + tmp->_distance == arrDistances[positionInMain(currentLastCity)]) {
						way = new node(_vertexes[i]->_city->getName(), way);
						currentLastCity = _vertexes[i]->_city->getName();
						break;
					}
				}
			}
		}

		cout << "Way: " << endl;
		auto tmp = way;
		while (tmp) {
			cout << tmp->_city;
			if (tmp->_next)
				cout << " -> ";
			tmp = tmp->_next;
		}
		cout << endl;

		delete[] arrDistances;
	}















};








int main() {

	


	return 0;
}