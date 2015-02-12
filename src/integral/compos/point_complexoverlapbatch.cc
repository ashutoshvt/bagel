//
// BAGEL - Parallel electron correlation program.
// Filename: point_complexoverlapbatch.cc
// Copyright (C) 2014 Toru Shiozaki
//
// Author: Ryan D. Reynolds <RyanDReynolds@u.northwestern.edu>
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


#include <complex>
#include <src/integral/carsphlist.h>
#include <src/util/timer.h>
#include <src/integral/compos/point_complexoverlapbatch.h>

using namespace std;
using namespace bagel;

const static CCarSphList carsphlist;

static const double pisqrtinv__ = 1.0 / sqrt(pi__);

void Point_ComplexOverlapBatch::compute() {


  const CSortList sort_(spherical_);

  complex<double>* const intermediate_p = stack_->get<complex<double>>(size_block_);
  fill_n(intermediate_p, size_block_, 0.0);
  perform_VRR(intermediate_p);

  complex<double>* const cdata = data_;
  const complex<double>* const csource = intermediate_p;
  complex<double>* const intermediate_c = stack_->get<complex<double>>(cont0_ * cont1_ * asize_intermediate_);
  fill_n(intermediate_c, cont0_ * cont1_ * asize_intermediate_, 0.0);
  perform_contraction(asize_intermediate_, csource, prim0_, prim1_, intermediate_c,
                      basisinfo_[0]->contractions(), basisinfo_[0]->contraction_ranges(), cont0_,
                      basisinfo_[1]->contractions(), basisinfo_[1]->contraction_ranges(), cont1_);

  if (basisinfo_[0]->spherical() && basisinfo_[1]->spherical()) {
    // transform both indices to spherical
    complex<double>* const intermediate_i = stack_->get<complex<double>>(cont0_ * cont1_ * asize_final_);
    fill_n(intermediate_i, cont0_ * cont1_ * asize_final_, 0.0);
    const unsigned int carsph_index = basisinfo_[0]->angular_number() * ANG_HRR_END + basisinfo_[1]->angular_number();
    const int nloops = cont0_ * cont1_;
    carsphlist.carsphfunc_call(carsph_index, nloops, intermediate_c, intermediate_i);

    const unsigned int sort_index = basisinfo_[1]->angular_number() * ANG_HRR_END + basisinfo_[0]->angular_number();
    sort_.sortfunc_call(sort_index, cdata, intermediate_i, cont1_, cont0_, 1, swap01_);
    stack_->release(cont0_ * cont1_ * asize_final_, intermediate_i);
  } else {
    const unsigned int sort_index = basisinfo_[1]->angular_number() * ANG_HRR_END + basisinfo_[0]->angular_number();
    sort_.sortfunc_call(sort_index, cdata, intermediate_c, cont1_, cont0_, 1, swap01_);
  }

  stack_->release(cont0_ * cont1_ * asize_intermediate_, intermediate_c);
  stack_->release(size_block_, intermediate_p);

}


void Point_ComplexOverlapBatch::perform_VRR(std::complex<double>* intermediate) {

  const int worksize = amax1_;
  complex<double>* workx = stack_->get<complex<double>>(worksize);
  complex<double>* worky = stack_->get<complex<double>>(worksize);
  complex<double>* workz = stack_->get<complex<double>>(worksize);

  const double pAx = location_[0] - basisinfo_[0]->position(0);
  const double pAy = location_[1] - basisinfo_[0]->position(1);
  const double pAz = location_[2] - basisinfo_[0]->position(2);
  const double pBx = location_[0] - basisinfo_[1]->position(0);
  const double pBy = location_[1] - basisinfo_[1]->position(1);
  const double pBz = location_[2] - basisinfo_[1]->position(2);

  for (int ii = 0; ii != prim0_*prim1_; ++ii) {
    // Perform VRR
    int offset_ii = ii*asize_intermediate_;
    complex<double>* current_data = &intermediate[offset_ii];

    const double cxp = xp_[ii];
    const complex<double> cxpl = location_[0] - P_[ii*3  ];
    const complex<double> cypl = location_[1] - P_[ii*3+1];
    const complex<double> czpl = location_[2] - P_[ii*3+2];

    // obtain S(0, 0)
    const double tmp = pisqrtinv__ * std::sqrt(cxp);
    workx[0] = coeffsx_[ii] * tmp * std::exp(- cxp * (cxpl * cxpl));
    worky[0] = coeffsy_[ii] * tmp * std::exp(- cxp * (cypl * cypl));
    workz[0] = coeffsz_[ii] * tmp * std::exp(- cxp * (czpl * czpl));

    for (int i = 1; i != amax1_; ++i) {
      // obtain S(i, 0)
      workx[i] = pAx*workx[i-1];
      worky[i] = pAy*worky[i-1];
      workz[i] = pAz*workz[i-1];
    }

    // peform HRR
    for (int j = 1; j <= ang1_ + 1; ++j) {
      for (int i = 0; i != amax1_ - j; ++i) {
        // obtain S(i, j)
        workx[j*amax1_+i] = pBx*workx[(j-1)*amax1_+i];
        worky[j*amax1_+i] = pBy*worky[(j-1)*amax1_+i];
        workz[j*amax1_+i] = pBz*workz[(j-1)*amax1_+i];
      }
    }

    /// assembly process

    for (int iz = 0; iz <= amax_; ++iz) {
      for (int iy = 0; iy <= amax_ - iz; ++iy) {
        const complex<double> iyiz = workz[iz] * worky[iy];
        for (int ix = max(0, amin_ - iy - iz); ix <= amax_ - iy - iz; ++ix) {
          int pos = amapping_[ix + amax1_ * (iy + amax1_ * iz)];
          current_data[pos] = workx[ix] * iyiz;
        }
      }
    }

  } // end of primsize loop

  stack_->release(worksize, workz);
  stack_->release(worksize, worky);
  stack_->release(worksize, workx);
}


std::complex<double> Point_ComplexOverlapBatch::get_P(const double coord1, const double coord2, const double exp1, const double exp2, const double one12,
                                                       const int dim, const bool swap) {
  const double Areal = coord1*exp1;
  const double Breal = coord2*exp2;
  const double Aimag = basisinfo_[0]->vector_potential(dim);
  const double Bimag = basisinfo_[1]->vector_potential(dim);
  double imag;
  if (swap) imag = 0.5*(Bimag - Aimag);
  else imag = 0.5*(Aimag - Bimag);
  const std::complex<double> num (Areal + Breal, imag);
  return num * one12;
}

