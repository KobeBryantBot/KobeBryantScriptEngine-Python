#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

extern void initCommandRegistry(py::module_& m);
extern void initEvent(py::module_& m);
extern void initSchedule(py::module_& m);
extern void initPacket(py::module_& m);
extern void initLogger(py::module_& m);

PYBIND11_EMBEDDED_MODULE(KobeBryantAPI, m) {
    initCommandRegistry(m);
    initLogger(m);
    initEvent(m);
    initPacket(m);
    initSchedule(m);
}