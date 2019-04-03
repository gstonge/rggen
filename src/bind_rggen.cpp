/*
 * MIT License
 *
 * Copyright (c) 2018 Guillaume St-Onge
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "GraphGenerator.hpp"

using namespace std;
using namespace rggen;

namespace py = pybind11;


PYBIND11_MODULE(_rggen, m)
{
    py::class_<ConfigurationModelGenerator>(m, "ConfigurationModelGenerator")

        .def(py::init<vector<unsigned int>, unsigned int >(), R"pbdoc(
            Default constructor of the class ConfigurationModelGenerator

            Args:
               degree_sequence: Sequence of degree for the network
               seed: Seed for the RNG.
            )pbdoc", py::arg("degree_sequence"), py::arg("seed") = 42)

        .def("get_graph", &ConfigurationModelGenerator::get_graph,
                R"pbdoc(
            Create a random edge list from the configuration model.

            Args:
               simple_graph: bool indicating if the network must be a
               simple graph
            )pbdoc", py::arg("simple_graph") = false);

    py::class_<ConfigurationModelSampler>(m, "ConfigurationModelSampler")

        .def(py::init<EdgeList, unsigned int, bool >(), R"pbdoc(
            Default constructor of the class ConfigurationModelSampler

            Args:
               edge_list: Edge list for the initial network
               seed: Seed for the RNG.
               simple_graph: Bool indicating if the generated graph are simple
            )pbdoc", py::arg("edge_list"), py::arg("seed") = 42,
                py::arg("simple_graph") = true)

        .def("get_graph", &ConfigurationModelSampler::get_graph,
                R"pbdoc(
            Sample a random edge list after a certain number of mixing steps

            Args:
               step: Number of edge swap before sampling
            )pbdoc", py::arg("step"));

    py::class_<ClusteredGraphGenerator>(m, "ClusteredGraphGenerator")

        .def(py::init<vector<unsigned int>, vector<unsigned int>,
                unsigned int >(), R"pbdoc(
            Default constructor of the class ClusteredGraphGenerator

            Args:
               membership_sequence: Sequence of group membership for each node
               clique_size_sequence: Sequence of group size for each group
               seed: Seed for the RNG.
            )pbdoc", py::arg("membership_sequence"),
                py::arg("clique_size_sequence"), py::arg("seed") = 42)

        .def("get_graph", &ClusteredGraphGenerator::get_graph,
                R"pbdoc(
            Create a random edge list from the clustered graph model.
            )pbdoc");

}
