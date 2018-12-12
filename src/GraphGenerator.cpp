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

/* ========================================
 * Configuration model generator
 * ======================================== */

//Constructor of configuration model generator
ConfigurationModelGenerator::ConfigurationModelGenerator(
        const vector<unsigned int>& degree_sequence, unsigned int seed) :
    gen_(seed), degree_sequence_(degree_sequence)
{
}

//Generates configuration model instance
EdgeList ConfigurationModelGenerator::get_graph(
        bool simple_graph)
{
    EdgeList edge_list;

    // Generate the stubs list
    vector<Node> stub_list;
    for (Node i=0; i < degree_sequence_.size(); i++)
    {
        for (unsigned int j=0; j<degree_sequence_[i]; j++)
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

    return edge_list;
}

/* ========================================
 * Configuration model sampler
 * ======================================== */

//Constructor of configuration model sampler
ConfigurationModelSampler::ConfigurationModelSampler(
            const EdgeList& edge_list,
            unsigned int seed, bool simple_graph) :
    gen_(seed), current_edge_list_(edge_list), simple_graph_(simple_graph),
    current_edge_set_()
{
    for (int i = 0; i < edge_list.size(); i++)
    {
        current_edge_set_.insert(edge_list[i]);
    }
}

//verify the existance of an edge
bool ConfigurationModelSampler::exists(const Edge& e) const
{
    Edge e_copy = e;
    bool in_edge_set = current_edge_set_.count(e_copy);
    swap(e_copy.first, e_copy.second);
    return in_edge_set or current_edge_set_.count(e_copy);
}

//perform an edge swap
void ConfigurationModelSampler::edge_swap()
{
    //get first edge
    size_t index1 = random_int(current_edge_list_.size(), gen_);
    Edge e1 = current_edge_list_[index1];

    //remove from edge list and edge set
    swap(current_edge_list_[index1], current_edge_list_.back());
    current_edge_list_.pop_back();
    current_edge_set_.erase(e1);

    //get a second edge
    size_t index2 = random_int(current_edge_list_.size(), gen_);
    Edge e2 = current_edge_list_[index2];

    //resample e2 if same edge
    while(e2 == e1)
    {
        index2 = random_int(current_edge_list_.size(), gen_);
        e2 = current_edge_list_[index2];
    }

    //remove from the edge list and edge set
    swap(current_edge_list_[index2], current_edge_list_.back());
    current_edge_list_.pop_back();
    current_edge_set_.erase(e2);

    //choose between two possible types of swap
    int choice = random_int(2, gen_);
    if(choice)
    {
        swap(e2.second, e1.second);
    }
    else
    {
        swap(e2.second, e1.first);
    }

    //verify if the new graph is simple graph
    if(simple_graph_)
    {
        if(e1.first == e1.second or e2.first == e2.second or
                exists(e1) or exists(e2))
        {
            //the swap makes the graph exit the graph space : swap back
            if(choice)
            {
                swap(e2.second, e1.second);
            }
            else
            {
                swap(e2.second, e1.first);
            }
        }
    }

    //insert the new edges
    current_edge_set_.insert(e1);
    current_edge_set_.insert(e2);
    current_edge_list_.push_back(e1);
    current_edge_list_.push_back(e2);
}

//return the current edge list after a certain mixing time
EdgeList ConfigurationModelSampler::get_graph(unsigned int step)
{
    for (int i = 0; i < step; i++)
    {
        edge_swap();
    }
    return current_edge_list_;
}

/* ========================================
 * Clustered graph generator
 * ======================================== */

//Constructor of configuration model generator
ClusteredGraphGenerator::ClusteredGraphGenerator(
        const vector<unsigned int>& group_size_sequence,
        const vector<unsigned int>& membership_sequence,
        double edge_probability, unsigned int seed) :
    gen_(seed),
    group_size_sequence_(group_size_sequence),
    membership_sequence_(membership_sequence),
    edge_probability_(edge_probability)
{
    try
    {
        if (accumulate(group_size_sequence.begin(), group_size_sequence.end(),
                    0) != accumulate(membership_sequence.begin(),
                    membership_sequence.end(), 0))
        {
            throw invalid_argument(
                    "Membership and group size sequence do not match");
        }
    }
    catch (invalid_argument& e)
    {
        throw e;
    }
}

//get a clustered graph realization
pair<EdgeList,vector<set<Node>>> ClusteredGraphGenerator::get_graph()
{
    //create groups as vector
    vector<vector<Node>> group_vector(group_size_sequence_.size());
    vector<set<Node>> group_set_vector(group_size_sequence_.size());

    //create group and member index lists
    vector<unsigned int> group_index_vector;
    vector<Node> member_index_vector;
    for (Node i = 0; i < membership_sequence_.size(); i++)
    {
        for (int j = 0; j < membership_sequence_[i]; j++)
        {
            member_index_vector.push_back(i);
        }
    }
    for (unsigned int i = 0; i < group_size_sequence_.size(); i++)
    {
        for (int j = 0; j < group_size_sequence_[i]; j++)
        {
            group_index_vector.push_back(i);
        }
    }

    //shuffle the lists
    shuffle(group_index_vector.begin(),group_index_vector.end(), gen_);
    shuffle(member_index_vector.begin(),member_index_vector.end(), gen_);

    //define the edge set and try the matching process
    EdgeSet edge_set;

    //insert members in groups
    for (size_t i = 0; i < group_index_vector.size(); i++)
    {
        group_vector[group_index_vector[i]].push_back(
                member_index_vector[i]);
        group_set_vector[group_index_vector[i]].insert(
                member_index_vector[i]);
    }

    //For each group, connect the nodes as an ER network
    for (auto iter = group_vector.begin(); iter != group_vector.end();
            iter++)
    {
        random_matching(edge_set, *iter, edge_probability_, gen_);
    }

    return make_pair(EdgeList(edge_set.begin(), edge_set.end()),
            group_set_vector);
}


}//end of namespace rggen
