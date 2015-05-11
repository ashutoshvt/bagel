//
// BAGEL - Parallel electron correlation program.
// Filename: relmofile.cc
// Copyright (C) 2013 Toru Shiozaki
//
// Author: Michael Caldwell <caldwell@u.northwestern.edu>
// Maintainer: Shiozaki group
//
// This file is part of the BAGEL package.
//
// The BAGEL package is free software; you can redistribute it and/or modify
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

#include <iostream>
#include <algorithm>
#include <cmath>
#include <src/util/f77.h>
#include <src/util/math/quatmatrix.h>
#include <src/util/prim_op.h>
#include <src/ci/zfci/relmofile.h>
#include <src/mat1e/rel/reloverlap.h>
#include <src/mat1e/rel/relhcore.h>
#include <src/mat1e/giao/reloverlap_london.h>
#include <src/mat1e/giao/relhcore_london.h>

using namespace std;
using namespace bagel;

RelMOFile::RelMOFile(const shared_ptr<const Geometry> geom, shared_ptr<const RelCoeff_Block> co, const int charge, const bool gaunt, const bool breit, const bool tsymm)
 : charge_(charge), geom_(geom), coeff_(co), gaunt_(gaunt), breit_(breit), tsymm_(tsymm) {
  // density fitting is assumed
  assert(geom_->df());
}


// nstart and nfence are based on the convention in Dirac calculations
void RelMOFile::init(const int nstart, const int nfence, const bool restricted) {
  // first compute all the AO integrals in core
  nbasis_ = geom_->nbasis();
  nocc_ = (nfence - nstart)/2;
  assert((nfence - nstart) % 2 == 0);
  if (!geom_->dfs())
    geom_ = geom_->relativistic(gaunt_);

  // calculates the core fock matrix
  shared_ptr<const ZMatrix> hcore;
  if (!geom_->magnetism())
    hcore = make_shared<RelHcore>(geom_);
  else
    hcore = make_shared<RelHcore_London>(geom_);

  if (nstart != 0) {
    shared_ptr<const ZMatrix> den = coeff_->distmatrix()->form_density_rhf(nstart)->matrix();
    core_fock_ = make_shared<DFock>(geom_, hcore, coeff_->slice_copy(0, nstart), gaunt_, breit_, /*do_grad = */false, /*robust*/breit_);
    const complex<double> prod = (*den * (*hcore+*core_fock_)).trace();
    if (fabs(prod.imag()) > 1.0e-12) {
      stringstream ss; ss << "imaginary part of energy is nonzero!! Perhaps Fock is not Hermite for some reasons " << setprecision(10) << prod.imag();
      cout << ss.str() << endl;
    }
    core_energy_ = 0.5*prod.real();
  } else {
    core_fock_ = hcore;
    core_energy_ = 0.0;
  }

  // then compute Kramers adapated coefficient matrices
  shared_ptr<const ZMatrix> overlap;
  if (!geom_->magnetism())
    overlap = make_shared<RelOverlap>(geom_);
  else
    overlap = make_shared<RelOverlap_London>(geom_);

  if (!restricted) {
    shared_ptr<RelCoeff_Striped> kcoeff = coeff_->striped_format()->init_kramers_coeff_dirac(geom_, overlap, hcore, geom_->nele()-charge_, tsymm_, gaunt_, breit_);
    shared_ptr<RelCoeff_Block> kbcoeff = kcoeff->block_format();
    kramers_coeff_ = make_shared<Kramers<2,ZMatrix>>();
    kramers_coeff_->emplace(0, kbcoeff->slice_copy(nstart, nstart+(nfence-nstart)/2));
    kramers_coeff_->emplace(1, kbcoeff->slice_copy(nstart+(nfence-nstart)/2, nfence));
  } else {
    kramers_coeff_ = make_shared<Kramers<2,ZMatrix>>();
    kramers_coeff_->emplace(0, coeff_->slice_copy(nstart, nstart+(nfence-nstart)/2));
    kramers_coeff_->emplace(1, coeff_->slice_copy(nstart+(nfence-nstart)/2, nfence));
  }

  // calculate 1-e MO integrals
  shared_ptr<Kramers<2,ZMatrix>> buf1e = compute_mo1e(kramers_coeff_);

  // calculate 2-e MO integrals
  shared_ptr<Kramers<4,ZMatrix>> buf2e = compute_mo2e(kramers_coeff_);

  // compress and set mo1e_ and mo2e_
  compress_and_set(buf1e, buf2e);
}



