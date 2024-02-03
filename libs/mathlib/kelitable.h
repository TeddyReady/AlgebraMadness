#ifndef KELITABLE_H
#define KELITABLE_H
#include "matrix.tpp"

class KeliTable
{
public:
    explicit KeliTable();
    KeliTable(const std::vector<std::vector<std::string>> &data,
                       std::vector<std::string> v_header,
                       std::vector<std::string> h_header);
    KeliTable(const KeliTable &other);

    void operator= (const KeliTable &other);

    bool isAbel() const;
    void swapRows(size_t row_1, size_t row_2);
    void swapCols(size_t col_1, size_t col_2);
    int volume() const;

    operator std::string() const;
    friend std::ostream& operator<< (std::ostream& out, const KeliTable &other);
private:
    std::vector<std::vector<std::string>> data;
    std::vector<std::string> v_header, h_header;
};

#endif // KELITABLE_H
