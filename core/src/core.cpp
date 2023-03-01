
#include <iostream>
#include <chrono>
#include <cstdint>
#include <future>
#include <memory>
#include <thread>
#include <Eigen/Dense>
#include <pybind11/pybind11.h>
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xview.hpp"

#if __linux__
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/mavlink_passthrough/mavlink_passthrough.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

#include <mavlink/v2.0/minimal/mavlink.h>
#endif //__linux__

#include "core.h"

#define BUFFER_LENGTH 2041 // minimum buffer size that can be used with qnx

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

void UCore::test_mavlink(const std::string &url)
{
#if __linux__ 

    using namespace mavsdk;
    using std::chrono::seconds;

    std::cout << "Test call MAVSDK CPP : " << url << std::endl;

    mavsdk::Mavsdk mavsdk;
    // Create a MAVLink connection

    std::cout << "add connection : " << url << std::endl;
    ConnectionResult connection_result = mavsdk.add_any_connection(url.c_str()); 

    // check if we have a connection   
    if (connection_result != ConnectionResult::Success) 
    {
        std::cout << "Connection failed: " << connection_result << std::endl;
        return;
    }
    else{
        std::cout << "MAVLink connected" << std::endl; 
    }

    // get a system from the connection
    auto new_system_promise = std::promise<std::shared_ptr<System>>{};
    auto new_system_future = new_system_promise.get_future();

    mavsdk.subscribe_on_new_system([&mavsdk, &new_system_promise]() {
        new_system_promise.set_value(mavsdk.systems().at(0));
        mavsdk.subscribe_on_new_system(nullptr);
    });

    auto system = new_system_future.get();

    //auto system = get_system_id(mavsdk);
    if (!system)
    {
        std::cout << "Could not obtain a system from the MAVLink connection" << std::endl;  
    }
    else{
        std::cout << "System was obtained from the MAVLink connection" << std::endl;
    }
#else
    std::cout << "Test call MAVSDK(empty) CPP : " << url << std::endl;
#endif
}

void UCore::test_mavlink_v2(const std::string &str)
{
#if __linux__ 
    std::cout << "Test call MAVLINK CPP : " << str << std::endl;

    uint8_t buf[BUFFER_LENGTH];
    mavlink_message_t msg;

    mavlink_msg_heartbeat_pack(1, 200, &msg, MAV_TYPE_HELICOPTER, MAV_AUTOPILOT_GENERIC, MAV_MODE_GUIDED_ARMED, 0, MAV_STATE_ACTIVE);
	auto len = mavlink_msg_to_send_buffer(buf, &msg);

	mavlink_status_t status;
	mavlink_parse_char(MAVLINK_COMM_0, buf[0], &msg, &status);
#endif
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
    m.def("test_mavlink", &core::UCore::test_mavlink, "cpp mavlink test call");
    m.def("test_mavlink_v2", &core::UCore::test_mavlink_v2, "cpp mavlink v2 test call");
}