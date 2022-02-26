#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "matrix.h"

struct unknown_command_error : std::runtime_error {
    explicit unknown_command_error(const std::string &command_name)
        : std::runtime_error("Unknown command: '" + command_name + "'") {
    }
};

struct invalid_command_format_error : std::runtime_error {
    explicit invalid_command_format_error()
        : std::runtime_error("Invalid command format") {
    }
};

struct invalid_register_error : std::runtime_error {
    explicit invalid_register_error(const std::string &register_name)
        : std::runtime_error("'" + register_name + "' is not a register") {
    }
};
void check_register(const std::string &register_name) {
    if (register_name[0] != '$' || register_name[1] > '9' ||
        register_name[1] < '0') {
        throw invalid_register_error(register_name);
    }
}
bool check_elem(const std::string &value) {
    return std::all_of(value.begin(), value.end(),
                       [](char i) { return (i < '0' || i > '9'); });
}

void check_line_size(size_t need_size, size_t have_size) {
    if (need_size != have_size) {
        throw invalid_command_format_error();
    }
}

std::vector<std::string> tokenize(const std::string &input) {
    std::stringstream ss;
    ss << input;
    std::vector<std::string> tokens;
    std::string temp;
    while (ss >> temp) {
        tokens.push_back(temp);
    }

    return tokens;
}

void print(const std::string &reg,
           const std::vector<matrix_interpreter::matrix> &registers) {
    check_register(reg);
    std::string reg_format = reg.substr(1, reg.size());
    matrix_interpreter::print(registers[std::atoi(reg_format.c_str())]);
}

void load(const std::string &reg,
          const std::string &path,
          std::vector<matrix_interpreter::matrix> &registers) {
    check_register(reg);
    std::string reg_format = reg.substr(1, reg.size());
    registers[std::atoi(reg_format.c_str())] = matrix_interpreter::load(path);
}

void elem(const std::string &reg,
          const std::string &rows_num,
          const std::string &cols_num,
          const std::vector<matrix_interpreter::matrix> &registers) {
    int rows_nm = std::atoi(rows_num.c_str());
    int cols_nm = std::atoi(cols_num.c_str());

    if (check_elem(rows_num) || check_elem(cols_num) || rows_nm > 1e6 ||
        cols_nm > 1e6 || rows_nm < 0 || cols_nm < 0) {
        throw invalid_command_format_error();
    }
    check_register(reg);
    std::string reg_format = reg.substr(1, reg.size());
    long long temp_element =
        elem(registers[std::atoi(reg_format.c_str())], rows_nm, cols_nm);
    std::cout << temp_element << "\n";
}

void add(const std::string &reg1,
         const std::string &reg2,
         std::vector<matrix_interpreter::matrix> &registers) {
    check_register(reg1);
    check_register(reg2);
    std::string reg1_format = reg1.substr(1, reg1.size());
    std::string reg2_format = reg2.substr(1, reg2.size());
    registers[std::atoi(reg1_format.c_str())] =
        registers[std::atoi(reg1_format.c_str())] +
        registers[std::atoi(reg2_format.c_str())];
}

void mul(const std::string &reg1,
         const std::string &reg2,
         std::vector<matrix_interpreter::matrix> &registers) {
    check_register(reg1);
    check_register(reg2);
    std::string reg1_format = reg1.substr(1, reg1.size());
    std::string reg2_format = reg2.substr(1, reg2.size());
    registers[std::atoi(reg1_format.c_str())] =
        registers[std::atoi(reg1_format.c_str())] *
        registers[std::atoi(reg2_format.c_str())];
}

int main() {  // NOLINT(bugprone-exception-escape)
    std::string input;
    const int count_of_registers = 10;
    std::vector<matrix_interpreter::matrix> registers(
        count_of_registers, matrix_interpreter::matrix(0, 0));
    while (std::getline(std::cin, input)) {
        std::vector<std::string> tokens = tokenize(input);
        if (tokens[0] == "exit") {
            return 0;
        } else if (tokens[0] == "load") {
            try {
                check_line_size(3, tokens.size());
                std::string reg = tokens[1];
                load(reg, tokens[2], registers);
            } catch (std::exception &e) {
                std::cout << e.what() << "\n";
            }
        } else if (tokens[0] == "elem") {
            std::string reg = tokens[1];
            try {
                check_line_size(4, tokens.size());
                elem(reg, tokens[2], tokens[3], registers);
            } catch (std::exception &e) {
                std::cout << e.what() << "\n";
            }
        } else if (tokens[0] == "print") {
            try {
                check_line_size(2, tokens.size());
                std::string reg = tokens[1];
                print(reg, registers);
            } catch (std::exception &e) {
                std::cout << e.what() << "\n";
            }
        } else if (tokens[0] == "add") {
            std::string reg1 = tokens[1];
            std::string reg2 = tokens[2];
            try {
                check_line_size(3, tokens.size());
                add(reg1, reg2, registers);
            } catch (std::exception &e) {
                std::cout << e.what() << "\n";
            }
        } else if (tokens[0] == "mul") {
            std::string reg1 = tokens[1];
            std::string reg2 = tokens[2];
            try {
                check_line_size(3, tokens.size());
                mul(reg1, reg2, registers);
            } catch (std::exception &e) {
                std::cout << e.what() << "\n";
            }
        } else if (!tokens[0].empty()) {
            std::cout << unknown_command_error(tokens[0]).what() << "\n";
        }
    }
}