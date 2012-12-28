//
// BAGEL - Parallel electron correlation program.
// Filename: scf.h
// Copyright (C) 2012 Toru Shiozaki
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


#ifndef __BAGEL_SRC_SCF_SCF_H
#define __BAGEL_SRC_SCF_SCF_H

#include <src/scf/scf_base.h>
#include <src/scf/levelshift.h>
#include <src/util/diis.h>
#include <src/prop/dipole.h>
#include <src/wfn/reference.h>
#include <iostream>
#include <iomanip>
#include <src/parallel/paramatrix.h>
#include <src/parallel/mpi_interface.h>
#include <src/util/timer.h>

namespace bagel {

template<int DF>
class SCF : public SCF_base {
  protected:
    std::shared_ptr<LevelShift> levelshift_;

  public:
    SCF(const std::multimap<std::string, std::string>& idata_, const std::shared_ptr<const Geometry> geom,
        const std::shared_ptr<const Reference> re = std::shared_ptr<const Reference>())
      : SCF_base(idata_, geom, re) {

      // For the moment, I can't be bothered to test the level shifting apparatus for UHF and ROHF cases.
      // In the future, this should probably be moved to SCF_base and designed to work properly there
      double lshift = read_input<double>(idata_, "levelshift", 0.0);
      if (lshift == 0.0) {
        levelshift_ = std::shared_ptr<LevelShift>(new LevelShift());
      }
      else {
        levelshift_ = std::shared_ptr<LevelShift>(new ShiftVirtual(nocc_, lshift));
      }

      if (DF == 1) {
        // TODO init schwarz for auxiliary basis
      }
    };

    ~SCF() {};

    void compute() override {
      std::string indent = "  ";
      std::shared_ptr<Fock<DF> > previous_fock;
      std::shared_ptr<Fock<DF> > hcore_fock;
      {
        previous_fock = std::shared_ptr<Fock<DF> >(new Fock<DF>(geom_, hcore_));
        if (DF) hcore_fock = previous_fock;
      }

      if (SCF_base::coeff_ == nullptr) {
        ParaMatrix intermediate = *tildex_ % *previous_fock * *tildex_;
        intermediate.diagonalize(eig());
        coeff_ = std::shared_ptr<Coeff>(new Coeff(*tildex_ * intermediate));
      }
      aodensity_ = coeff_->form_density_rhf(nocc_);

      std::cout << indent << "=== Nuclear Repulsion ===" << std::endl << indent << std::endl;
      std::cout << indent << std::fixed << std::setprecision(10) << std::setw(15) << geom_->nuclear_repulsion() << std::endl << std::endl;
      std::cout << indent << "    * DIIS with " << (density_change_ ? "density changes" : "orbital gradients") << " will be used."
                << std::endl << std::endl;
      std::cout << indent << "=== RHF iteration (" + geom_->basisfile() + ") ===" << std::endl << indent << std::endl;

      // starting SCF iteration

      DIIS<Matrix> diis(5);
      std::shared_ptr<Matrix> densitychange = aodensity_; // assumes hcore guess...

      Timer scftime;
      for (int iter = 0; iter != max_iter_; ++iter) {

#ifdef HAVE_MPI_H
        Timer pdebug;
#endif

        std::shared_ptr<Fock<DF> >fock;
        if (DF == 0)
          fock = std::shared_ptr<Fock<DF> >(new Fock<DF>(geom_, previous_fock, densitychange, schwarz_));
        else
          fock = std::shared_ptr<Fock<DF> >(new Fock<DF>(geom_, hcore_fock, aodensity_, schwarz_, coeff_->slice(0, nocc_)));
        previous_fock = fock;
        // Need to share exactly the same between MPI processes
        if (DF == 0) mpi__->broadcast(previous_fock->data(), previous_fock->size(), 0);

#ifdef HAVE_MPI_H
        pdebug.tick_print("Fock build");
#endif

        ParaMatrix intermediate = *coeff_ % *fock * *coeff_;

#ifdef HAVE_MPI_H
        pdebug.tick_print("Trans to orth");
#endif

// TODO level shift - needed?
//      intermediate.add_diag(1.0, this->nocc(), geom_->nbasis());
        levelshift_->shift(intermediate);

        intermediate.diagonalize(eig());

#ifdef HAVE_MPI_H
        pdebug.tick_print("Diag");
#endif

        coeff_ = std::shared_ptr<Coeff>(new Coeff((*coeff_) * intermediate));
        std::shared_ptr<Matrix> new_density = coeff_->form_density_rhf(nocc_);

        std::shared_ptr<Matrix> error_vector(new Matrix(
          density_change_ ? (*new_density - *aodensity_) : (*fock**aodensity_**overlap_ - *overlap_**aodensity_**fock)
        ));
        const double error = error_vector->rms();

        energy_ = 0.5*(*aodensity_ * *hcore_).trace() + geom_->nuclear_repulsion();
        for (int i = 0; i != this->nocc(); ++i) energy_ += eig_[i];

        std::cout << indent << std::setw(5) << iter << std::setw(20) << std::fixed << std::setprecision(8) << energy_ << "   "
                                          << std::setw(17) << error << std::setw(15) << std::setprecision(2) << scftime.tick() << std::endl;

#ifdef HAVE_MPI_H
        pdebug.tick_print("Post process");
#endif

        if (error < thresh_scf_) {
          std::cout << indent << std::endl << indent << "  * SCF iteration converged." << std::endl << std::endl;
          break;
        } else if (iter == max_iter_-1) {
          std::cout << indent << std::endl << indent << "  * Max iteration reached in SCF." << std::endl << std::endl;
          break;
        }

        std::shared_ptr<Matrix> diis_density;
        if (iter >= diis_start_) {
          std::shared_ptr<Matrix> tmp_fock = diis.extrapolate(make_pair(fock, error_vector));
          std::shared_ptr<ParaMatrix> intermediate(new ParaMatrix(*tildex_ % *tmp_fock * *tildex_));

#ifdef HAVE_MPI_H
          pdebug.tick_print("DIIS");
#endif
          intermediate->diagonalize(eig());
          coeff_ = std::shared_ptr<Coeff>(new Coeff(*tildex_**intermediate));
          diis_density = coeff_->form_density_rhf(nocc_);

#ifdef HAVE_MPI_H
          pdebug.tick_print("Diag");
#endif
        } else {
          diis_density = new_density;
        }

        densitychange = std::shared_ptr<Matrix>(new Matrix(*diis_density - *aodensity_));
        aodensity_ = diis_density;

      }
      // by default we compute dipoles
      if (!geom_->external()) {
        Dipole mu(geom_, aodensity_);
        mu.compute();
      }
    };

    std::shared_ptr<Reference> conv_to_ref() const {
      std::shared_ptr<Reference> out(new Reference(geom_, coeff(), nocc(), 0, geom_->nbasis()-nocc(), energy()));
      std::vector<double> e(eig_.get(), eig_.get()+geom_->nbasis());
      out->set_eig(e);
      return out;
    };

};

}

#endif
