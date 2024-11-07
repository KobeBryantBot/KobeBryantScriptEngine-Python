#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class MyClass {
public:
    void myMethod() { py::print("Called myMethod from MyClass"); }
};

class AnotherClass {
public:
    int anotherMethod(int value) {
        py::print("Called anotherMethod from AnotherClass");
        return value * 2; // 示例返回值
    }
};

#include "api/Logger.hpp"
#include "api/Schedule.hpp"

void stdFunction(std::function<void()> func) {
    // 调用传入的函数
    Logger().warn("Call");
    Schedule::addRepeatTask(
        std::chrono::seconds(5),
        [=] {
            Logger().warn("try call {}", func ? true : false);
            func();
        },
        true
    );
}

PYBIND11_EMBEDDED_MODULE(my_module, m) {
    // 绑定 MyClass 类
    py::class_<MyClass>(m, "MyClass").def(py::init<>()).def("myMethod", &MyClass::myMethod);

    // 绑定 AnotherClass 类
    py::class_<AnotherClass>(m, "AnotherClass").def(py::init<>()).def("anotherMethod", &AnotherClass::anotherMethod);

    // 导出一个独立的函数
    m.def("myFunction", []() { py::print("Called myFunction"); });
    m.def("stdFunction", &stdFunction);
}
