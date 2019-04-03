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
#include <set>
#include <unordered_set>
#include <random>
#include <iostream>
#include <string>
#include <stdexcept>
#include <numeric>
#include <limits>
#include <cmath>


namespace rggen
{//start of namespace rggen

typedef unsigned int Node;
typedef pcg32 RNGType;
typedef std::pair<Node,Node> Edge;
typedef std::vector<std::pair<Node,Node> > EdgeList;
typedef std::set<std::pair<Node,Node> > EdgeSet;

/*
 * Generation of configuration model graph using direct sampling. Can be simple
 * or not.
 */
class ConfigurationModelGenerator
{
public:
    ConfigurationModelGenerator(
            const std::vector<unsigned int>& degree_sequence,
            unsigned int seed);

    //graph generation methods
    EdgeList get_graph(bool simple_graph = false);


private:
    RNGType gen_;
    std::vector<unsigned int> degree_sequence_;
};

/*
 * Sampling of configuration model graph using MCMC
 */
class ConfigurationModelSampler
{
public:
    ConfigurationModelSampler(
            const EdgeList& edge_list,
            unsigned int seed, bool simple_graph = true);

    //graph generation methods
    EdgeList get_graph(unsigned int step);


private:
    bool exists(const Edge& e) const;
    void edge_swap();
    //members
    bool simple_graph_;
    RNGType gen_;
    EdgeList current_edge_list_;
    EdgeSet current_edge_set_;

};

/*
 * Generation of clustered networks using bipartite one-mode projection
 */
class ClusteredGraphGenerator
{
public:
    ClusteredGraphGenerator(
            const std::vector<unsigned int>& clique_size_sequence,
            const std::vector<unsigned int>& membership_sequence,
            unsigned int seed);

    //graph generation methods
    std::pair<EdgeList,std::vector<std::set<Node>>> get_graph();

private:
    RNGType gen_;
    std::vector<unsigned int> clique_size_sequence_;
    std::vector<unsigned int> membership_sequence_;
    std::vector<unsigned int> clique_stub_vector_;
    std::vector<Node> node_stub_vector_;
    double edge_probability_;
};


/* ==========================
 *     Utility functions
 * ==========================*/
inline unsigned int random_int(std::size_t size, RNGType& gen)
{
    return floor(std::generate_canonical<double,
        std::numeric_limits<double>::digits>(gen)*size);
}

inline double random_01(RNGType& gen)
{
    return std::generate_canonical<double,
        std::numeric_limits<double>::digits>(gen);
}

//randomly match nodes as in an ER network
inline void random_matching(EdgeSet& edge_set, std::vector<Node>& node_vector,
        double edge_probability, RNGType& gen)
{
    size_t N = node_vector.size();
    for (size_t i = 0; i < N - 1; i++)
    {
        size_t j = i + 1;
        while (j < N)
        {
            if (edge_probability < 1.)
            {
                double r = random_01(gen);
                j += floor(log(r)/log(1 - edge_probability));
            }
            if (j < N)
            {
                if (node_vector[i] < node_vector[j])
                {
                     edge_set.insert(std::make_pair(node_vector[i],
                                 node_vector[j]));
                }
                if (node_vector[i] > node_vector[j])
                {
                     edge_set.insert(std::make_pair(node_vector[j],
                                 node_vector[i]));
                }
                j += 1;
            }
        }
    }
}

}//end of namespace rggen

#endif /* GRAPH_GENERATOR_HPP_ */
