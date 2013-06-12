//
//  floydwarshal.h
//  floydwarshal
//
//  Created by Tomas Krejci on 6/12/13.
//  Copyright (c) 2013 Tomas Krejci. All rights reserved.
//

#ifndef __floydwarshal__floydwarshal__
#define __floydwarshal__floydwarshal__

#include <iostream>
#include <vector>

class floydwarshal {
public:
    using distance_type = int;
    using matrix_type = std::vector<std::vector<distance_type>>;


    static const distance_type infty;
    
    void run(std::string file);
    void algorithm(const matrix_type & m);

private:
    matrix_type load_file(std::string filename);
    std::ostream & dfs_floydwarshal(std::ostream & out,
                                    const std::vector<std::vector<std::pair<distance_type, std::size_t>>> & mat,
                                    std::size_t i,
                                    std::size_t j);
};

int make_path(int argc, char * argv[]);

#endif /* defined(__floydwarshal__floydwarshal__) */
