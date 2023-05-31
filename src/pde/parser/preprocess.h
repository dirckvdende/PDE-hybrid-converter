
#pragma once

#include "pde/spec.h"
#include <unordered_map>

namespace pde::parser {

class Preprocessor {

public:

    /**
     * Constructor
     * @param system The input PDE system to preprocess
     */
    Preprocessor(PDESystem &system);

    /**
     * Destructor
     */
    ~Preprocessor();

    /**
     * Run the preprocessor
     */
    void run();

private:

    /**
     * Replace dimension names with dimension indices
     */
    void replaceDims();

    /**
     * Change abstract derivatives to concrete derivatives
     */
    void replaceDerivs();

    // The input PDE system to preprocess
    PDESystem &system;
    // Dimension name map
    std::unordered_map<std::string, size_t> dimMap;

};

}