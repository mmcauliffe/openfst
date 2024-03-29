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
// Creates binary FSTs from simple text format used by AT&T.

#include <cstring>
#include <iostream>
#include <istream>
#include <memory>
#include <string>
#ifdef _WIN32
#include <io.h>
#endif
#include <fcntl.h>
#include <iostream>

#include <fst/flags.h>
#include <fst/log.h>
#include <fstream>
#include <fst/script/compile.h>

DECLARE_bool(acceptor);
DECLARE_string(arc_type);
DECLARE_string(fst_type);
DECLARE_string(isymbols);
DECLARE_string(osymbols);
DECLARE_string(ssymbols);
DECLARE_bool(keep_isymbols);
DECLARE_bool(keep_osymbols);
DECLARE_bool(keep_state_numbering);
DECLARE_bool(allow_negative_labels);

int fstcompile_main(int argc, char **argv) {
  namespace s = fst::script;
  using fst::SymbolTable;
  using fst::SymbolTableTextOptions;

  std::string usage =
      "Creates binary FSTs from simple text format.\n\n  Usage: ";
  usage += argv[0];
  usage += " [text.fst [binary.fst]]\n";

  std::set_new_handler(FailedNewHandler);
  SET_FLAGS(usage.c_str(), &argc, &argv, true);
  if (argc > 3) {
    ShowUsage();
    return 1;
  }

  std::string source = "standard input";
  std::ifstream fstrm;
  if (argc > 1 && strcmp(argv[1], "-") != 0) {
    fstrm.open(argv[1], std::ios_base::in | std::ios_base::binary);
    if (!fstrm) {
      LOG(ERROR) << argv[0] << ": Open failed, file = " << argv[1];
      return 1;
    }
    source = argv[1];
  }
  #ifdef _WIN32
    if (source == "standard input") {
        _setmode(_fileno(stdin), _O_BINARY);
    }
  #endif
  std::istream &istrm = fstrm.is_open() ? fstrm : std::cin;

  const SymbolTableTextOptions opts(FST_FLAGS_allow_negative_labels);

  std::unique_ptr<const SymbolTable> isyms;
  if (!FST_FLAGS_isymbols.empty()) {
    isyms.reset(SymbolTable::ReadText(FST_FLAGS_isymbols, opts));
    if (!isyms) return 1;
  }

  std::unique_ptr<const SymbolTable> osyms;
  if (!FST_FLAGS_osymbols.empty()) {
    osyms.reset(SymbolTable::ReadText(FST_FLAGS_osymbols, opts));
    if (!osyms) return 1;
  }

  std::unique_ptr<const SymbolTable> ssyms;
  if (!FST_FLAGS_ssymbols.empty()) {
    ssyms.reset(SymbolTable::ReadText(FST_FLAGS_ssymbols));
    if (!ssyms) return 1;
  }

  const std::string dest = argc > 2 && strcmp(argv[2], "-") != 0 ? argv[2] : "";

  s::Compile(istrm, source, dest, FST_FLAGS_fst_type,
             FST_FLAGS_arc_type, isyms.get(), osyms.get(),
             ssyms.get(), FST_FLAGS_acceptor,
             FST_FLAGS_keep_isymbols,
             FST_FLAGS_keep_osymbols,
             FST_FLAGS_keep_state_numbering,
             FST_FLAGS_allow_negative_labels);

  return 0;
}
