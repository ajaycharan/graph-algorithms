//
//  dijkstra.cpp
//  dijkstra
//
//  Created by Tomáš Krejčí on 6/7/13.
//  Copyright (c) 2013 Tomáš Krejčí. All rights reserved.
//

#include "dijkstra.h"

#include <algorithm>
#include <limits>
#include <fstream>
#include <sstream>

const dijkstra::distance_type dijkstra::infty =
    std::numeric_limits<dijkstra::distance_type>::max();

void dijkstra::algorithm(const matrix_type & m, std::size_t origin) {
    std::vector<vertex_type> vertices(m.size());
    std::vector<decltype(vertices)::iterator> queue(m.size());
    
    for (std::size_t i = 0; i < m.size(); ++i) {
        queue[i] = vertices.begin() + i;
        for (std::size_t v = 0; v < m.size(); ++v)
            if (m[i][v]) vertices[i].adj.emplace(v, m[i][v]);
    }
    
    vertices[origin].dist = 0;
    vertices[origin].pred = vertices.begin() + origin;
    
    auto cmp = [&](const decltype(vertices)::iterator & it1,
                   const decltype(vertices)::iterator & it2) {
        return it1->dist > it2->dist;
    };
    
    std::make_heap(queue.begin(), queue.end(), cmp);

    while (! queue.empty()) {
        std::pop_heap(queue.begin(), queue.end(), cmp);
        auto node = queue.back();
        queue.pop_back();
        node->closed = true;
        
        for (auto & a : node->adj) 
            if (! vertices[a.first].closed)
                if (node->dist + a.second < vertices[a.first].dist) {
                    vertices[a.first].dist = node->dist + a.second;
                    vertices[a.first].pred = node;
                    std::make_heap(queue.begin(), queue.end(), cmp);
                }
    }
    
    /* OUTPUT */
    for (std::size_t i = 0; i < vertices.size(); ++i) {
        std::cout << " -- VERTEX " << i + 1 << " (predecessor: "
        << vertices[i].pred - vertices.begin() + 1 << ", "
        << "distance: " << vertices[i].dist << ")" << std::endl;
    }
}

dijkstra::matrix_type dijkstra::load_file(std::string filename) {
    std::ifstream in(filename);
    if (!in.is_open())
        throw "unable to open file";
    
    std::string line;
    matrix_type m;
    distance_type num;
    
    while (in.good()) {
        std::getline(in, line);
        if (line.empty()) continue;
        std::istringstream s(line);
        
        matrix_type::value_type m_line;
        while (s >> num) m_line.push_back(num);
        m.push_back(std::move(m_line));
    }
    
    return m;
}

void dijkstra::run(std::string file) {
    auto m = load_file(file);
    algorithm(m, 0); // <-- Starting vertex
}

int make_path(int argc, char * argv[]) {
    if (2 != argc) {
        std::cerr << "invalid arguments supplied" << std::endl;
        return 1;
    }
    
    try {
        dijkstra p;
        p.run(argv[1]);
    } catch (std::string & e) {
        std::cerr << "ERROR: " << e << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unexpected error catched - exiting" << std::endl;
        return 1;
    }
    
    return 0;
}