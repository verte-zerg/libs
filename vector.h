#include <stdexcept>
#include <cmath>

#ifndef VECTOR_H
#define VECTOR_H

/**
 * @class Vector
 * @brief Класс, реализующий статический массив числовых данных
 * 
 * @tparam T Числовой тип хранимых данных
 */
template <typename T>
class Vector
{
	T* data; ///< Массив хранимых данных	
	int size; ///< Количество хранимых данных
	
public:
	/**
	 * @brief Конструктор по умолчанию класса Vector 
	 */
	Vector() {}

	/**
	 * @brief Конструктор класса Vector
	 * 
	 * @param _size Количество элементов в массиве
	 */
	Vector(unsigned int _size): size(_size)
	{
		data = new T[size] {0};	
	}

	/**
	 * @brief Деструктор класса Vector
	 */
	~Vector()
	{
		//delete[] data;
		data = NULL;
	}

	/**
	 * @brief Констуктор копирования
	 * 
	 * @param copy_v Вектор, который нужно клонированить 
	 */
	Vector(const Vector<T>& copy_v)
	{
		size = copy_v.size;
		data = new T[size] {0};
		memcpy(data, copy_v, sizeof(T)*size);		
	}

	/**
	 * @brief Оператор доступа к элементу массива
	 * 
	 * @param index Номер нужного элемента
	 * @return T& Ссылка на элемент массива
	 */
	T& operator[](unsigned int index)
	{
		if (index >= size)
			throw std::out_of_range("Index out of range");
		return data[index];
	}

	/**
	 * @brief Вычисление нормы по Эйлеру
	 * 
	 * @return double Норма вектора
	 */
	double Abs()
	{
		double sum = .0;
		for (int i = 0; i < size; i++)
			sum += data[i]*data[i];
		return std::sqrt(sum);
	}

	/**
	 * @brief Оператор умножения
	 * 
	 * @param other Элемент на который нужно умножить
	 * @return Vector& Вектор, умноженный на элемент other
	 */
	Vector& operator*(const T& other)
	{
		Vector* tmp = new Vector(size);
		for (int i = 0; i < size; i++)
			(*tmp).data[i] = data[i] * other;
		return *tmp;
	}

	/**
	 * @brief Оператор умножения себя
	 * 
	 * @param other Элемент на который нужно умножить
	 * @return Vector& Ссылку на свой, 
	 */
	Vector& operator*=(const T& other)
	{		
		for (int i = 0; i < size; i++)
			data[i] = data[i] * other;
		return *this;
	}

	/**
	 * @brief Оператор сложения
	 * 
	 * @param other Вектор с которым нужно сложить
	 * @return Vector& Вектор, полученный в результате сложения текущего и переданного
	 */
	Vector& operator+(const Vector& other)
	{
		if (size != other.size)
			throw std::invalid_argument("Size vectors not equal.");
		Vector* tmp = new Vector(size);
		for (int i = 0; i < size; i++)
			(*tmp).data[i] = data[i] + other.data[i];
		return *tmp;
	}

	/**
	 * @brief Оператор вычитания
	 * 
	 * @param other Вектор, которым нужно вычесть
	 * @return Vector& Вектор, полученный в результате вычитания из текущего переданного
	 */
	Vector& operator-(const Vector& other)
	{
		if (size != other.size)
			throw std::invalid_argument("Size vectors not equal.");
		Vector* tmp = new Vector(size);
		for (int i = 0; i < size; i++)
			(*tmp).data[i] = data[i] - other.data[i];
		return *tmp;
	}

	/**
	 * @brief Получение размера массива
	 * 
	 * @return int Размер массива
	 */
	int GetSize()
	{
		return size;
	}

	/**
	 * @brief Получение ссылки на динамический массив
	 * 
	 * @return T* Ссылка на массив
	 */
	T* getArray()
	{
		return data;
	}

	/**
	 * @brief Задание новой ссылка на массив
	 * 
	 * @param arr Указатель на массив
	 */
	void setNewArray(T* arr)
	{
		delete[] data;
		data = arr;
	}
};

#endif