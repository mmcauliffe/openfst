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

#ifndef FST_SCRIPT_INFO_H_
#define FST_SCRIPT_INFO_H_

#include <string>
#include <tuple>

#include <fst/script/arcfilter-impl.h>
#include <fst/script/arg-packs.h>
#include <fst/script/fst-class.h>
#include <fst/script/info-impl.h>

#include <fst/script/script-impl.h>
#include <fst/exports/exports.h>

namespace fst {
namespace script {

using FstInfoArgs = std::tuple<const FstClass &, bool, ArcFilterType,
                               const std::string &, bool>;

template <class Arc>
void Info(FstInfoArgs *args) {
  const Fst<Arc> &fst = *std::get<0>(*args).GetFst<Arc>();
  const FstInfo info(fst, std::get<1>(*args), std::get<2>(*args),
                     std::get<3>(*args), std::get<4>(*args));
  info.Info();
}

void fstscript_EXPORT Info(const FstClass &fst, bool test_properties, ArcFilterType arc_filter,
          const std::string &info_type, bool verify);

}  // namespace script
}  // namespace fst

#endif  // FST_SCRIPT_INFO_H_
