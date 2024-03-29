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
// Forward declarations for the FST and FST script classes.

#ifndef FST_SCRIPT_FSTSCRIPT_DECL_H_
#define FST_SCRIPT_FSTSCRIPT_DECL_H_

#include <fst/fst-decl.h>
#include <fst/exports/exports.h>

namespace fst {
namespace script {

class fstscript_EXPORT ArcClass;

class fstscript_EXPORT ArcIteratorClass;
class fstscript_EXPORT MutableArcIteratorClass;

class fstscript_EXPORT EncodeMapperClass;

class fstscript_EXPORT FstClass;
class fstscript_EXPORT MutableFstClass;
class fstscript_EXPORT VectorFstClass;

class fstscript_EXPORT StateIteratorClass;

class fstscript_EXPORT WeightClass;

}  // namespace script
}  // namespace fst

#endif  // FST_SCRIPT_FSTSCRIPT_DECL_H_
