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
// Generates random paths through an FST.

#include <cstring>
#include <memory>
#include <string>

#include <fst/flags.h>
#include <fst/log.h>
#include <fst/script/getters.h>
#include <fst/script/randgen.h>

DECLARE_EXE_int32(max_length);
DECLARE_EXE_int32(npath);
DECLARE_EXE_uint64(seed);
DECLARE_EXE_string(select);
DECLARE_EXE_bool(weighted);
DECLARE_EXE_bool(remove_total_weight);

int fstrandgen_main(int argc, char **argv) {
  namespace s = fst::script;
  using fst::RandGenOptions;
  using fst::script::FstClass;
  using fst::script::VectorFstClass;

  std::string usage = "Generates random paths through an FST.\n\n  Usage: ";
  usage += argv[0];
  usage += " [in.fst [out.fst]]\n";

  std::set_new_handler(FailedNewHandler);
  SET_FLAGS(usage.c_str(), &argc, &argv, true);
  if (argc > 3) {
    ShowUsage();
    return 1;
  }

  VLOG(1) << argv[0] << ": Seed = " << FST_FLAGS_seed;

  const std::string in_name =
      (argc > 1 && strcmp(argv[1], "-") != 0) ? argv[1] : "";
  const std::string out_name =
      (argc > 2 && strcmp(argv[2], "-") != 0) ? argv[2] : "";

  std::unique_ptr<FstClass> ifst(FstClass::Read(in_name));
  if (!ifst) return 1;

  VectorFstClass ofst(ifst->ArcType());

  s::RandArcSelection ras;
  if (!s::GetRandArcSelection(FST_FLAGS_select, &ras)) {
    LOG(ERROR) << argv[0] << ": Unknown or unsupported select type "
               << FST_FLAGS_select;
    return 1;
  }

  s::RandGen(*ifst, &ofst,
             RandGenOptions<s::RandArcSelection>(
                 ras, FST_FLAGS_max_length,
                 FST_FLAGS_npath, FST_FLAGS_weighted,
                 FST_FLAGS_remove_total_weight),
             FST_FLAGS_seed);

  return !ofst.Write(out_name);
}
