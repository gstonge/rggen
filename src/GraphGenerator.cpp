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

#include "GraphGenerator.hpp"
#include <algorithm>

using namespace std;

namespace rggen
{//start of namespace rggen

EdgeList GraphGenerator::configuration_model(
        const std::vector<unsigned int>& degree_sequence,
        bool simple_graph)
{
    EdgeList edge_list;

    // Generate the stubs list
    vector<Node> stub_list;
    for (Node i=0; i < degree_sequence.size(); i++)
    {
        for (unsigned int j=0; j<degree_sequence[i]; j++)
        {
            stub_list.push_back(i);
        }
    }
    shuffle(stub_list.begin(),stub_list.end(), gen_);

    // Connect the stubs at random to generate the edge list
    while (!stub_list.empty())
    {
        Node stub1,stub2,node1,node2;
        pair<Node,Node> edge;
        stub1 = random_int(stub_list.size(), gen_);
        node1 = stub_list[stub1];
        swap(stub_list[stub1],stub_list[stub_list.size()-1]);
        stub_list.pop_back();
        stub2 = random_int(stub_list.size(), gen_);
        node2 = stub_list[stub2];
        swap(stub_list[stub2],stub_list[stub_list.size()-1]);
        stub_list.pop_back();
        if (node1 < node2)
            edge = make_pair(node1,node2);
        else
            edge = make_pair(node2,node1);
        edge_list.push_back(edge);
    }

    if (simple_graph)
    {
        // Check for self-loops and repeated edges
        bool faulty_links = true;
        while (faulty_links)
        {
            faulty_links = false;
            sort(edge_list.begin(),edge_list.end());
            for (size_t edge1=0; edge1<edge_list.size(); edge1++)
            {
                Node node1a = edge_list[edge1].first;
                Node node1b = edge_list[edge1].second;
                // If the link is faulty, rewire the stubs
                if (node1a == node1b || edge_list[edge1] == edge_list[(edge1+1)
                        % edge_list.size()])
                {
                    faulty_links = true;
                    unsigned int edge2 = random_int(edge_list.size(), gen_);
                    Node node2a = edge_list[edge2].first;
                    Node node2b = edge_list[edge2].second;
                    // Switch stubs
                    int choice = random_int(2, gen_);
                    if (choice)
                    {
                        edge_list[edge1].second = node2b;
                        edge_list[edge2].second = node1b;
                    }
                    else
                    {
                        edge_list[edge1].second = node2a;
                        edge_list[edge2].first = node1b;
                    }
                    if (edge_list[edge1].first > edge_list[edge1].second)
                    {
                        swap(edge_list[edge1].first,edge_list[edge1].second);
                    }
                    if (edge_list[edge2].first > edge_list[edge2].second)
                    {
                        swap(edge_list[edge2].first,edge_list[edge2].second);
                    }
                }
            }
        }
    }
}



}//end of namespace rggen
