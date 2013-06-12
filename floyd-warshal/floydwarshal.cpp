//
//  floydwarshal.cpp
//  floydwarshal
//
//  Created by Tomas Krejci on 6/12/13.
//  Copyright (c) 2013 Tomas Krejci. All rights reserved.
//

#include "floydwarshal.h"

#include <fstream>
#include <sstream>

const floydwarshal::distance_type floydwarshal::infty =
    std::numeric_limits<floydwarshal::distance_type>::max() / 2 - 1;

floydwarshal::matrix_type floydwarshal::load_file(std::string filename) {
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

void floydwarshal::run(std::string file) {
    auto m = load_file(file);
    algorithm(m);
}

void floydwarshal::algorithm(const matrix_type &m) {
    const std::size_t n = m.size();
    if (!n) return;
    else if (m[0].size() != n) return;

    std::vector<std::pair<distance_type, std::size_t>> line(n, std::make_pair(infty, 0));
    std::vector<std::vector<std::pair<distance_type, std::size_t>>> D(n, line);

    auto d = [&D](std::size_t i, std::size_t j) -> distance_type& { return D[i][j].first; };
    auto p = [&D](std::size_t i, std::size_t j) -> std::size_t& { return D[i][j].second; };

    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            if (i == j) {
                d(i, i) = 0;
                p(i, i) = 0;
            }
            else {
                d(i, j) = m[i][j] == 0 ? infty : m[i][j];
                p(i, j) = m[i][j] == 0 ? 0 : i;
            }
        }
    }

    for (std::size_t k = 0; k < n; ++k)
        for (std::size_t i = 0; i < n; ++i)
            for (std::size_t j = 0; j < n; ++j)
                if (d(i, j) > d(i, k) + d(k, j)) {
                    d(i, j) = d(i, k) + d(k, j);
                    p(i, j) = p(k, j);
                }

    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t j = 0; j < n; ++j) {
            std::cout << i << " - " << j << ": ";
            dfs_floydwarshal(std::cout, D, i, j) << std::endl;
        }
    
}

std::ostream & floydwarshal::dfs_floydwarshal(std::ostream & out,
                                              const std::vector<std::vector<std::pair<distance_type, std::size_t>>> & mat,
                                              std::size_t i,
                                              std::size_t j) {
    if (i == j) return out << i << " ";
    else if (mat[i][j].second == 0) return out << "cesta neexistuje";
    else return dfs_floydwarshal(out, mat, i, mat[i][j].second) << j << " ";
}


int make_path(int argc, char * argv[]) {
    if (2 != argc) {
        std::cerr << "invalid arguments supplied" << std::endl;
        return 1;
    }

    try {
        floydwarshal p;
        p.run(argv[1]);
    } catch (std::string & e) {
        std::cerr << "ERROR: " << e << std::endl;
        return 1;
    } catch (const std::exception & e) {
        std::cerr << "Unexpected error catched - exiting with output" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unexpected error catched - exiting" << std::endl;
        auto e = std::current_exception();
        return 1;
    }

    return 0;
}