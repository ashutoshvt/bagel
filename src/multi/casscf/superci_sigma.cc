//
// BAGEL - Brilliantly Advanced General Electronic Structure Library
// Filename: superci_sigma.cc
// Copyright (C) 2011 Toru Shiozaki
//
// Author: Toru Shiozaki <shiozaki@northwestern.edu>
// Maintainer: Shiozaki group
//
// This file is part of the BAGEL package.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


#include <src/multi/casscf/superci.h>

using namespace std;
using namespace bagel;


//////////////////////////////////////////////////////
// gradient vectors ... they are definitely correct.
//////////////////////////////////////////////////////


// <a/i|H|0> = 2f_ai /sqrt(2)
void SuperCI::grad_vc(const shared_ptr<Matrix> f, shared_ptr<RotFile> sigma) {
  if (!nvirt_ || !nclosed_) return;
  double* target = sigma->ptr_vc();
  for (int i = 0; i != nclosed_; ++i, target += nvirt_)
    blas::ax_plus_y_n(std::sqrt(2.0), f->element_ptr(nocc_,i), nvirt_, target);
}


// <a/r|H|0> finact_as h_sr + (as|tu)D_rs,tu = fact_ar  (/sqrt(nr) - due to normalization)
void SuperCI::grad_va(const shared_ptr<Matrix> fact, shared_ptr<RotFile> sigma) {
  if (!nvirt_ || !nact_) return;
  double* target = sigma->ptr_va();
  for (int i = 0; i != nact_; ++i, target += nvirt_) {
    const double fac = (occup_[i]>occup_thresh) ? 1.0/std::sqrt(occup_[i]) : 0.0;
    blas::ax_plus_y_n(fac, fact->element_ptr(nocc_, i), nvirt_, target);
  }
}


// <r/i|H|0> = (2f_ri - f^act_ri)/sqrt(2-nr)
void SuperCI::grad_ca(const shared_ptr<Matrix> f, shared_ptr<Matrix> fact, shared_ptr<RotFile> sigma) {
  if (!nclosed_ || !nact_) return;
  double* target = sigma->ptr_ca();
  for (int i = 0; i != nact_; ++i, target += nclosed_) {
    const double fac = (2.0-occup_[i] > occup_thresh) ? 1.0/std::sqrt(2.0-occup_[i]) : 0.0;
    blas::ax_plus_y_n(2.0*fac, f->element_ptr(0,nclosed_+i), nclosed_, target);
    blas::ax_plus_y_n(-fac, fact->element_ptr(0,i), nclosed_, target);
  }
}