// this is a static function!
shared_ptr<Kramers<2,ZMatrix>> RelMOFile::kramers(shared_ptr<const ZMatrix> coeff, shared_ptr<const ZMatrix> overlap, shared_ptr<const ZMatrix> hcore) {
  const int noff = coeff->mdim()/2;
  const int ndim = coeff->ndim();
  const int mdim = coeff->mdim();
  const int nb = ndim / 4;
  unique_ptr<complex<double>[]> eig = (*coeff % *hcore * *coeff).diag();

  auto out = make_shared<Kramers<2,ZMatrix>>();
  out->emplace(0, make_shared<ZMatrix>(ndim, noff));
  out->emplace(1, make_shared<ZMatrix>(ndim, noff));

  if (ndim%2 != 0 || ndim%4 != 0)
    throw logic_error("illegal call of RelMOFile::kramers");

  // overlap matrix
  auto sigmaz = overlap->copy();
  sigmaz->add_block(-2.0, nb, nb, nb, nb, sigmaz->get_submatrix(nb,nb,nb,nb));
  sigmaz->add_block(-2.0, nb*3, nb*3, nb, nb, sigmaz->get_submatrix(nb*3,nb*3,nb,nb));
  // just for convenience
  sigmaz->scale(-1.0);

  VectorB tmp(mdim);

  list<int> done;
  for (int i = 0; i != mdim; ++i) {
    if (find(done.begin(), done.end(), i) != done.end()) continue;
    list<int> current{i};
    const double e = eig[i].real();

    for (int j = i+1; j < mdim; ++j) {
      if (fabs(eig[j].real()-e)/fabs(e) < 1.0e-8)
        current.push_back(j);
    }
    const int n = current.size();
    if (n%2 != 0) throw runtime_error("orbitals are not kramers paired");

    auto cnow = make_shared<ZMatrix>(ndim, n);
    int j = 0;
    for (auto& i : current)
      cnow->copy_block(0, j++, ndim, 1, coeff->element_ptr(0,i));

    auto corig = cnow->copy();
    auto s = make_shared<ZMatrix>(*cnow % *sigmaz * *cnow);
    s->diagonalize(tmp);
    *cnow *= *s;

    // fix the phase - making the largest large-component element in each column real
#if 1
    for (int i = 0; i != n; ++i) {
      const int iblock = i/(n/2);
      complex<double> ele = *max_element(cnow->element_ptr(iblock*nb,i), cnow->element_ptr((iblock+1)*nb,i),
                                         [](complex<double> a, complex<double> b) { return norm(a)+1.0e-5 < norm(b); }); // favors the first one
      const complex<double> fac = norm(ele) / ele*complex<double>(1.0,1.0);
      for_each(cnow->element_ptr(0,i), cnow->element_ptr(0,i+1), [&fac](complex<double>& a) { a *= fac; });
    }
#endif

    // off diagonal
    const int m = n/2;
    cnow->add_block(-1.0, nb, 0, nb, m, cnow->get_submatrix(0, m, nb, m)->get_conjg());
    cnow->copy_block(0, m, nb, m, *cnow->get_submatrix(nb, 0, nb, m)->get_conjg() * (-1.0));
    cnow->add_block(-1.0, nb*3, 0, nb, m, cnow->get_submatrix(nb*2, m, nb, m)->get_conjg());
    cnow->copy_block(nb*2, m, nb, m, *cnow->get_submatrix(nb*3, 0, nb, m)->get_conjg() * (-1.0));

    // diagonal
    cnow->add_block(1.0, 0, 0, nb, m, cnow->get_submatrix(nb, m, nb, m)->get_conjg());
    cnow->copy_block(nb, m, nb, m, cnow->get_submatrix(0, 0, nb, m)->get_conjg());
    cnow->add_block(1.0, nb*2, 0, nb, m, cnow->get_submatrix(nb*3, m, nb, m)->get_conjg());
    cnow->copy_block(nb*3, m, nb, m, cnow->get_submatrix(nb*2, 0, nb, m)->get_conjg());

    auto diag = (*cnow % *overlap * *cnow).diag();
    for (int i = 0; i != n; ++i)
      for (int j = 0; j != ndim; ++j)
        cnow->element(j,i) /= sqrt(diag[i].real());

    ZMatrix unit = *corig % *overlap * *cnow;
    unit.purify_unitary();
    *cnow = *corig * unit;

    const int d = done.size();
    assert(d % 2 == 0);
    out->at(0)->copy_block(0, d/2, ndim, n/2, cnow->element_ptr(0, 0));
    out->at(1)->copy_block(0, d/2, ndim, n/2, cnow->element_ptr(0, n/2));

    done.insert(done.end(), current.begin(), current.end());
  }
  return out;
}


