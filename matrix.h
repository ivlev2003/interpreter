#ifndef MATRIX_H_
#define MATRIX_H_

#include <algorithm>
#include <cassert>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

namespace matrix_interpreter {

struct matrix {
private:
    int height = 0;
    int width = 0;
    std::vector<long long> data;

public:
    matrix() : data(0) {
    }

    matrix(int height_, int width_)
        : height(height_), width(width_), data(height * width) {
    }

    [[nodiscard]] int get_height() const {
        return height;
    }

    [[nodiscard]] int get_width() const {
        return width;
    }

    [[nodiscard]] long long get_elem(int h, int w) const {
        return data[h * width + w];
    }

    void set_elem(int h, int w, int value) {
        data[h * width + w] = value;
    }

    friend matrix operator+(const matrix &a, const matrix &b);
    friend matrix operator*(const matrix &a, const matrix &b);
};

matrix operator+(const matrix &a, const matrix &b);
matrix operator*(const matrix &a, const matrix &b);
matrix_interpreter::matrix load(const std::string &path);
void print(const matrix &item);
long long elem(const matrix &item, int h, int w);
}  // namespace matrix_interpreter
#endif