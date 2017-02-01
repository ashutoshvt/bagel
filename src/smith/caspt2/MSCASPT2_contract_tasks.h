//
// BAGEL - Brilliantly Advanced General Electronic Structure Library
// Filename: MSCASPT2_contract_tasks.h
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

#ifndef __SRC_SMITH_MSCASPT2_CONTRACT_TASKS_H
#define __SRC_SMITH_MSCASPT2_CONTRACT_TASKS_H

#include <src/smith/indexrange.h>
#include <src/smith/tensor.h>
#include <src/smith/task.h>
#include <src/smith/subtask.h>
#include <src/smith/storage.h>

namespace bagel {
namespace SMITH {
namespace MSCASPT2 {

class Task900 : public Task {
  protected:
    std::shared_ptr<Tensor> deci_;
    IndexRange closed_;
    IndexRange active_;
    IndexRange virt_;
    IndexRange ci_;
    const bool reset_;

    void compute_() {
      if (reset_) deci_->zero();
    }

  public:
    Task900(std::vector<std::shared_ptr<Tensor>> t, const bool reset);
    ~Task900() {}
};

class Task901 : public Task {
  protected:
    std::shared_ptr<Tensor> out_;
    std::array<std::shared_ptr<const Tensor>,2> in_;
    class Task_local : public SubTask<1,2> {
      protected:
        const std::array<std::shared_ptr<const IndexRange>,4> range_;

        const Index& b(const size_t& i) const { return this->block(i); }
        std::shared_ptr<const Tensor> in(const size_t& i) const { return this->in_tensor(i); }
        std::shared_ptr<Tensor> out() { return this->out_tensor(); }

      public:
        Task_local(const std::array<const Index,1>& block, const std::array<std::shared_ptr<const Tensor>,2>& in, std::shared_ptr<Tensor>& out,
                   std::array<std::shared_ptr<const IndexRange>,4>& ran)
          : SubTask<1,2>(block, in, out), range_(ran) { }

        void compute() override;
    };

    std::vector<std::shared_ptr<Task_local>> subtasks_;

    void compute_() override {
      if (!out_->allocated())
        out_->allocate();
      for (auto& i : in_)
        i->init();
      for (auto& i : subtasks_) i->compute();
    }

  public:
    Task901(std::vector<std::shared_ptr<Tensor>> t, std::array<std::shared_ptr<const IndexRange>,4> range);
    ~Task901() {}

};

class Task902 : public Task {
  protected:
    std::shared_ptr<Tensor> out_;
    std::array<std::shared_ptr<const Tensor>,2> in_;
    class Task_local : public SubTask<1,2> {
      protected:
        const std::array<std::shared_ptr<const IndexRange>,4> range_;

        const Index& b(const size_t& i) const { return this->block(i); }
        std::shared_ptr<const Tensor> in(const size_t& i) const { return this->in_tensor(i); }
        std::shared_ptr<Tensor> out() { return this->out_tensor(); }

      public:
        Task_local(const std::array<const Index,1>& block, const std::array<std::shared_ptr<const Tensor>,2>& in, std::shared_ptr<Tensor>& out,
                   std::array<std::shared_ptr<const IndexRange>,4>& ran)
          : SubTask<1,2>(block, in, out), range_(ran) { }

        void compute() override;
    };

    std::vector<std::shared_ptr<Task_local>> subtasks_;

    void compute_() override {
      if (!out_->allocated())
        out_->allocate();
      for (auto& i : in_)
        i->init();
      for (auto& i : subtasks_) i->compute();
    }

  public:
    Task902(std::vector<std::shared_ptr<Tensor>> t, std::array<std::shared_ptr<const IndexRange>,4> range);
    ~Task902() {}

};

class Task903 : public Task {
  protected:
    std::shared_ptr<Tensor> out_;
    std::array<std::shared_ptr<const Tensor>,2> in_;
    class Task_local : public SubTask<1,2> {
      protected:
        const std::array<std::shared_ptr<const IndexRange>,4> range_;

        const Index& b(const size_t& i) const { return this->block(i); }
        std::shared_ptr<const Tensor> in(const size_t& i) const { return this->in_tensor(i); }
        std::shared_ptr<Tensor> out() { return this->out_tensor(); }

      public:
        Task_local(const std::array<const Index,1>& block, const std::array<std::shared_ptr<const Tensor>,2>& in, std::shared_ptr<Tensor>& out,
                   std::array<std::shared_ptr<const IndexRange>,4>& ran)
          : SubTask<1,2>(block, in, out), range_(ran) { }

        void compute() override;
    };

    std::vector<std::shared_ptr<Task_local>> subtasks_;

    void compute_() override {
      if (!out_->allocated())
        out_->allocate();
      for (auto& i : in_)
        i->init();
      for (auto& i : subtasks_) i->compute();
    }

  public:
    Task903(std::vector<std::shared_ptr<Tensor>> t, std::array<std::shared_ptr<const IndexRange>,4> range);
    ~Task903() {}

};

class Task904 : public Task {
  protected:
    std::shared_ptr<Tensor> out_;
    std::array<std::shared_ptr<const Tensor>,2> in_;
    class Task_local : public SubTask<1,2> {
      protected:
        const std::array<std::shared_ptr<const IndexRange>,4> range_;

        const Index& b(const size_t& i) const { return this->block(i); }
        std::shared_ptr<const Tensor> in(const size_t& i) const { return this->in_tensor(i); }
        std::shared_ptr<Tensor> out() { return this->out_tensor(); }

      public:
        Task_local(const std::array<const Index,1>& block, const std::array<std::shared_ptr<const Tensor>,2>& in, std::shared_ptr<Tensor>& out,
                   std::array<std::shared_ptr<const IndexRange>,4>& ran)
          : SubTask<1,2>(block, in, out), range_(ran) { }

        void compute() override;
    };

    std::vector<std::shared_ptr<Task_local>> subtasks_;

    void compute_() override {
      if (!out_->allocated())
        out_->allocate();
      for (auto& i : in_)
        i->init();
      for (auto& i : subtasks_) i->compute();
    }

  public:
    Task904(std::vector<std::shared_ptr<Tensor>> t, std::array<std::shared_ptr<const IndexRange>,4> range);
    ~Task904() {}

};

class Task905 : public Task {
  protected:
    std::shared_ptr<Tensor> out_;
    std::array<std::shared_ptr<const Tensor>,2> in_;
    class Task_local : public SubTask<1,2> {
      protected:
        const std::array<std::shared_ptr<const IndexRange>,4> range_;

        const Index& b(const size_t& i) const { return this->block(i); }
        std::shared_ptr<const Tensor> in(const size_t& i) const { return this->in_tensor(i); }
        std::shared_ptr<Tensor> out() { return this->out_tensor(); }

      public:
        Task_local(const std::array<const Index,1>& block, const std::array<std::shared_ptr<const Tensor>,2>& in, std::shared_ptr<Tensor>& out,
                   std::array<std::shared_ptr<const IndexRange>,4>& ran)
          : SubTask<1,2>(block, in, out), range_(ran) { }

        void compute() override;
    };

    std::vector<std::shared_ptr<Task_local>> subtasks_;

    void compute_() override {
      if (!out_->allocated())
        out_->allocate();
      for (auto& i : in_)
        i->init();
      for (auto& i : subtasks_) i->compute();
    }

  public:
    Task905(std::vector<std::shared_ptr<Tensor>> t, std::array<std::shared_ptr<const IndexRange>,4> range);
    ~Task905() {}

};


}
}
}

#endif