void RelMOFile::compress_and_set(shared_ptr<Kramers<2,ZMatrix>> buf1e, shared_ptr<Kramers<4,ZMatrix>> buf2e) {
  mo1e_ = buf1e;
  mo2e_ = make_shared<Kramers<4,ZMatrix>>();

  // Harrison requires <ij|kl> = (ik|jl)
  for (auto& mat : *buf2e) {
    shared_ptr<ZMatrix> tmp = mat.second->clone();
    sort_indices<0,2,1,3,0,1,1,1>(mat.second->data(), tmp->data(), nocc_, nocc_, nocc_, nocc_);
    bitset<4> s = mat.first.tag();
    s[2] = mat.first.tag()[1];
    s[1] = mat.first.tag()[2];
    mo2e_->emplace(s, tmp);
  }
}


shared_ptr<Kramers<2,ZMatrix>> RelJop::compute_mo1e(shared_ptr<const Kramers<2,ZMatrix>> coeff) {
  auto out = make_shared<Kramers<2,ZMatrix>>();
  const int n = tsymm_ ? 3 : 4;
  for (size_t i = 0; i != n; ++i)
    out->emplace(i, make_shared<ZMatrix>(*coeff->at(i/2) % *core_fock_ * *coeff->at(i%2)));
  if (tsymm_)
    out->emplace({1,1}, out->at({0,0})->get_conjg());

  assert(out->size() == 4);
  // symmetry requirement
  assert((*out->at({1,0}) - *out->at({0,1})->transpose_conjg()).rms() < 1.0e-8);
  // Kramers requirement
  assert((*make_shared<ZMatrix>(*coeff->at(1) % *core_fock_ * *coeff->at(1)) - *out->at({0,0})->get_conjg()).rms() < 1.0e-8 || !tsymm_);

  return out;
}


