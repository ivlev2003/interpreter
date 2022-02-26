#include "matrix.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace matrix_interpreter {
struct read_error : std::runtime_error {
    explicit read_error(const std::string &filename)
        : std::runtime_error("Unable to open file '" + filename + "'") {
    }
};

struct file_format_error : std::runtime_error {
    file_format_error() : std::runtime_error("Invalid file format") {
    }
};

struct out_of_bounds_error : std::runtime_error {
    out_of_bounds_error()
        : std::runtime_error("Requested element is out of bounds") {
    }
};

struct size_error : std::runtime_error {
    explicit size_error(const std::string &what) : std::runtime_error(what) {
    }
};

matrix_interpreter::matrix load(const std::string &path) {
    std::ifstream instr(path);
    if (!instr.is_open()) {
        throw read_error(path);
    }
    int h = 0;
    int w = 0;

    if (!(instr >> h >> w)) {
        throw file_format_error();
    }
    if (h == 0 || w == 0) {
        h = 0;
        w = 0;
    }
    matrix temp(h, w);
    int inserted_element = 0;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (!(instr >> inserted_element)) {
                throw file_format_error();
            } else {
                temp.set_elem(i, j, inserted_element);
            }
        }
    }
    instr.close();
    return temp;
}

void print(const matrix &item) {
    int h = item.get_height();
    int w = item.get_width();
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            std::cout << item.get_elem(i, j);
            if (j != w - 1) {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
}

long long elem(const matrix &item, int h, int w) {
    if (h < 0 || w < 0 || h >= item.get_height() || w >= item.get_width()) {
        throw out_of_bounds_error();
    }
    return item.get_elem(h, w);
}

std::string error_generator(int arg1, int arg2) {
    std::stringstream ss;
    ss << "Dimension mismatch: lhs=";
    ss << arg1;
    ss << ", rhs=";
    ss << arg2;
    return ss.str();
}

matrix operator+(const matrix &a, const matrix &b) {
    if (a.height != b.height) {
        throw size_error(error_generator(a.height, b.height));
    }
    if (a.width != b.width) {
        throw size_error(error_generator(a.width, b.width));
    }
    matrix sum(a.height, a.width);
    for (int i = 0; i < a.height; i++) {
        for (int j = 0; j < a.width; j++) {
            sum.data[i * a.width + j] = elem(a, i, j) + elem(b, i, j);
        }
    }
    return sum;
}

matrix operator*(const matrix &a, const matrix &b) {
    if (a.width != b.height) {
        throw size_error(error_generator(a.width, b.height));
    }
    matrix product(a.height, b.width);
    for (int i = 0; i < a.height; i++) {
        for (int j = 0; j < b.width; j++) {
            product.data[i * b.width + j] = 0;
            for (int z = 0; z < b.height; z++) {
                product.data[i * b.width + j] += elem(a, i, z) * elem(b, z, j);
            }
        }
    }
    return product;
}
}  // namespace matrix_interpreter