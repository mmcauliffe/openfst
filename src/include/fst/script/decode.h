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

#ifndef FST_SCRIPT_DECODE_H_
#define FST_SCRIPT_DECODE_H_

#include <tuple>
#include <utility>

#include <fst/encode.h>
#include <fst/script/encodemapper-class.h>
#include <fst/script/fst-class.h>

#include <fst/script/script-impl.h>
#include <fst/exports/exports.h>

namespace fst {
namespace script {

using FstDecodeArgs = std::pair<MutableFstClass *, const EncodeMapperClass &>;

template <class Arc>
void Decode(FstDecodeArgs *args) {
  MutableFst<Arc> *fst = std::get<0>(*args)->GetMutableFst<Arc>();
  const EncodeMapper<Arc> &mapper = *std::get<1>(*args).GetEncodeMapper<Arc>();
  Decode(fst, mapper);
}

void fstscript_EXPORT Decode(MutableFstClass *fst, const EncodeMapperClass &encoder);

}  // namespace script
}  // namespace fst

#endif  // FST_SCRIPT_DECODE_H_
