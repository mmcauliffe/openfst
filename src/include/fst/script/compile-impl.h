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
// Class to to compile a binary FST from textual input.

#ifndef FST_SCRIPT_COMPILE_IMPL_H_
#define FST_SCRIPT_COMPILE_IMPL_H_

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <fst/fst.h>
#include <fst/string.h>
#include <fst/symbol-table.h>
#include <fst/util.h>
#include <fst/vector-fst.h>
#include <unordered_map>
#include <string_view>
#include <fst/exports/exports.h>

namespace fst {

// Compile a binary FST from textual input, helper class for fstcompile.cc.
// WARNING: Stand-alone use of this class not recommended, most code should
// read/write using the binary format which is much more efficient.
template <class Arc>
class FstCompiler {
 public:
  using Label = typename Arc::Label;
  using StateId = typename Arc::StateId;
  using Weight = typename Arc::Weight;

  // WARNING: use of negative labels not recommended as it may cause conflicts.
  // If add_symbols_ is true, then the symbols will be dynamically added to the
  // symbol tables. This is only useful if you set the (i/o)keep flag to attach
  // the final symbol table, or use the accessors. (The input symbol tables are
  // const and therefore not changed.)
  FstCompiler(std::istream &istrm, const std::string &source,
              const SymbolTable *isyms, const SymbolTable *osyms,
              const SymbolTable *ssyms, bool accep, bool ikeep, bool okeep,
              bool nkeep, bool allow_negative_labels = false) {
    std::unique_ptr<SymbolTable> misyms(isyms ? isyms->Copy() : nullptr);
    std::unique_ptr<SymbolTable> mosyms(osyms ? osyms->Copy() : nullptr);
    std::unique_ptr<SymbolTable> mssyms(ssyms ? ssyms->Copy() : nullptr);
    Init(istrm, source, misyms.get(), mosyms.get(), mssyms.get(), accep, ikeep,
         okeep, nkeep, allow_negative_labels, false);
  }

  FstCompiler(std::istream &istrm, const std::string &source,
              SymbolTable *isyms, SymbolTable *osyms, SymbolTable *ssyms,
              bool accep, bool ikeep, bool okeep, bool nkeep,
              bool allow_negative_labels, bool add_symbols) {
    Init(istrm, source, isyms, osyms, ssyms, accep, ikeep, okeep, nkeep,
         allow_negative_labels, add_symbols);
  }

  void Init(std::istream &istrm, const std::string &source, SymbolTable *isyms,
            SymbolTable *osyms, SymbolTable *ssyms, bool accep, bool ikeep,
            bool okeep, bool nkeep, bool allow_negative_labels,
            bool add_symbols) {
    nline_ = 0;
    source_ = source;
    isyms_ = isyms;
    osyms_ = osyms;
    ssyms_ = ssyms;
    nstates_ = 0;
    keep_state_numbering_ = nkeep;
    allow_negative_labels_ = allow_negative_labels;
    add_symbols_ = add_symbols;
    bool start_state_populated = false;
    char line[kLineLen];
    const std::string separator =
        FST_FLAGS_fst_field_separator + "\n";
    while (istrm.getline(line, kLineLen)) {
      ++nline_;
      std::vector<std::string_view> col =
          StrSplit(line, ByAnyChar(separator), SkipEmpty());
      if (col.empty() || col[0].empty()) continue;
      if (col.size() > 5 || (col.size() > 4 && accep) ||
          (col.size() == 3 && !accep)) {
        FSTERROR() << "FstCompiler: Bad number of columns, source = " << source_
                   << ", line = " << nline_;
        fst_.SetProperties(kError, kError);
        return;
      }
      StateId s = StrToStateId(col[0]);
      while (s >= fst_.NumStates()) fst_.AddState();
      if (!start_state_populated) {
        fst_.SetStart(s);
        start_state_populated = true;
      }

      Arc arc;
      StateId d = s;
      switch (col.size()) {
        case 1:
          fst_.SetFinal(s, Weight::One());
          break;
        case 2:
          fst_.SetFinal(s, StrToWeight(col[1], true));
          break;
        case 3:
          arc.nextstate = d = StrToStateId(col[1]);
          arc.ilabel = StrToILabel(col[2]);
          arc.olabel = arc.ilabel;
          arc.weight = Weight::One();
          fst_.AddArc(s, arc);
          break;
        case 4:
          arc.nextstate = d = StrToStateId(col[1]);
          arc.ilabel = StrToILabel(col[2]);
          if (accep) {
            arc.olabel = arc.ilabel;
            arc.weight = StrToWeight(col[3], true);
          } else {
            arc.olabel = StrToOLabel(col[3]);
            arc.weight = Weight::One();
          }
          fst_.AddArc(s, arc);
          break;
        case 5:
          arc.nextstate = d = StrToStateId(col[1]);
          arc.ilabel = StrToILabel(col[2]);
          arc.olabel = StrToOLabel(col[3]);
          arc.weight = StrToWeight(col[4], true);
          fst_.AddArc(s, arc);
      }
      while (d >= fst_.NumStates()) fst_.AddState();
    }
    if (ikeep) fst_.SetInputSymbols(isyms);
    if (okeep) fst_.SetOutputSymbols(osyms);
  }

