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
// Prints out binary FSTs in simple text format used by AT&T.

#include <cstring>
#include <memory>
#include <ostream>
#include <string>
#ifdef _WIN32
#include <io.h>
#endif
#include <fcntl.h>
#include <iostream>

#include <fst/flags.h>
#include <fst/log.h>
#include <fstream>
#include <fst/script/print.h>

DECLARE_bool(acceptor);
DECLARE_string(isymbols);
DECLARE_string(osymbols);
DECLARE_string(ssymbols);
DECLARE_bool(numeric);
DECLARE_string(save_isymbols);
DECLARE_string(save_osymbols);
DECLARE_bool(show_weight_one);
DECLARE_bool(allow_negative_labels);
DECLARE_string(missing_symbol);

int fstprint_main(int argc, char **argv) {
  namespace s = fst::script;
  using fst::SymbolTable;
  using fst::SymbolTableTextOptions;
  using fst::script::FstClass;

  std::string usage =
      "Prints out binary FSTs in simple text format.\n\n  Usage: ";
  usage += argv[0];
  usage += " [binary.fst [text.fst]]\n";

  std::set_new_handler(FailedNewHandler);
  SET_FLAGS(usage.c_str(), &argc, &argv, true);
  if (argc > 3) {
    ShowUsage();
    return 1;
  }

  const std::string in_name =
      (argc > 1 && strcmp(argv[1], "-") != 0) ? argv[1] : "";
  const std::string out_name =
      (argc > 2 && strcmp(argv[2], "-") != 0) ? argv[2] : "";

  std::unique_ptr<FstClass> fst(FstClass::Read(in_name));
  if (!fst) return 1;

  std::string dest = "standard output";
  std::ofstream fstrm;
  if (argc == 3) {
    fstrm.open(argv[2], std::ios_base::out | std::ios_base::binary);
    if (!fstrm) {
      LOG(ERROR) << argv[0] << ": Open failed, file = " << argv[2];
      return 1;
    }
    dest = argv[2];
  }
  #ifdef _WIN32
  if (!fstrm.is_open()) {
      _setmode(_fileno(stdout), _O_BINARY);
  }
  #endif
  std::ostream &ostrm = fstrm.is_open() ? fstrm : std::cout;
  ostrm.precision(9);

  const SymbolTableTextOptions opts(FST_FLAGS_allow_negative_labels);

  std::unique_ptr<const SymbolTable> isyms;
  if (!FST_FLAGS_isymbols.empty() && !FST_FLAGS_numeric) {
    isyms.reset(SymbolTable::ReadText(FST_FLAGS_isymbols, opts));
    if (!isyms) return 1;
  }

  std::unique_ptr<const SymbolTable> osyms;
  if (!FST_FLAGS_osymbols.empty() && !FST_FLAGS_numeric) {
    osyms.reset(SymbolTable::ReadText(FST_FLAGS_osymbols, opts));
    if (!osyms) return 1;
  }

  std::unique_ptr<const SymbolTable> ssyms;
  if (!FST_FLAGS_ssymbols.empty() && !FST_FLAGS_numeric) {
    ssyms.reset(SymbolTable::ReadText(FST_FLAGS_ssymbols));
    if (!ssyms) return 1;
  }

  if (!isyms && !FST_FLAGS_numeric && fst->InputSymbols()) {
    isyms.reset(fst->InputSymbols()->Copy());
  }

  if (!osyms && !FST_FLAGS_numeric && fst->OutputSymbols()) {
    osyms.reset(fst->OutputSymbols()->Copy());
  }

  s::Print(*fst, ostrm, dest, isyms.get(), osyms.get(), ssyms.get(),
           FST_FLAGS_acceptor, FST_FLAGS_show_weight_one,
           FST_FLAGS_missing_symbol);

  if (isyms && !FST_FLAGS_save_isymbols.empty()) {
    if (!isyms->WriteText(FST_FLAGS_save_isymbols)) return 1;
  }

  if (osyms && !FST_FLAGS_save_osymbols.empty()) {
    if (!osyms->WriteText(FST_FLAGS_save_osymbols)) return 1;
  }

  return 0;
}
