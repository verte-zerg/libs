#include <vector>
#include <stdexcept>

#ifndef TRIDIAGONAL_H
#define TRIDIAGONAL_H

template<class T>
class Tridiagonal
{
    T* alpha; ///< Прогоночный коэффициент $\alpha$
    T* beta; ///< Прогоночный коэффициент $\beta$
    T* x; ///< Столбец - результат
    uint size; ///< Размерность матрицы
    uint trueSize; ///< Истинная размерность матрицы

public:
    /**
     * @brief Конструктор класса Tridiagonal
     *
     * @param _size Размерность уравнения
     */
    Tridiagonal(uint _trueSize)
    {
        trueSize = _trueSize;
        size = trueSize;

        alpha = new T[size];
        beta = new T[size];
        x = new T[size];
    }

    /**
     * @brief Установка нового значения размерности матрицы
     * 
     * @param _size Новая размерность матрицы
     */
    void setSize(uint _size)
    {
        if (trueSize < _size)
            throw std::invalid_argument("Size cannot be larger than true size (class Tridiagonal).");
        size = _size;
    }

    /**
     * @brief Получения установленного значения размерности матрицы
     * 
     * @return uint Текущее значения размерности матрицы
     */
    uint getSize() { return size; }

    /**
     * @brief Получения истинного значения размерности матрицы
     * 
     * @return uint Истинное значения размерности матрицы
     */
    uint getTrueSize() { return trueSize; }

    /**
     * @brief Получение элемента результирующего столбца
     *
     * @param index Индекс элемента
     * @return T& Ссылка на элемент
     */
    T& operator[](uint index)
    {
        if (index >= size)
            throw std::out_of_range("Index out of range in operator[] (class SLETridiagonal).");
        return x[index];
    }

    /**
     * @brief Получение элемента результирующего столбца
     *
     * @param index Индекс элемента
     * @return T& Ссылка на элемент
     */
    T& operator[](uint index) const
    {
        if (index >= size)
            throw std::out_of_range("Index out of range in operator[] (class SLETridiagonal).");
        return x[index];
    }

    /**
     * @brief Решает СЛАУ
     *
     * @param a Нижняя диагональ
     * @param b Верхняя диагональ
     * @param c Главная диагональ
     * @param f Столбец свободных членов
     */
    void solve(T* a, T* b, T* c, T* f)
    {
        alpha[0] = -b[0]/c[0];
        beta[0] = f[0]/c[0];

        for (size_t i = 1; i < size - 1; i++)
        {
            alpha[i] = -b[i]/(a[i-1]*alpha[i-1] + c[i]);
            beta[i] = (f[i] - a[i-1]*beta[i-1])/(a[i-1]*alpha[i-1] + c[i]);
        }

        x[size - 1] = (f[size - 1] - a[size - 2]*beta[size - 2])/(a[size - 2]*alpha[size - 2] + c[size - 1]);

        for (size_t i = size - 1; i > 0; i--)
            x[i-1] = alpha[i-1]*x[i] + beta[i-1];
    }

    /**
     * @brief Возвращает результирующий столбец в виде вектора
     *
     * @return std::vector<T> Результирующий столбец
     */
    std::vector<T> getResultVector()
    {
        std::vector<T> v;
        for (size_t i = 0; i < size; i++)
            v.push_back(x[i]);
        return v;
    }
};

#endif
