#include <stdexcept>
#include <stdarg.h>
#include "vector.h"

#ifndef MATRIX_H
#define MATRIX_H

/**
 * @class Matrix
 * @brief Класс, реализующий статический двумерный массив числовых данных
 * 
 * @tparam T Числовой тип хранимых данных
 */
template <typename T>
class Matrix
{
	Vector<T>** points; ///< Матрица хранимых данных
	int row; ///< Количество строки
	int col; ///< Количество столбцов

public:
	/**
	 * @brief Конструктор класса Matrix
	 * 
	 * @param _row Количество строк
	 * @param _col Количество столбцов
	 */
	Matrix(int _row, int _col): row(_row), col(_col)
	{
		points = new Vector<T>*[row];
		for (int i = 0; i < row; i++)
			points[i] = new Vector<T>(col);
	}

	/**
	 * @brief Конструктор класса Matrix с инициализацией
	 * 
	 * @param _row Количество строк
	 * @param _col Количество столбцов
	 * @param x Список элементов
	 */
	Matrix(int _row, int _col, const T x...): row(_row), col(_col)
	{
		points = new Vector<T>*[row];
		va_list arg;
		va_start(arg, x);
		T tmp = x;
		for (int i = 0; i < row; i++)
		{
			points[i] = new Vector<T>(col);	
			for (int j = 0; j < col; j++)
			{				
				(*points[i])[j] = tmp;				
				tmp = va_arg(arg, T);
			}
		}
	}

	/**
	 * @brief Конструктор класса Matrix по умолчанию
	 */
	Matrix() {}

	/**
	 * @brief Деструкттор класса Matrix
	 * 
	 */
	~Matrix()
	{
		//delete[] points;
		points = NULL;
	}

	/**
	 * @brief Оператор доступа к элементу массива
	 * 
	 * @param index Номер нужного элемента
	 * @return Vector<T>& Ссылка на вектор-строку в матрице
	 */
	Vector<T>& operator[](unsigned int index)
	{
		if (index >= row)
			throw std::out_of_range("Index out of range");
		return *points[index];
	}
};

#endif