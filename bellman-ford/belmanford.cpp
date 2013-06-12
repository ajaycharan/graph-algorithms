//
//  belmanford.cpp
//  belman-ford
//
//  Created by Tomáš Krejčí on 6/12/13.
//  Copyright (c) 2013 Tomáš Krejčí. All rights reserved.
//

#include "belmanford.h"

#include <limits>
#include <fstream>
#include <sstream>

const belmanford::distance_type belmanford::infty =
    std::numeric_limits<belmanford::distance_type>::max() / 2 - 1;

belmanford::matrix_type belmanford::load_file(std::string filename) {
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

void belmanford::run(std::string file) {
    auto m = load_file(file);
    auto res = algorithm(m, 0);
    
    for (std::size_t i = 0; i < m.size(); ++i) {
        print_path(std::cout, res, i) << ", L: " << length(res, i) << std::endl;
    }
}

std::vector<belmanford::vertex_type> belmanford::algorithm(const matrix_type & m,
                                                           std::size_t origin) {
    if (!m.size() && m[0].size() != m.size())
        throw "bad size";
    
    std::cout << m.size() << "x" << m[0].size() << std::endl;
    
    const auto n = m.size();
    std::vector<vertex_type> vertices(n);
    
    map_type edges;
    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t j = 0; j < n; ++j)
            if (m[i][j]) edges.emplace(std::make_pair(i, j), m[i][j]);
    
    auto from = [](const map_type::iterator & e) { return e->first.first; };
    auto to = [](const map_type::iterator & e) { return e->first.second; };
    auto dist = [&](std::size_t v)->distance_type& { return vertices[v].dist; };
    auto length = [&](const map_type::iterator & e) { return e->second; };
    auto pred = [&](std::size_t v) -> std::vector<vertex_type>::iterator&
    { return vertices[v].pred; };
    
    vertices[origin].dist = 0;
    vertices[origin].pred = vertices.begin() + origin;
    
    for (std::size_t i = 0; i < n - 1; ++i)
        for (auto e = edges.begin(); e != edges.end(); ++e)
            if (dist(from(e)) + length(e) < dist(to(e))) {
                dist(to(e)) = dist(from(e)) + length(e);
                pred(to(e)) = vertices.begin() + e->first.first;
            }

    for (auto e = edges.begin(); e != edges.end(); ++e)
        if (dist(from(e)) + length(e) < dist(to(e)))
            std::cout << "obsahuje zaporne cykly" << std::endl;
    
    return vertices;
}

belmanford::distance_type belmanford::length(const std::vector<vertex_type> & vertices,
                                             std::size_t v) const {
    return vertices[v].dist;
}

std::ostream & belmanford::print_path(std::ostream & out,
                                      const std::vector<vertex_type> & vertices,
                                      std::size_t v) const {
    if (vertices[v].pred == vertices.begin() + v) return out << v;
    else {
        out << v << " -> ";
        return print_path(out, vertices, vertices[v].pred - vertices.begin());
    }
}

int make_path(int argc, char * argv[]) {
    if (2 != argc) {
        std::cerr << "invalid arguments supplied" << std::endl;
        return 1;
    }
    
    try {
        belmanford p;
        p.run(argv[1]);
    } catch (std::string & e) {
        std::cerr << "ERROR: " << e << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unexpected error catched - exiting" << std::endl;
        auto e = std::current_exception();
        return 1;
    }
    
    return 0;
}
