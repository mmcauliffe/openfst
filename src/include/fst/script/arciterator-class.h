// Copyright 2005-2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the 'License');
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an 'AS IS' BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// See www.openfst.org for extensive documentation on this weighted
// finite-state transducer library.

#ifndef FST_SCRIPT_ARCITERATOR_CLASS_H_
#define FST_SCRIPT_ARCITERATOR_CLASS_H_

#include <cstdint>
#include <memory>
#include <utility>

#include <fst/fstlib.h>
#include <fst/script/fst-class.h>
#include <fst/script/script-impl.h>
#include <fst/exports/exports.h>

// Scripting API support for ArcIterator.
//
// A call to Value() causes the underlying arc to be used to construct the
// associated ArcClass.

namespace fst {
namespace script {

// Non-mutable arc iterators.
    class fstscript_EXPORT ArcIteratorImplBase;
// Virtual interface implemented by each concrete ArcIteratorImpl<F>.
class ArcIteratorImplBase {
 public:
  virtual bool Done() const = 0;
  virtual uint8_t Flags() const = 0;
  virtual void Next() = 0;
  virtual size_t Position() const = 0;
  virtual void Reset() = 0;
  virtual void Seek(size_t a) = 0;
  virtual void SetFlags(uint8_t flags, uint8_t mask) = 0;
  virtual ArcClass Value() const = 0;
  virtual ~ArcIteratorImplBase() {}
};

// Templated implementation.
template <class Arc>
class ArcIteratorClassImpl : public ArcIteratorImplBase {
 public:
  explicit ArcIteratorClassImpl(const Fst<Arc> &fst, int64_t s)
      : aiter_(fst, s) {}

  bool Done() const final { return aiter_.Done(); }

  uint8_t Flags() const final { return aiter_.Flags(); }

  void Next() final { aiter_.Next(); }

  size_t Position() const final { return aiter_.Position(); }

  void Reset() final { aiter_.Reset(); }

  void Seek(size_t a) final { aiter_.Seek(a); }

  void SetFlags(uint8_t flags, uint8_t mask) final {
    aiter_.SetFlags(flags, mask);
  }

  // This is returned by value because it has not yet been constructed, and
  // is likely to participate in return-value optimization.
  ArcClass Value() const final { return ArcClass(aiter_.Value()); }

  ~ArcIteratorClassImpl() override {}

 private:
  ArcIterator<Fst<Arc>> aiter_;
};

class ArcIteratorClass;

using InitArcIteratorClassArgs =
    std::tuple<const FstClass &, int64_t, ArcIteratorClass *>;

// Untemplated user-facing class holding a templated pimpl.
class ArcIteratorClass {
 public:
  ArcIteratorClass(const FstClass &fst, int64_t s);

  template <class Arc>
  ArcIteratorClass(const Fst<Arc> &fst, int64_t s)
      : impl_(std::make_unique<ArcIteratorClassImpl<Arc>>(fst, s)) {}

  bool Done() const { return impl_->Done(); }

  uint8_t Flags() const { return impl_->Flags(); }

  void Next() { impl_->Next(); }

  size_t Position() const { return impl_->Position(); }

  void Reset() { impl_->Reset(); }

  void Seek(size_t a) { impl_->Seek(a); }

  void SetFlags(uint8_t flags, uint8_t mask) { impl_->SetFlags(flags, mask); }

  ArcClass Value() const { return impl_->Value(); }

  template <class Arc>
  friend void InitArcIteratorClass(InitArcIteratorClassArgs *args);

 private:
  std::unique_ptr<ArcIteratorImplBase> impl_;
};

template <class Arc>
void InitArcIteratorClass(InitArcIteratorClassArgs *args) {
  const Fst<Arc> &fst = *std::get<0>(*args).GetFst<Arc>();
  std::get<2>(*args)->impl_ =
      std::make_unique<ArcIteratorClassImpl<Arc>>(fst, std::get<1>(*args));
}

// Mutable arc iterators.

// Virtual interface implemented by each concrete MutableArcIteratorImpl<F>.
class MutableArcIteratorImplBase : public ArcIteratorImplBase {
 public:
  virtual void SetValue(const ArcClass &) = 0;

  ~MutableArcIteratorImplBase() override {}
};

// Templated implementation.
template <class Arc>
class MutableArcIteratorClassImpl : public MutableArcIteratorImplBase {
 public:
  explicit MutableArcIteratorClassImpl(MutableFst<Arc> *fst, int64_t s)
      : aiter_(fst, s) {}

  bool Done() const final { return aiter_.Done(); }

  uint8_t Flags() const final { return aiter_.Flags(); }

  void Next() final { aiter_.Next(); }

  size_t Position() const final { return aiter_.Position(); }

  void Reset() final { aiter_.Reset(); }

  void Seek(size_t a) final { aiter_.Seek(a); }

  void SetFlags(uint8_t flags, uint8_t mask) final {
    aiter_.SetFlags(flags, mask);
  }

  void SetValue(const ArcClass &ac) final { SetValue(ac.GetArc<Arc>()); }

  // This is returned by value because it has not yet been constructed, and
  // is likely to participate in return-value optimization.
  ArcClass Value() const final { return ArcClass(aiter_.Value()); }

  ~MutableArcIteratorClassImpl() override {}

 private:
  void SetValue(const Arc &arc) { aiter_.SetValue(arc); }

  MutableArcIterator<MutableFst<Arc>> aiter_;
};

class MutableArcIteratorClass;

using InitMutableArcIteratorClassArgs =
    std::tuple<MutableFstClass *, int64_t, MutableArcIteratorClass *>;

// Untemplated user-facing class holding a templated pimpl.
class MutableArcIteratorClass {
 public:
  MutableArcIteratorClass(MutableFstClass *fst, int64_t s);

  template <class Arc>
  MutableArcIteratorClass(MutableFst<Arc> *fst, int64_t s)
      : impl_(std::make_unique<MutableArcIteratorClassImpl<Arc>>(fst, s)) {}

  bool Done() const { return impl_->Done(); }

  uint8_t Flags() const { return impl_->Flags(); }

  void Next() { impl_->Next(); }

  size_t Position() const { return impl_->Position(); }

  void Reset() { impl_->Reset(); }

  void Seek(size_t a) { impl_->Seek(a); }

  void SetFlags(uint8_t flags, uint8_t mask) { impl_->SetFlags(flags, mask); }

  void SetValue(const ArcClass &ac) { impl_->SetValue(ac); }

  ArcClass Value() const { return impl_->Value(); }

  template <class Arc>
  friend void InitMutableArcIteratorClass(
      InitMutableArcIteratorClassArgs *args);

 private:
  std::unique_ptr<MutableArcIteratorImplBase> impl_;
};

template <class Arc>
void InitMutableArcIteratorClass(InitMutableArcIteratorClassArgs *args) {
  MutableFst<Arc> *fst = std::get<0>(*args)->GetMutableFst<Arc>();
  std::get<2>(*args)->impl_ =
      std::make_unique<MutableArcIteratorClassImpl<Arc>>(fst,
                                                          std::get<1>(*args));
}


}  // namespace script
}  // namespace fst

#endif  // FST_SCRIPT_ARCITERATOR_CLASS_H_
