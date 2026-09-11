#ifndef IOUTILS_HPP
#define IOUTILS_HPP

#include "TridiagonalSolver.hpp"

#include <vector>
#include <fstream>

class IOUtils final 
{
public:
    static TriSystem read_slae(std::ifstream& file);

    static void print_vector(const std::vector<double>& v);
    static void print_result_table(const TriResult& result);
};




#endif // IOUTILS_HPP
