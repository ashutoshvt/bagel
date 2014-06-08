//
// BAGEL - Parallel electron correlation program.
// Filename: sphharmonics.h
// Copyright (C) 2014 Toru Shiozaki
//
// Author: Hai-Anh Le <anh@u.northwestern.edu>
// Maintainer: Shiozaki group
//
// This file is part of the BAGEL package.
//
// The BAGEL package is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// The BAGEL package is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with the BAGEL package; see COPYING.  If not, write to
// the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
//


#ifndef __SRC_INTEGRAL_ECP_SPHHARMONICS_H
#define __SRC_INTEGRAL_ECP_SPHHARMONICS_H

#include <array>
#include <vector>
#include <complex>
#include <cmath>
#include <src/util/constants.h>
#include <src/math/factorial.h>
#include <src/math/comb.h>

namespace bagel {

class SphHarmonics {
  protected:

    std::array<int, 2> angular_momentum_;
    std::array<double, 3> centre_;
    double theta_;
    double phi_;

    double LegendrePolynomial(const int l, const int am, const double x) const;

  public:

    SphHarmonics(const std::array<int, 2> lm, const std::array<double, 3> c);
    SphHarmonics(const int l, const int m, const std::array<double, 3> c);
    SphHarmonics(const std::array<int, 2> lm);
    SphHarmonics(const int l, const int m);

    ~SphHarmonics() {}

    double centre(const int i) const { return centre_[i]; }
    std::array<double, 3> centre() const { return centre_; }
    int angular_momentum(const int i) const { return angular_momentum_[i]; }
    std::array<int, 2> angular_momentum() const { return angular_momentum_; }

    double theta() const { return theta_; }
    double phi() const { return phi_; }

    std::complex<double> ylm() const;
    double zlm() const;
    double zlm(const int l, const int m) const;

    void print() const;

    double sph_to_USP(const int lx, const int ly) const;
    std::vector<std::pair<double, int>> sph_to_USPs_expansion() const;
    void sph_to_USPs_expansion_print() const;

};

}

#endif