shared_ptr<Kramers<4,ZMatrix>> RelJop::compute_mo2e(shared_ptr<const Kramers<2,ZMatrix>> coeff) {

  auto compute = [&coeff, this](shared_ptr<Kramers<4,ZMatrix>> out, const bool gaunt, const bool breit) {
    assert(!breit || gaunt);
    // (1) make DFDists
    vector<shared_ptr<const DFDist>> dfs;
    if (!gaunt) {
      dfs = geom_->dfs()->split_blocks();
      dfs.push_back(geom_->df());
    } else {
      dfs = geom_->dfsl()->split_blocks();
    }
    list<shared_ptr<RelDF>> dfdists = DFock::make_dfdists(dfs, gaunt);

    // Separate Coefficients into real and imaginary
    // correlated occupied orbitals
    array<array<shared_ptr<const Matrix>,4>,2> rocoeff;
    array<array<shared_ptr<const Matrix>,4>,2> iocoeff;
    for (int k = 0; k != 2; ++k) {
      for (int i = 0; i != 4; ++i) {
        shared_ptr<const ZMatrix> oc = coeff->at(k)->get_submatrix(i*nbasis_, 0, nbasis_, nocc_);
        assert(nocc_ == coeff->at(k)->mdim());
        rocoeff[k][i] = oc->get_real_part();
        iocoeff[k][i] = oc->get_imag_part();
      }
    }

    // (2) first-transform
    array<list<shared_ptr<RelDFHalf>>,2> half_complex;
    for (int k = 0; k != 2; ++k)
      half_complex[k] = DFock::make_half_complex(dfdists, rocoeff[k], iocoeff[k]);

    // (3) split and factorize
    array<list<shared_ptr<RelDFHalf>>,2> half_complex_exch, half_complex_exch2;
    for (size_t k = 0; k != 2; ++k) {
      for (auto& i : half_complex[k]) {
        list<shared_ptr<RelDFHalf>> tmp = i->split(/*docopy=*/false);
        half_complex_exch[k].insert(half_complex_exch[k].end(), tmp.begin(), tmp.end());
      }
      half_complex[k].clear();
      DFock::factorize(half_complex_exch[k]);
    }

    // ** save 1 external integrals (to be used in CASSCF) *** //
    if (!gaunt) {
      half_complex_coulomb_ = half_complex_exch;
    } else {
      half_complex_gaunt_ = half_complex_exch;
    }

    if (breit) {
      // TODO Not the best implementation -- one could avoid apply_J to half-transformed objects
      auto breitint = make_shared<BreitInt>(geom_);
      list<shared_ptr<Breit2Index>> breit_2index;
      for (int i = 0; i != breitint->Nblocks(); ++i) {
        breit_2index.push_back(make_shared<Breit2Index>(breitint->index(i), breitint->data(i), geom_->df()->data2()));
        if (breitint->not_diagonal(i))
          breit_2index.push_back(breit_2index.back()->cross());
      }
      for (size_t k = 0; k != 2; ++k) {
        for (auto& i : half_complex_exch[k])
          half_complex_exch2[k].push_back(i->apply_J());

        for (auto& i : half_complex_exch[k])
          for (auto& j : breit_2index)
            if (i->alpha_matches(j)) {
              half_complex_exch2[k].push_back(i->apply_J()->multiply_breit2index(j));
              DFock::factorize(half_complex_exch2[k]);
            }
      }
    }

    // (4) compute (gamma|ii)
    shared_ptr<const Kramers<2,RelDFFull>> full = compute_full(rocoeff, iocoeff, half_complex_exch, true);
    shared_ptr<const Kramers<2,RelDFFull>> full2 = !breit ? full : compute_full(rocoeff, iocoeff, half_complex_exch2, false);

    // (5) compute 4-index quantities (16 of them - we are not using symmetry... and this is a very cheap step)
    const double gscale = gaunt ? (breit ? -0.5 : -1.0) : 1.0;
    for (size_t i = 0; i != 16; ++i) {
      // we do not need (1000, 0111, 1110, 0001, 1100, 0110)
      if (i == 8 || i == 7 || i == 14 || i == 1 || i == 12 || i == 6)
        continue;

      // we will construct (1111, 1010, 1101, 0100) later
      if ((tsymm_ && i == 15) || i == 10 || i == 13 || i == 4)
        continue;

      // we compute: 0000, 0010, 1001, 0101, 0011, 1011

      // TODO : put in if statement for apply_J if nact*nact is much smaller than number of MPI processes
      const int b2a = i/4;
      const int b2b = i%4;
      out->add(i, full->at(b2a)->form_4index(full2->at(b2b), gscale * (breit_ ? 0.5 : 1.0)));

      // in breit cases we explicitly symmetrize the Hamiltnian (hence the prefactor 0.5 above)
      if (breit_)
        *out->at(i) += *full2->at(b2a)->form_4index(full->at(b2b), gscale*0.5);
    }
  };

  auto out = make_shared<Kramers<4,ZMatrix>>();
  // Dirac-Coulomb term
  compute(out, false, false);

  if (gaunt_)
    compute(out, true, breit_);

  // Kramers and particle symmetry
  if (tsymm_)
    (*out)[{1,1,1,1}] = out->at({0,0,0,0})->get_conjg();

  (*out)[{1,0,1,0}] = out->at({0,1,0,1})->clone();
  shared_ptr<ZMatrix> m1010 = out->at({0,1,0,1})->get_conjg();
  sort_indices<1,0,3,2,0,1,1,1>(m1010->data(), out->at({1,0,1,0})->data(), nocc_, nocc_, nocc_, nocc_);

  (*out)[{1,1,0,1}] = out->at({1,0,1,1})->clone();
  shared_ptr<ZMatrix> m1101 = out->at({1,0,1,1})->get_conjg();
  sort_indices<3,2,1,0,0,1,1,1>(m1101->data(), out->at({1,1,0,1})->data(), nocc_, nocc_, nocc_, nocc_);

  (*out)[{0,1,0,0}] = out->at({0,0,1,0})->clone();
  shared_ptr<ZMatrix> m0100 = out->at({0,0,1,0})->get_conjg();
  sort_indices<3,2,1,0,0,1,1,1>(m0100->data(), out->at({0,1,0,0})->data(), nocc_, nocc_, nocc_, nocc_);

#if 0
  // for completeness we can compute the others too
  vector<int> target{8, 7, 14, 1, 6};
  for (auto& t : target) {
    bitset<4> tb(t);
    bitset<4> sb; sb[0] = tb[1]; sb[1] = tb[0]; sb[2] = tb[3]; sb[3] = tb[2];
    assert(out.find(tb) == out.end());
    out[tb] = out.at(sb)->clone();
    sort_indices<1,0,3,2,0,1,1,1>(out.at(sb)->data(), out.at(tb)->data(), nocc_, nocc_, nocc_, nocc_);
    transform(out.at(tb)->data(), out.at(tb)->data()+nocc_*nocc_*nocc_*nocc_, out.at(tb)->data(), [](complex<double> a) { return conj(a); });
  }
  out[bitset<4>("1100")] = out.at(bitset<4>("0011"))->transpose();
#endif

  return out;
}


