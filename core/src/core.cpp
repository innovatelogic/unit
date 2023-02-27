
#include <iostream>
#include <Eigen/Dense>
#include <pybind11/pybind11.h>
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xview.hpp"
#include "core.h"

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

void UCore::test_func_xtensor(int a, int b)
{
    xt::xarray<double> arr1
    {{1.0, 2.0, 3.0},
    {2.0, 5.0, 7.0},
    {2.0, 5.0, 7.0}};

    xt::xarray<double> arr2
    {5.0, 6.0, 7.0};

    xt::xarray<double> res = xt::view(arr1, 1) + arr2;

    std::cout << "Test call xtensor CPP :\n " << res << std::endl;
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
    m.def("test_func_xtensor", &core::UCore::test_func_xtensor, "cpp xtensor test call");
}