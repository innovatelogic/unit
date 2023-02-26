#include "core.h"
#include <iostream>
#include <Eigen/Dense>
#include <pybind11/pybind11.h>

namespace core
{

void UCore::test_func_eigen(int a, int b)
{
    using Eigen::MatrixXd;

    MatrixXd m(a, b);

    if (a >= 2 && b >= 2)
    {
        m(0, 0) = 3;
        m(1, 0) = 2.5;
        m(0, 1) = -1;
        m(1, 1) = m(1, 0) + m(0, 1);
    }
    std::cout << "Test call eigen CPP :\n " << m << std::endl;
}

}

void test_func_cpp(int a, int b)
{
    std::cout << "Test call CPP : a + b = " << a + b << std::endl;
}

void test_func_c(int a, int b)
{
    std::cout << "Test call C : a + b = " << a + b << std::endl;
}

PYBIND11_MODULE(unit_core, m) 
{
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("test_func_c", &test_func_c, "C test call");
    m.def("test_func_cpp", &test_func_cpp, "Cpp test call");
    m.def("test_func_eigen", &core::UCore::test_func_eigen, "cpp eigen test call");
}