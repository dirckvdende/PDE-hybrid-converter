
#pragma once

#include "presystem.h"

namespace pde::parser {

/**
 * Transforms the input system to a form that can be processed by the rest of
 * the compiler
 */
class InputTransform {

public:

    /**
     * Constructor
     * @param preSystem A reference to the presystem to transform
     */
    InputTransform(PreSystem &preSystem);

    /**
     * Destructor
     */
    ~InputTransform();

    /**
     * Run the input transformer
     */
    void run();

private:

    // A reference to the presystem to tranform, note that the system will
    // be modified by this object
    PreSystem &preSystem;

};

}