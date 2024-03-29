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

#ifndef FST_SYMBOL_TABLE_OPS_H_
#define FST_SYMBOL_TABLE_OPS_H_

#include <cstdint>
#include <string>
#include <vector>


#include <fst/fst.h>
#include <fst/symbol-table.h>
#include <fst/exports/exports.h>

#include <unordered_set>

namespace fst {

// Returns a minimal symbol table containing only symbols referenced by the
// passed fst. Symbols preserve their original numbering, so fst does not
// require relabeling.
template <class Arc>
SymbolTable *PruneSymbolTable(const Fst<Arc> &fst, const SymbolTable &syms,
                              bool input) {
  std::unordered_set<typename Arc::Label> seen;
  seen.insert(0);  // Always keep epsilon.
  for (StateIterator<Fst<Arc>> siter(fst); !siter.Done(); siter.Next()) {
    for (ArcIterator<Fst<Arc>> aiter(fst, siter.Value()); !aiter.Done();
         aiter.Next()) {
      const auto sym = (input) ? aiter.Value().ilabel : aiter.Value().olabel;
      seen.insert(sym);
    }
  }
  auto *pruned = new SymbolTable(syms.Name() + "_pruned");
  for (const auto &stitem : syms) {
    const auto label = stitem.Label();
    if (seen.count(label)) pruned->AddSymbol(stitem.Symbol(), label);
  }
  return pruned;
}

// Relabels a symbol table to make it a contiguous mapping.
SymbolTable fst_EXPORT *CompactSymbolTable(const SymbolTable &syms);

// Merges two SymbolTables, all symbols from left will be merged into right
// with the same IDs. Symbols in right that have conflicting IDs with those
// in left will be assigned to value assigned from the left SymbolTable.
// The returned symbol table will never modify symbol assignments from the left
// side, but may do so on the right. If right_relabel_output is non-null, it
// will be assigned true if the symbols from the right table needed to be
// reassigned.
//
// A potential use case is to compose two FSTs that have different symbol
// tables. You can reconcile them in the following way:
//
//   Fst<Arc> a, b;
//   bool relabel;
//   std::unique_ptr<SymbolTable> bnew(MergeSymbolTable(a.OutputSymbols(),
//                                     b.InputSymbols(), &relabel);
//   if (relabel) Relabel(b, bnew.get(), nullptr);
//   b.SetInputSymbols(bnew);
SymbolTable fst_EXPORT *MergeSymbolTable(const SymbolTable &left, const SymbolTable &right,
                              bool *right_relabel_output = nullptr);

// Read the symbol table from any Fst::Read()able file, without loading the
// corresponding FST. Returns nullptr if the FST does not contain a symbol
// table or the symbol table cannot be read.
SymbolTable fst_EXPORT *FstReadSymbols(const std::string &source, bool input);

// Adds a contiguous range of symbols to a symbol table using a simple prefix
// for the string, returning false if the inserted symbol string clashes with
// any currently present.
bool fst_EXPORT AddAuxiliarySymbols(const std::string &prefix, int64_t start_label,
                         int64_t nlabels, SymbolTable *syms);

}  // namespace fst

#endif  // FST_SYMBOL_TABLE_OPS_H_
