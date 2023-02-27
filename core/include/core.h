#pragma once

#include "platform.h"
#include <string>

namespace core{

class UNIT_CORE_EXPORT UCore
{
public:
	static void test_func_eigen(int a, int b);
    static void test_func_xtensor(int a, int b);
    static void test_mavlink(const std::string &str);

protected:
private:

};

}

UNIT_CORE_EXPORT void test_func_cpp(int a, int b);

extern "C"
{
    UNIT_CORE_EXPORT void test_func_c(int a, int b);
}