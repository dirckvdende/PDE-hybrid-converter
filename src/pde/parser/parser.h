
#pragma once

#include "preparser.h"
#include "systemgen.h"
#include "transform.h"
#include "validator.h"

namespace pde::parser {

/**
 * Parses PDE text input to specification
 */
class Parser {

public:

    /**
     * Constructor
     */
    Parser();

    /**
     * Destructor
     */
    ~Parser();

    /**
     * Set the text to be parser
     * @param txt The input text to be parsed
     */
    void setText(const std::string &txt);

    /**
     * Run the parser
     */
    void run();

    /**
     * Get the PDE system specification that was parsed
     * @return A constant reference to the system specification
     */
    const PDESystem &getSpecs() const;

private:

    // Preparser object
    PreParser preParser;
    // Input validator
    InputValidator validator;
    // System transformer, for higher-order time derivatives
    InputTransform transformer;
    // PDE system generator
    SystemGen systemGen;

};

}