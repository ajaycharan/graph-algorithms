//
//  dijkstra.h
//  dijkstra
//
//  Created by Tomáš Krejčí on 6/7/13.
//  Copyright (c) 2013 Tomáš Krejčí. All rights reserved.
//

#ifndef __dijkstra__dijkstra__
#define __dijkstra__dijkstra__

#include <iostream>
#include <map>
#include <vector>
#include <limits>

class dijkstra {
public:
    using distance_type = int;
    using matrix_type = std::vector<std::vector<distance_type>>;
    
    static const distance_type infty;
    
    struct vertex_type {
        std::map<std::size_t, distance_type> adj;
        std::vector<vertex_type>::iterator pred;
        distance_type dist = infty;
        bool closed = false;
    };
    
    void run(std::string file);
    void algorithm(const matrix_type & m, std::size_t origin);
    
private:
    matrix_type load_file(std::string filename);
};

int make_path(int argc, char * argv[]);


#endif /* defined(__dijkstra__dijkstra__) */
