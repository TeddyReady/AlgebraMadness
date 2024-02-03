#ifndef MATRIX_TPP
#define MATRIX_TPP
#include "basemath.h"

template <class T>
class Matrix {
private:
    std::size_t rows, cols;
    std::vector<std::vector<T>> data;

public:
    explicit Matrix(): rows(0), cols(0) {}
    explicit Matrix(std::size_t rows, std::size_t cols): rows(rows), cols(cols)
    {
        data.resize(rows);
        for(std::size_t i = 0; i < rows; ++i)
            data[i].resize(cols);
    }
    explicit Matrix(std::size_t rows, std::size_t cols, T diagonalValue): Matrix<T>(rows, cols)
    {
        for(std::size_t i = 0; i < rows; ++i)
            for(std::size_t j = 0; j < cols; ++j)
            {
                if (i == j) data[i][j] = diagonalValue;
                else data[i][j] = static_cast<T>(0);
            }
    }
    explicit Matrix(const std::vector<std::vector<T>> &base)
        : Matrix<T>(base.size(), base[0].size())
    {
        for(std::size_t i = 0; i < rows; ++i)
            for(std::size_t j = 0; j < cols; ++j)
                data[i][j] = base[i][j];
    }
    Matrix(const Matrix<T> &other): Matrix<T>(other.rows, other.cols)
    {
        for(std::size_t i = 0; i < rows; ++i)
            for(std::size_t j = 0; j < cols; ++j)
                data[i][j] = other.data[i][j];
    }

    Matrix<T> operator- () const
    {
        Matrix<T> result(*this);

        for(std::size_t i = 0; i < result.rows; ++i)
            for(std::size_t j = 0; j < result.cols; ++j)
                result.data[i][j] = -result.data[i][j];

        return result;
    }

    Matrix<T> operator+ (const Matrix<T> &other) const
    {
        if (rows != other.rows || cols != other.cols)
        {
            std::cerr << "ERROR in matrix.tpp: Matrix dimensions does be equal!" << std::endl;
            return *this;
        }

        Matrix<T> result(rows, cols);

        for (std::size_t i = 0; i < rows; ++i) 
            for (std::size_t j = 0; j < cols; ++j)
                result.data[i][j] = data[i][j] + other.data[i][j];

        return result;
    }

    void operator+= (const Matrix<T> &other) const
    {
        *this = *this + other;
    }

    Matrix<T> operator- (const Matrix<T> &other) const
    {
        return *this + (-other);
    }

    void operator-= (const Matrix<T> &other) const
    {
        *this = *this - other;
    }

    Matrix<T> operator* (const Matrix<T> &other) const
    {
        if (cols != other.rows)
        {
            std::cerr << "ERROR in matrix.tpp: Matrix rows does be equal with cols!" << std::endl;
            return *this;    
        }

        Matrix<T> result(rows, other.cols, static_cast<T>(0));

        for(std::size_t i = 0; i < rows; ++i)
            for(std::size_t j = 0; j < other.cols; ++j)
                for(std::size_t k = 0; k < other.rows; ++k)
                    result.data[i][j] += data[i][k] * other.data[k][j];

        return result;
    }

    void operator*= (const Matrix<T> &other) const
    {
        *this = *this * other;
    }

    // Return Matrix whithout 1/det
    Matrix<T> operator~ () const
    {
        if(!isSquare() || det() == static_cast<T>(0))
        {
            std::cerr << "ERROR in matrix.tpp: Matrix does be square and det cannot be equal to zero!" << std::endl;
            return *this;
        }

        Matrix<T> result(rows, cols, static_cast<T>(0));

        for (std::size_t i = 0; i < rows; ++i)
            for (std::size_t j = 0; j < cols; ++j)
                result.data[i][j] = minor(i, j).det() * static_cast<T>(fastPower(-1, i + j));

        return result.trans();
    }

    bool operator== (const Matrix<T> &other) const
    {
        if (rows != other.rows || cols != other.cols)
            return false;

        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                if (data[i][j] != other.data[i][j])
                    return false;

        return true;
    }

    bool operator!= (const Matrix<T> &other) const
    {
        return !(*this == other);
    }

    bool isSquare() const { return (cols == rows); }

    T det() const
    {
        if (!isSquare())
        {
            std::cerr << "ERROR in matrix.tpp: Cannot calculate det of non sqyare matrix!" << std::endl;
            return static_cast<T>(0);
        }
        if (rows == 1)
            return data[0][0];
        else if (rows == 2)
            return data[0][0] * data[1][1] - data[0][1] * data[1][0];
        else {
            Matrix<T> tmp(rows - 1, rows - 1, static_cast<T>(0));
            T det = static_cast<T>(0);
            std::size_t k1, k2;
            for(size_t i = 0; i < rows; ++i) {
                k1 = 0;
                for(size_t j = 1; j < rows; ++j) {
                    k2 = 0;
                    for(size_t k = 0; k < rows; k++) {
                        if (k != i) {
                            tmp.data[k1][k2] = data[j][k];
                            ++k2;
                        }
                    }
                    ++k1;
                }
                det += static_cast<T>(fastPower(-1, i + 2)) * data[0][i] * tmp.det();
            }
            return det;
        }
    }

    Matrix<T> trans() const
    {
        Matrix<T> matrix(cols, rows);

        for(std::size_t i = 0; i < cols; ++i)
            for(std::size_t j = 0; j < rows; ++j)
                matrix.data[i][j] = data[j][i];

        return matrix;
    }

    Matrix<T> minor(std::size_t x, std::size_t y) const
    {
        if (!isSquare()) 
        {
            std::cerr << "ERROR in matrix.tpp: Cannot find minor of non square matrix!" << std::endl;
            return *this;
        }

        Matrix<T> tmp(rows - 1, rows - 1, static_cast<T>(0));
        
        tmp.data.clear();
        tmp.data.resize(data.size() - 1);
        
        size_t countIndex = -1;
        
        for (std::size_t i = 0; i < rows; ++i)
        {
            if (i == x) continue;
            else ++countIndex;
            for (std::size_t j = 0; j < rows; ++j)
            {
                if (j == y) continue;
                tmp.data[countIndex].emplace_back(data[i][j]);
            }
        }
        return tmp;
    }

    std::vector<std::vector<T>> get() const
    {
        return data;
    }

    void set(std::size_t i, std::size_t j, T value)
    {
        if (i < rows && j < cols)
            data[i][j] = value;
        else
            std::cerr << "ERROR in matrix.tpp: Cannot set value - incorrect dimension!" << std::endl;
    }
};

#endif // MATRIX_TPP
