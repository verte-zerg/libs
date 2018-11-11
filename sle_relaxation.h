#ifndef SLE_RELAXATION_H
#define SLE_RELAXATION_H

#include <stdexcept>
#include <vector>

namespace sle 
{
    /**
     * @сlass SLERelaxation
     * @brief Решатель СЛАУ методом релаксации
     * Условие сходимости: диагональный элемент должен быть больше сумму элементов всей строки
     * @tparam T Тип элементов СЛАУ
     */
    template <class T>
    class SLERelaxation
    {
        T** data; ///< Основная матрица
        T* res; ///< Столбец - результат
        T* free; ///< Столбец свободны членов
        double omega; ///< Параметр метода релаксации
        uint size; ///< Размерность уравнения
        uint steps; ///< Количество проведенных итераций

    public:
        /**
         * @brief Конструктор класса SLERelaxation
         * 
         * @param _data Основная матрица
         * @param _free Столбец свободны членов
         * @param _size Размерность уравнения
         * @param _omega Параметр метода релаксации
         */
        SLERelaxation(T** _data, T* _free, uint _size, double _omega = 1.): omega(_omega)
        {
            size = _size;

            data = new T*[size];
            free = new T[size];

            for (uint i = 0; i < size; i++)
            {
                data[i] = new T[size];
                free[i] = _free[i];
            }

            for (uint i = 0; i < size; i++)
                for (uint j = 0; j < size; j++)
                    data[i][j] = _data[i][j];
        }

        /**
         * @brief Получение элемента результирующего столбца 
         * 
         * @param i Индекс элемента
         * @return T& Ссылка на элемент
         */
        T& operator[](uint i)
        {
            if (i >= size)
                throw std::out_of_range("Index out of range in array 'res' (class 'SLAURelaxation').");
            return res[i];
        }

        /**
         * @brief Получение элемента результирующего столбца 
         * 
         * @param i Индекс элемента
         * @return T& Ссылка на элемент
         */
        T& operator[](uint i) const
        {
            if (i >= size)
                throw std::out_of_range("Index out of range in array 'res' (class 'SLAURelaxation').");
            return res[i];
        }

        /**
         * @brief Решает СЛАУ
         * 
         * @param eps Допустимая погрешность
         * @param initGuess Начальное приближение
         */
        void solve(double eps, T* initGuess)
        {
            steps = 0;
            res = new T[size];

            for (uint i = 0; i < size; i++)
                res[i] = initGuess[i];

            double newValue, sum, sub, maxErr;
            do
            {
                steps++;
                maxErr = 0.;
                for (uint i = 0; i < size; i++)
                {
                    sum = 0.;                

                    for (uint j = 0; j < size; j++)
                        if (j != i)
                            sum += data[i][j]*res[j];
                        else
                            continue;

                    newValue = (1. - omega)*res[i] + omega/data[i][i]*(free[i] - sum);
                    sub = std::abs(newValue - res[i]);
                    res[i] = newValue;

                    if (sub > maxErr)
                        maxErr = sub;                
                }
            } 
            while (maxErr > eps);
        }

        /**
         * @brief Возвращает результирующий столбец в виде вектора
         * 
         * @return std::vector<T> Результирующий столбец
         */
        std::vector<T> getResultVector()
        {
            std::vector<T> v;
            for (uint i = 0; i < size; i++)
                v.push_back(res[i]);
            return v;
        }

        /**
         * @brief Возвращает количество проведенных итераций
         * 
         * @return uint Количество итераций
         */
        uint getSteps()
        {
            return steps;
        }
    };
}

#endif