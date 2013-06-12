//
//  belmanford.h
//  belman-ford
//
//  Created by Tomáš Krejčí on 6/12/13.
//  Copyright (c) 2013 Tomáš Krejčí. All rights reserved.
//

#ifndef __belman_ford__belmanford__
#define __belman_ford__belmanford__

#include <iostream>
#include <vector>
#include <map>

class belmanford {
public:
    using distance_type = int;
    using matrix_type = std::vector<std::vector<distance_type>>;
    using map_type = std::map<std::pair<std::size_t, std::size_t>, distance_type>;
    
    static const distance_type infty;
    
    struct vertex_type {
        std::map<std::size_t, distance_type> adj;
        std::vector<vertex_type>::iterator pred;
        distance_type dist = infty;
    };
    
    matrix_type load_file(std::string filename);
    void run(std::string file);
    std::vector<vertex_type> algorithm(const matrix_type & m,
                                       std::size_t origin);
    
    distance_type length(const std::vector<vertex_type> & vertices,
                         std::size_t v) const;
    
    std::ostream & print_path(std::ostream & out,
                              const std::vector<vertex_type> & vertices,
                              std::size_t v) const;
};

int make_path(int argc, char * argv[]);

#endif /* defined(__belman_ford__belmanford__) */