  const VectorFst<Arc> &Fst() const { return fst_; }

 private:
  // Maximum line length in text file.
  static constexpr int kLineLen = 8096;

  StateId StrToId(std::string_view s, SymbolTable *syms,
                  std::string_view name, bool allow_negative = false) const {
    StateId n = 0;
    if (syms) {
      n = (add_symbols_) ? syms->AddSymbol(s) : syms->Find(s);
      if (n == kNoSymbol || (!allow_negative && n < 0)) {
        FSTERROR() << "FstCompiler: Symbol \"" << s
                   << "\" is not mapped to any integer " << name
                   << ", symbol table = " << syms->Name()
                   << ", source = " << source_ << ", line = " << nline_;
        fst_.SetProperties(kError, kError);
      }
    } else {
      auto maybe_n = ParseInt64(s);
      if (!maybe_n.has_value() || (!allow_negative && *maybe_n < 0)) {
        FSTERROR() << "FstCompiler: Bad " << name << " integer = \"" << s
                   << "\", source = " << source_ << ", line = " << nline_;
        fst_.SetProperties(kError, kError);
      }
      n = *maybe_n;
    }
    return n;
  }

  StateId StrToStateId(std::string_view s) {
    StateId n = StrToId(s, ssyms_, "state ID");
    if (keep_state_numbering_) return n;
    // Remaps state IDs to make dense set.
    const auto it = states_.find(n);
    if (it == states_.end()) {
      states_[n] = nstates_;
      return nstates_++;
    } else {
      return it->second;
    }
  }

  StateId StrToILabel(std::string_view s) const {
    return StrToId(s, isyms_, "arc ilabel", allow_negative_labels_);
  }

  StateId StrToOLabel(std::string_view s) const {
    return StrToId(s, osyms_, "arc olabel", allow_negative_labels_);
  }

  Weight StrToWeight(std::string_view s, bool allow_zero) const {
    Weight w;
    std::istringstream strm(std::string{s});
    strm >> w;
    if (!strm || (!allow_zero && w == Weight::Zero())) {
      FSTERROR() << "FstCompiler: Bad weight = \"" << s
                 << "\", source = " << source_ << ", line = " << nline_;
      fst_.SetProperties(kError, kError);
      w = Weight::NoWeight();
    }
    return w;
  }

  mutable VectorFst<Arc> fst_;
  size_t nline_;
  std::string source_;  // Text FST source name.
  SymbolTable *isyms_;  // ilabel symbol table (not owned).
  SymbolTable *osyms_;  // olabel symbol table (not owned).
  SymbolTable *ssyms_;  // slabel symbol table (not owned).
  std::unordered_map<StateId, StateId> states_;  // State ID map.
  StateId nstates_;                               // Number of seen states.
  bool keep_state_numbering_;
  bool allow_negative_labels_;  // Not recommended; may cause conflicts.
  bool add_symbols_;            // Add to symbol tables on-the fly.

  FstCompiler(const FstCompiler &) = delete;
  FstCompiler &operator=(const FstCompiler &) = delete;
};

}  // namespace fst

#endif  // FST_SCRIPT_COMPILE_IMPL_H_
