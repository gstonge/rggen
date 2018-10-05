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

#ifndef GRAPH_GENERATOR_HPP_
#define GRAPH_GENERATOR_HPP_

#include "pcg-cpp/include/pcg_random.hpp"
#include <utility>
#include <vector>
#include <random>
#include <iostream>
#include <numeric>
#include <limits>


namespace rggen
{//start of namespace rggen

typedef unsigned int Node;
typedef pcg32 RNGType;
typedef std::vector<std::pair<Node,Node> > EdgeList;

/*
 * Class for general graph generation
 */
class ConfigurationModelGenerator
{
public:
    ConfigurationModelGenerator(
            const std::vector<unsigned int>& degree_sequence,
            unsigned int seed);

    //graph generation methods
    EdgeList get_graph(bool simple_graph = true);


private:
    RNGType gen_;
    std::vector<unsigned int> degree_sequence_;
};

//Utility function
inline unsigned int random_int(std::size_t size, RNGType& gen)
{
    return floor(std::generate_canonical<double,
        std::numeric_limits<double>::digits>(gen)*size);
}


}//end of namespace rggen

#endif /* GRAPH_GENERATOR_HPP_ */
