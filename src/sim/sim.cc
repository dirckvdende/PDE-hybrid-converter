
#include "sim.h"

using namespace sim;

Sim::Sim() { }

Sim::~Sim() { }

void Sim::setText(const std::string &txt) {
    this->txt = txt;
    parser.setText(txt);
}