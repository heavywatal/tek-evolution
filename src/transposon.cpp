// -*- mode: c++; coding: utf-8 -*-
/*! @file transposon.cpp
    @brief Implementation of Transposon class
*/
#include "transposon.hpp"

#include <iostream>

#include <wtl/debug.hpp>
#include <wtl/math.hpp>
#include <wtl/prandom.hpp>

namespace tek {

double Transposon::ALPHA_ = 0.8;
double Transposon::THRESHOLD_ = 1.0 - Transposon::ALPHA_;
unsigned int Transposon::BETA_ = 24U;

namespace po = boost::program_options;

po::options_description Transposon::options_desc() {HERE;
    po::options_description description("Transposon");
    description.add_options()
      ("alpha", po::value(&ALPHA_)->default_value(ALPHA_))
      ("beta", po::value(&BETA_)->default_value(BETA_));
    return description;
}

void Transposon::set_parameters() {HERE;
    THRESHOLD_ = 1.0 - ALPHA_;
}

void Transposon::mutate() {
    static std::uniform_int_distribution<size_t> POS_DIST(0U, SEQUENCE_LENGTH_ - 1U);
    sequence_.flip(POS_DIST(wtl::sfmt()));
}

double Transposon::activity() const {
    if (has_indel_) return 0.0;
    const double diff = sequence_.count() * OVER_L_;
    if (diff >= THRESHOLD_) return 0.0;
    return wtl::pow(1.0 - diff / THRESHOLD_, BETA_);
}

std::ostream& Transposon::write(std::ostream& ost) const {
    return ost << has_indel_ << sequence_;
}

std::ostream& operator<<(std::ostream& ost, const Transposon& x) {
    return x.write(ost);
}

void Transposon::unit_test() {HERE;
    Transposon te;
    te.mutate();
    te.mutate();
    std::cout << te << std::endl;
    std::cout << te.activity() << std::endl;
    te.indel();
    std::cout << te << std::endl;
    std::cout << te.activity() << std::endl;
}

} // namespace tek