#ifndef IOUTILS_HPP
#define IOUTILS_HPP

#include "Matrix.hpp"

#include <vector>
#include <fstream>

struct SystemInput
{
    Matrix A;
    std::vector<double> b;
};

class IOUtils final
{
public:
    static SystemInput read_slae(std::ifstream& file);
    static Matrix read_matrix(std::ifstream& file);
    static void print_matrix(const Matrix& m);
    static void print_vector(const std::vector<double>& v);

    static void print_permutation_vector(const std::vector<size_t>& P_vec);
};



#endif //IOUTILS_HPP