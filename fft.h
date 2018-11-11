#include <complex>
#include <cmath>
#include <cstring>
#include <stdexcept>

#ifndef FFT_H
#define FFT_H

/**
 * @brief Тип преобразования
 */
enum TYPE_TRANSFORM
{
    NORMAL, ///< Преобразование виде $\left(\frac{2\pi kn}{N}\right)$
    SIN_HALF_FREQ, ///< Преобразование виде $\left(\frac{\pi kn}{N}\right)$, функция продлеввается нечетно
    COS_HALF_FREQ ///< Преобразование виде $\left(\frac{\pi kn}{N}\right)$, функция продлеввается четно
};

/**
 * @class FFT
 * @brief Класс, реализующий метод Fast Fourier Transform (FFT).
 */
class FFT
{
    typedef std::complex<double> complexD; ///< Определяем новый тип для удобства

    complexD* data; ///< Хранит начальные данные, или после преобразования хранит выходной спектр
    uint count;
    uint order; ///< Хранит логарифм от количество элементов
    TYPE_TRANSFORM type_transform; ///< Вычисление FFT для половинной фазы
    const double pi = 3.14159265358979323846; ///< Константа $\pi$

    /**
     * @brief Функция предназначенная для инвертирования порядка бит в числе
     * 
     * @param[in] x Число, которое нужно инвертировать
     * @return uint Число, с инвертированным порядком бит
     */
    const uint reverseBit(uint x)
    {
        uint base = 1 << order;
        uint res = 0;
        while (x != 0) 
        {
            res += (x & 1) * (base >>= 1);
            x >>= 1;
        }
        return res;
    }

    /**
     * @brief Вычисляет коэффициент поворота $W_{k}^n$
     * 
     * @param[in] k Коэффициент $k$
     * @param[in] n Коэффициент $n$
     * @return complexD Коэффициент поворота
     */
    const complexD rotate(double k, double n)
    {
        return exp(-complexD(0., 1.)*2.*pi*k/n);
    }

    /**
     * @brief Производит прямое или обратное FFT
     * 
     * @param[in] type Указывает тип преобразование (1 - прямое, -1 - обратное)
     */
    void transform(short type)
    {
        uint half, intervals;        
        complexD less, more;

        for (uint i = 0; i < count; i++)
        {
            uint revBit = reverseBit(i);
            if (revBit >= i)
                swap(data[i], data[revBit]);            
        }            
        
        for (uint i = 0; i < order; i++)
        {
            half = 1 << i;
            intervals = count >> (i + 1);     
            for (uint j = 0; j < intervals; j++)
                for (uint k = 0; k < half; k++)
                {                    
                    less = data[j*2*half + k];
                    more = data[j*2*half + half + k];
                    data[j*2*half + k] = less + rotate(k*type, half*2)*more;
                    data[j*2*half + half + k] = less - rotate(k*type, half*2)*more;
                }            
        }
    }

    /**
     * @brief Производит манипуляцию с данными
     * 
     * @param[in] link Тип перадачи данных: по ссылке или по значению.
     */
    void transformData(bool link)
    {
        if (!link)
        {
            complexD* _data;

            if (type_transform == TYPE_TRANSFORM::NORMAL)
                _data = new complexD[count];
            else
                _data = new complexD[2*count];   

            memcpy(_data, data, sizeof(complexD)*count);

            data = _data;
        }

        if (type_transform == TYPE_TRANSFORM::SIN_HALF_FREQ)
        {
            for(int i = 0; i < count; i++)                           
                data[2*count - i - 1] = -1.*data[i + 1];       
            data[count] = 0.;
        }

        //TODO, CHECK FOR CORRECT
        if (type_transform == TYPE_TRANSFORM::COS_HALF_FREQ)
        {
            for(int i = 0; i < count; i++)                           
                data[2*count - i - 1] = data[i + 1];
            data[count] = 0.5*(data[count + 1] + data[count - 1]);
        }
    }    

public:
    /**
     * @brief Конструктор класса FFT
     * 
     * Данная реализация не задействует собственной памяти, а изменяет данные _data, количество элементов в массиве _data должно
     * быть пропорционально $2^N$. 
     * 
     * @param[in] _data Данные, над которыми нужно выполнить FFT
     * @param[in] _count Количество элементов в массиве _data
     * @param[in] link Передача данных по ссылке. Требует соответствия количеству элементов массива выражения $2^{order}$.
     * Данная реализация не задействует собственной памяти, а изменяет данные _data. Если при этом _type_transform не равняется NORMAL, 
     * необходимо чтобы размер массива _data был $2^{order+1}$, причём первая половина будет продлена на вторую в зависимости от 
     * _type_transform четным или нечетным образом
     * 
     * @param[in] _type_transform Тип преобразования
     */
    FFT(complexD* _data, uint _count, bool link = false, TYPE_TRANSFORM _type_transform = TYPE_TRANSFORM::NORMAL)
    {
        count = _count;
        order = std::log2(count - 1) + 1; 
        type_transform = _type_transform;

        data = _data;

        transformData(link);
    }

    /**
     * @brief Производит прямое FFT
     */
    void directTransformation()
    {
        if (type_transform != TYPE_TRANSFORM::NORMAL)
        {
            count *= 2;
            order++;
        }

        transform(1);

        if (type_transform != TYPE_TRANSFORM::NORMAL)
        {
            count /= 2;
            order--;
        }
    }

    /**
     * @brief Производит обратное FFT
     */
    void inverseTransformation()
    {
        if (type_transform != TYPE_TRANSFORM::NORMAL)
        {
            count *= 2;
            order++;
        }

        transform(-1);

        if (type_transform != TYPE_TRANSFORM::NORMAL)
        {
            count /= 2;
            order--;
        }

        for (uint i = 0; i < count; i++)
            data[i] /= 2.*(double)count;               
    }

    /**
     * @brief Возвращает из массива data элемент с номером index
     * 
     * @param[in] index Индекс в массиве
     * @return complexD& Ссылка на элемент массив
     */
    complexD& operator[](uint index)
    {
        if (index > count)
            throw std::out_of_range("Index out of range.");
        return data[index];
    }


    /**
     * @brief Возвращает размер массива data
     * 
     * @return uint Размер массива
     */
    const uint size()
    {
        return count;
    }
};

#endif