shared_ptr<const Kramers<2,RelDFFull>>
  RelMOFile::compute_full(array<array<shared_ptr<const Matrix>,4>,2> rocoeff, array<array<shared_ptr<const Matrix>,4>,2> iocoeff,
                          array<list<shared_ptr<RelDFHalf>>,2> half, const bool appj, const bool appjj) {
  auto out = make_shared<Kramers<2,RelDFFull>>();

  // TODO remove once DFDistT class is fixed
  const bool transform_with_full = !(half[0].front()->nocc()*rocoeff[0][0]->mdim() <= mpi__->size());
  if (!transform_with_full) {
    for (int t = 0; t != 2; ++t)
      for (auto& i : half[t])
        if (appj)
          i = i->apply_J();
        else if (appjj)
          i = i->apply_JJ();
  }

  for (size_t t = 0; t != 4; ++t) {
    assert(!appj || !appjj);

    list<shared_ptr<RelDFFull>> dffull;
    for (auto& i : half[t/2])
      dffull.push_back(make_shared<RelDFFull>(i, rocoeff[t%2], iocoeff[t%2]));
    DFock::factorize(dffull);
    assert(dffull.size() == 1);
    dffull.front()->scale(dffull.front()->fac()); // take care of the factor
    (*out)[t] = dffull.front();

    if (transform_with_full) {
      if (appj)
        out->at(t) = out->at(t)->apply_J();
      else if (appjj)
        out->at(t) = out->at(t)->apply_JJ();
    }
  }
  return out;
}


void RelMOFile::rearrange_eig(VectorB& eig, shared_ptr<ZMatrix> coeff, const bool includes_neg) {
  const int n = coeff->ndim()/2;
  assert(2*n == coeff->ndim());  // could be triggered if Kramers + and - sets had different sizes or linear dependencies

  // check that pos & neg energy eigenvalues are properly separated
  assert(!includes_neg || *std::min_element(eig.begin()+n, eig.begin()+2*n) - *std::max_element(eig.begin(), eig.begin()+n) > c__*c__);

  // need to reorder things so negative energy states don't all come at the beginning
  // TODO there should be a more efficient way...
  VectorB tempv(2*n);
  shared_ptr<ZMatrix> tempm = coeff->clone();
  for (int i = 0; i != n; ++i) {
    tempv[  i] = eig[2*i];
    tempv[n+i] = eig[2*i+1];

    for (int j=0; j!=2*n; ++j) {
      tempm->element(j,   i) = coeff->element(j, 2*i);
      tempm->element(j, n+i) = coeff->element(j, 2*i+1);
    }
  }
  eig = tempv;
  *coeff = *tempm;
}

