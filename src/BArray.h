#ifndef NANDARRAY
#define NANDARRAY
#include "Libraries.h"
//test edit
//A heap allocated custom vector class

template <typename T>
class BArray
{
public:

    size_t size;
    size_t capacity;
	T * Data;

	BArray();
	~BArray();

	void ReAlloc(size_t newCapacity);

	const T& operator[](size_t index) const;

	T& operator[](size_t index);


	void Push(const T& n);

	void Remove(size_t index);

	void ClearData();

};

template <typename T>
BArray<T>::BArray()
{
	capacity = 1;
	Data = new T[capacity];
	size = 0;
	std::cout << "BArray constructed with capacity " << capacity << "\n";
}

template <typename T>
BArray<T>::~BArray()
{
	delete[] Data;
}

template <typename T>
void BArray<T>::ReAlloc(size_t newCapacity)
{
	if (size > 0)
	{
		T* newBlock = new T[newCapacity];
		for (size_t i = 0; i < size; i++)
		{
			newBlock[i] = Data[i];
		}
		delete[] Data;
		Data = newBlock;
	}
	capacity = newCapacity;
	//std::cout << "Reallocated memory:" << capacity << "\n";
}

template <typename T>
const T& BArray<T>::operator[](size_t index) const
{
	return Data[index];
}

template <typename T>
T& BArray<T>::operator[](size_t index)
{
	return Data[index];
}

template <typename T>
void BArray<T>::Push(const T& n)
{
	if (size >= capacity - 1)
	{
		ReAlloc(capacity + capacity);
	}
	Data[size] = n;
	size++;
	//std::cout << "Pushed an item\n";
}

template <typename T>
void BArray<T>::Remove(size_t index)
{
	if (index >= size)
	{
		std::cout << "Too high number wtf bruv \n";
		throw;
	}
	for (int i = index; i < size; i++)
	{
		Data[i] = Data[i++];
	}
	size--;
	int32_t HalfCap = capacity / 2;
	if (size < HalfCap)
	{
		ReAlloc(HalfCap);
	}
}

template <typename T>
void BArray<T>::ClearData()
{
	delete[] Data;
	capacity = 1;
	Data = new T[capacity];
	size = 0;
}

#endif