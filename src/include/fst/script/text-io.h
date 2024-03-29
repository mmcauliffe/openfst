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
//
// Utilities for reading and writing textual strings representing states,
// labels, and weights and files specifying label-label pairs and potentials
// (state-weight pairs).

#ifndef FST_SCRIPT_TEXT_IO_H__
#define FST_SCRIPT_TEXT_IO_H__

#include <string>
#include <vector>

#include <fst/script/weight-class.h>
#include <string_view>
#include <fst/exports/exports.h>

namespace fst {
namespace script {

bool fstscript_EXPORT ReadPotentials(std::string_view weight_type, const std::string &source,
                    std::vector<WeightClass> *potentials);

bool fstscript_EXPORT WritePotentials(const std::string &source,
                     const std::vector<WeightClass> &potentials);

}  // namespace script
}  // namespace fst

#endif  // FST_SCRIPT_TEXT_IO_H__
