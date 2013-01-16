//
// BAGEL - Parallel electron correlation program.
// Filename: distfci_ab.h
// Copyright (C) 2013 Toru Shiozaki
//
// Author: Toru Shiozaki <shiozaki@northwestern.edu>
// Maintainer: Shiozaki group
//
// This file is part of the BAGEL package.
//
// The BAGEL package is free software; you can redistribute it and\/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
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

#ifndef __SRC_FCI_DISTFCI_AB_H
#define __SRC_FCI_DISTFCI_AB_H

#include <bitset>
#include <memory>
#include <src/util/f77.h>
#include <src/fci/determinants.h>
#include <src/fci/mofile.h>
#include <src/fci/civec.h>

namespace bagel {

class DistABTask {
  protected:
    std::bitset<nbit__> astring;
    std::shared_ptr<Determinants> base_det;
    std::shared_ptr<Determinants> int_det;
    std::shared_ptr<const MOFile> jop;
    std::shared_ptr<const DistCivec> cc;
    std::shared_ptr<DistCivec> sigma;

    std::unique_ptr<double[]> buf;
    std::unique_ptr<double[]> buf2;
    std::unique_ptr<double[]> buf3;

  public:
    DistABTask(const std::bitset<nbit__> ast, std::shared_ptr<Determinants> b, std::shared_ptr<Determinants> i, std::shared_ptr<const MOFile> j,
               std::shared_ptr<const DistCivec> cc, std::shared_ptr<DistCivec> s)
     : astring(ast), base_det(b), int_det(i), jop(j), sigma(s) {
      // first receive all the data (nele_a * lenb)
      const int norb_ = base_det->norb();
      const size_t lbs = base_det->lenb();
      const size_t lbt = int_det->lenb();

      buf = std::unique_ptr<double[]>(new double[lbs*norb_]);
      std::fill_n(buf.get(), lbs*norb_, 0.0);

      for (int i = 0; i != norb_; ++i) {
        if (!astring[i]) {
          std::bitset<nbit__> tmp = astring; tmp.set(i);
          cc->get_bstring_buf(buf.get()+i*lbs, base_det->lexical<0>(tmp));
        }
      }

      // TODO buffer should be nelec size (not norb size)
      buf2 = std::unique_ptr<double[]>(new double[lbt*norb_*norb_]);
      buf3 = std::unique_ptr<double[]>(new double[lbt*norb_*norb_]);
      std::fill_n(buf2.get(), lbt*norb_*norb_, 0.0);
    }

    void compute() {
      const int norb_ = base_det->norb();
      const int ij = norb_*norb_; 
      const size_t lbs = base_det->lenb();
      const size_t lbt = int_det->lenb();
      for (int k = 0, kl = 0; k != norb_; ++k)
        for (int l = 0; l != norb_; ++l, ++kl)
          for (auto& b : int_det->phiupb(l))
            buf2[b.source+lbt*kl] += base_det->sign(astring, -1, k) * b.sign * buf[b.target+k*lbs];

      dgemm_("n", "n", lbt, ij, ij, 1.0, buf2.get(), lbt, jop->mo2e_ptr(), ij, 0.0, buf3.get(), lbt);

      for (int i = 0; i < norb_; ++i) {
        if (astring[i]) continue;
        std::bitset<nbit__> atarget = astring; atarget.set(i);
        const double asign = base_det->sign(astring, -1, i);

        std::unique_ptr<double[]> bcolumn(new double[lbs]);
        std::fill_n(bcolumn.get(), lbs, 0.0);

        for (int j = 0; j < norb_; ++j) {
          for (auto& b : int_det->phiupb(j))
            bcolumn[b.target] += asign * b.sign * buf3[b.source+lbt*(j+norb_*i)];
        }
        sigma->accumulate_bstring_buf(bcolumn, base_det->lexical<0>(atarget));
      }
      sigma->flush_accumulate();
    }
};

}

#endif