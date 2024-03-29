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

#ifndef FST_GENERIC_REGISTER_H_
#define FST_GENERIC_REGISTER_H_

#include <functional>

#include <fst/compat.h>
#include <string_view>
#include <fst/lock.h>
#ifndef FST_NO_DYNAMIC_LINKING
#include <dlfcn.h>
#endif
#include <map>
#include <string>
#include <typeinfo>

#include <fst/log.h>
#include <fst/exports/exports.h>

// Generic class representing a globally-stored correspondence between
// objects of KeyType and EntryType.
//
// KeyType must:
//
// * be such as can be stored as a key in a std::map<>.
//
// EntryType must be default constructible.
//
// The third template parameter should be the type of a subclass of this class
// (think CRTP). This is to allow GetRegister() to instantiate and return an
// object of the appropriate type.


namespace fst {

namespace internal {
template <class T>
struct KeyLookupReferenceType {
  using type = const T &;
};

template <>
struct KeyLookupReferenceType<std::string> {
  using type = std::string_view;
};
}  // namespace internal

class RegisterBase { };

class Singleton {
public:

    template <class RegisterType>
    std::shared_ptr<RegisterType> GetRegister() {

        std::string type_name = typeid(RegisterType).name();
        if (registry.find(type_name) == registry.end()) {
            std::shared_ptr<RegisterType> r = std::make_shared<RegisterType>();
            registry[type_name] = std::static_pointer_cast <RegisterBase>(r);
        }
        return  std::static_pointer_cast <RegisterType>(registry[type_name]);
    }
private:
    std::map<std::string, std::shared_ptr<RegisterBase>> registry;
};

fst_EXPORT Singleton& GetSingleton();

template <class KeyType, class EntryType, class RegisterType>
class GenericRegister : public RegisterBase {
 public:
  using Key = KeyType;
  using KeyLookupRef = typename internal::KeyLookupReferenceType<KeyType>::type;
  using Entry = EntryType;

  static RegisterType *GetRegister() {
      return GetSingleton().GetRegister<RegisterType>().get();
  }

  void SetEntry(const KeyType &key, const EntryType &entry) {
    MutexLock l(&register_lock_);
    register_table_.emplace(key, entry);
  }

  EntryType GetEntry(KeyLookupRef key) const {
    const auto *entry = LookupEntry(key);
    if (entry) {
      return *entry;
    } else {
      return LoadEntryFromSharedObject(key);
    }
  }

  virtual ~GenericRegister() {}

 protected:
  // Override this if you want to be able to load missing definitions from
  // shared object files.
  virtual EntryType LoadEntryFromSharedObject(KeyLookupRef key) const {
#ifdef FST_NO_DYNAMIC_LINKING
    return EntryType();
#else
    const auto so_filename = ConvertKeyToSoFilename(key);
    void *handle = dlopen(so_filename.c_str(), RTLD_LAZY);
    if (handle == nullptr) {
      LOG(ERROR) << "GenericRegister::GetEntry: " << dlerror();
      return EntryType();
    }
#ifdef RUN_MODULE_INITIALIZERS
    RUN_MODULE_INITIALIZERS();
#endif
    // We assume that the DSO constructs a static object in its global scope
    // that does the registration. Thus we need only load it, not call any
    // methods.
    const auto *entry = this->LookupEntry(key);
    if (entry == nullptr) {
      LOG(ERROR) << "GenericRegister::GetEntry: "
                 << "lookup failed in shared object: " << so_filename;
      return EntryType();
    }
    return *entry;
#endif  // FST_NO_DYNAMIC_LINKING
  }

  // Override this to define how to turn a key into an SO filename.
  virtual std::string ConvertKeyToSoFilename(KeyLookupRef key) const = 0;

  virtual const EntryType *LookupEntry(KeyLookupRef key) const {
    MutexLock l(&register_lock_);
    const auto it = register_table_.find(key);
    if (it != register_table_.end()) {
      return &it->second;
    } else {
      return nullptr;
    }
  }

 private:
  mutable Mutex register_lock_;
  std::map<KeyType, EntryType, std::less<>> register_table_;
};


// Generic register-er class capable of creating new register entries in the
// given RegisterType template parameter. This type must define types Key and
// Entry, and have appropriate static GetRegister() and instance SetEntry()
// functions. An easy way to accomplish this is to have RegisterType be the
// type of a subclass of GenericRegister.
template <class RegisterType>
class GenericRegisterer {
 public:
  using Key = typename RegisterType::Key;
  using Entry = typename RegisterType::Entry;

  GenericRegisterer(Key key, Entry entry) {
      GetSingleton().GetRegister<RegisterType>()->SetEntry(key, entry);
  }
};

}  // namespace fst

#endif  // FST_GENERIC_REGISTER_H_
