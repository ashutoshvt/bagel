//
// BAGEL - Brilliantly Advanced General Electronic Structure Library
// Filename: MSCASPT2_contract_gen.cc
// Copyright (C) 2017 Toru Shiozaki
//
// Author: Jae Woo Park <jwpk1201@northwestern.edu>
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

#include <bagel_config.h>
#ifdef COMPILE_SMITH


#include <src/smith/caspt2/MSCASPT2.h>
#include <src/smith/caspt2/MSCASPT2_contract_tasks.h>

using namespace std;
using namespace bagel;
using namespace bagel::SMITH;
using namespace bagel::SMITH::MSCASPT2;

Task900::Task900(vector<shared_ptr<Tensor>> t, const bool reset) : reset_(reset) {
  deci_ = t[0];
}

Task901::Task901(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,4> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  out_ = t[0];
  in_ = in;

  subtasks_.reserve(range[3]->nblock());
  for (auto& ci0 : *range[3])
    if (t[0]->is_local(ci0))
      subtasks_.push_back(make_shared<Task_local>(array<const Index,1>{{ci0}}, in, t[0], range));
}

Task902::Task902(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,4> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  out_ = t[0];
  in_ = in;

  subtasks_.reserve(range[3]->nblock());
  for (auto& ci0 : *range[3])
    if (t[0]->is_local(ci0))
      subtasks_.push_back(make_shared<Task_local>(array<const Index,1>{{ci0}}, in, t[0], range));
}

Task903::Task903(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,4> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  out_ = t[0];
  in_ = in;

  subtasks_.reserve(range[3]->nblock());
  for (auto& ci0 : *range[3])
    if (t[0]->is_local(ci0))
      subtasks_.push_back(make_shared<Task_local>(array<const Index,1>{{ci0}}, in, t[0], range));
}

Task904::Task904(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,4> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  out_ = t[0];
  in_ = in;

  subtasks_.reserve(range[3]->nblock());
  for (auto& ci0 : *range[3])
    if (t[0]->is_local(ci0))
      subtasks_.push_back(make_shared<Task_local>(array<const Index,1>{{ci0}}, in, t[0], range));
}

Task905::Task905(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,4> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  out_ = t[0];
  in_ = in;

  subtasks_.reserve(range[3]->nblock());
  for (auto& ci0 : *range[3])
    if (t[0]->is_local(ci0))
      subtasks_.push_back(make_shared<Task_local>(array<const Index,1>{{ci0}}, in, t[0], range));
}
#endif