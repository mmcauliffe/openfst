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

#include <fst/extensions/special/rho-fst.h>

#include <fst/arc.h>
#include <fst/fst.h>
#include <fst/register.h>

#ifdef fstspecial_EXPORTS

int64_t FST_FLAGS_rho_fst_rho_label = 0;
static FlagRegisterer<int64_t>
rho_fst_rho_label_flags_registerer("rho_fst_rho_label", FlagDescription<int64_t>(&FST_FLAGS_rho_fst_rho_label, 
    "Label of transitions to be interpreted as rho ('rest') "
    "transitions",
    "int64_t",
    __FILE__,
    0));

std::string FST_FLAGS_rho_fst_rewrite_mode = "auto";                                            \
static FlagRegisterer<std::string>                                                 \
rho_fst_rewrite_mode_flags_registerer("rho_fst_rewrite_mode", FlagDescription<std::string>(&FST_FLAGS_rho_fst_rewrite_mode, \
    "Rewrite both sides when matching? One of:"
    " \"auto\" (rewrite iff acceptor), \"always\", \"never\"", \
    "std::string", \
    __FILE__, \
    "auto"));

#endif // fstspecial_EXPORTS


namespace fst {

#ifndef _WIN32
    REGISTER_FST(RhoFst, StdArc);
    REGISTER_FST(RhoFst, LogArc);
    REGISTER_FST(RhoFst, Log64Arc);

    REGISTER_FST(InputRhoFst, StdArc);
    REGISTER_FST(InputRhoFst, LogArc);
    REGISTER_FST(InputRhoFst, Log64Arc);

    REGISTER_FST(OutputRhoFst, StdArc);
    REGISTER_FST(OutputRhoFst, LogArc);
    REGISTER_FST(OutputRhoFst, Log64Arc);
#endif


}  // namespace fst
