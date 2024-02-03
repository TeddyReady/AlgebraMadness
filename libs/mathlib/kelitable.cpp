#include "kelitable.h"

KeliTable::KeliTable() {}

KeliTable::KeliTable(const std::vector<std::vector<std::string>> &data,
                     std::vector<std::string> v_header,
                     std::vector<std::string> h_header)
    : data(data), v_header(v_header), h_header(h_header) {}

KeliTable::KeliTable(const KeliTable &other)
    : data(other.data), v_header(other.v_header), h_header(other.h_header) {}

bool KeliTable::isAbel() const
{
    Matrix<std::string> matrix(data);
    return (matrix == matrix.trans());
}

void KeliTable::operator= (const KeliTable &other)
{
    data = other.data;
    v_header = other.v_header;
    h_header = other.h_header;
}
int KeliTable::volume() const
{
    return data.size();
}

void KeliTable::swapRows(size_t row_1, size_t row_2)
{
    if (row_1 == row_2) return;
    std::swap(v_header[row_1], v_header[row_2]);
    std::swap(data[row_1], data[row_2]);
}

void KeliTable::swapCols(size_t col_1, size_t col_2)
{
    if (col_1 == col_2) return;
    std::swap(h_header[col_1], h_header[col_2]);
    for (size_t i = 0; i < data.size(); ++i)
        std::swap(data[i][col_1], data[i][col_2]);
}

KeliTable::operator std::string() const
{
    size_t volume = data.size() + 1;
    std::string result;
    for (std::size_t i = 0; i < volume; ++i)
    {
        for (std::size_t j = 0; j < volume; ++j)
        {
            if (i == 0 && j == 0)
                result += "";
            else if (i == 0)
                result += h_header[j-1];
            else if (j == 0)
                result += v_header[i-1];
            else
                result += data.at(i - 1).at(j - 1);

            if (j + 1 != volume) result += "&";
            else result += "\\\\\\hline ";
        }
    }

    return "\\begin{array}{|r|r|}\\hline" + result + "\\end{array}";
}

std::ostream& operator<< (std::ostream& out, const KeliTable &other)
{
    return out << std::string(other);
}
