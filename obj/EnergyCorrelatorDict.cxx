// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedImiyagudIePICmIEECmIsimsdIobjdIEnergyCorrelatorDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// The generated code does not explicitly qualifies STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "HistoManager.h"
#include "Particle.h"
#include "Jet.h"
#include "EnergyCorrelator.h"

// Header files passed via #pragma extra_include

namespace HistoManager {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *HistoManager_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("HistoManager", 0 /*version*/, "HistoManager.h", 12,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &HistoManager_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *HistoManager_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static TClass *Particle_Dictionary();
   static void Particle_TClassManip(TClass*);
   static void *new_Particle(void *p = 0);
   static void *newArray_Particle(Long_t size, void *p);
   static void delete_Particle(void *p);
   static void deleteArray_Particle(void *p);
   static void destruct_Particle(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Particle*)
   {
      ::Particle *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Particle));
      static ::ROOT::TGenericClassInfo 
         instance("Particle", "Particle.h", 6,
                  typeid(::Particle), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Particle_Dictionary, isa_proxy, 4,
                  sizeof(::Particle) );
      instance.SetNew(&new_Particle);
      instance.SetNewArray(&newArray_Particle);
      instance.SetDelete(&delete_Particle);
      instance.SetDeleteArray(&deleteArray_Particle);
      instance.SetDestructor(&destruct_Particle);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Particle*)
   {
      return GenerateInitInstanceLocal((::Particle*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Particle*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Particle_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Particle*)0x0)->GetClass();
      Particle_TClassManip(theClass);
   return theClass;
   }

   static void Particle_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *Jet_Dictionary();
   static void Jet_TClassManip(TClass*);
   static void *new_Jet(void *p = 0);
   static void *newArray_Jet(Long_t size, void *p);
   static void delete_Jet(void *p);
   static void deleteArray_Jet(void *p);
   static void destruct_Jet(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Jet*)
   {
      ::Jet *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Jet));
      static ::ROOT::TGenericClassInfo 
         instance("Jet", "Jet.h", 7,
                  typeid(::Jet), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Jet_Dictionary, isa_proxy, 4,
                  sizeof(::Jet) );
      instance.SetNew(&new_Jet);
      instance.SetNewArray(&newArray_Jet);
      instance.SetDelete(&delete_Jet);
      instance.SetDeleteArray(&deleteArray_Jet);
      instance.SetDestructor(&destruct_Jet);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Jet*)
   {
      return GenerateInitInstanceLocal((::Jet*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Jet*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Jet_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Jet*)0x0)->GetClass();
      Jet_TClassManip(theClass);
   return theClass;
   }

   static void Jet_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *EnergyCorrelator_Dictionary();
   static void EnergyCorrelator_TClassManip(TClass*);
   static void delete_EnergyCorrelator(void *p);
   static void deleteArray_EnergyCorrelator(void *p);
   static void destruct_EnergyCorrelator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EnergyCorrelator*)
   {
      ::EnergyCorrelator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::EnergyCorrelator));
      static ::ROOT::TGenericClassInfo 
         instance("EnergyCorrelator", "EnergyCorrelator.h", 16,
                  typeid(::EnergyCorrelator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &EnergyCorrelator_Dictionary, isa_proxy, 4,
                  sizeof(::EnergyCorrelator) );
      instance.SetDelete(&delete_EnergyCorrelator);
      instance.SetDeleteArray(&deleteArray_EnergyCorrelator);
      instance.SetDestructor(&destruct_EnergyCorrelator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EnergyCorrelator*)
   {
      return GenerateInitInstanceLocal((::EnergyCorrelator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::EnergyCorrelator*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *EnergyCorrelator_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::EnergyCorrelator*)0x0)->GetClass();
      EnergyCorrelator_TClassManip(theClass);
   return theClass;
   }

   static void EnergyCorrelator_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_Particle(void *p) {
      return  p ? new(p) ::Particle : new ::Particle;
   }
   static void *newArray_Particle(Long_t nElements, void *p) {
      return p ? new(p) ::Particle[nElements] : new ::Particle[nElements];
   }
   // Wrapper around operator delete
   static void delete_Particle(void *p) {
      delete ((::Particle*)p);
   }
   static void deleteArray_Particle(void *p) {
      delete [] ((::Particle*)p);
   }
   static void destruct_Particle(void *p) {
      typedef ::Particle current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Particle

namespace ROOT {
   // Wrappers around operator new
   static void *new_Jet(void *p) {
      return  p ? new(p) ::Jet : new ::Jet;
   }
   static void *newArray_Jet(Long_t nElements, void *p) {
      return p ? new(p) ::Jet[nElements] : new ::Jet[nElements];
   }
   // Wrapper around operator delete
   static void delete_Jet(void *p) {
      delete ((::Jet*)p);
   }
   static void deleteArray_Jet(void *p) {
      delete [] ((::Jet*)p);
   }
   static void destruct_Jet(void *p) {
      typedef ::Jet current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Jet

namespace ROOT {
   // Wrapper around operator delete
   static void delete_EnergyCorrelator(void *p) {
      delete ((::EnergyCorrelator*)p);
   }
   static void deleteArray_EnergyCorrelator(void *p) {
      delete [] ((::EnergyCorrelator*)p);
   }
   static void destruct_EnergyCorrelator(void *p) {
      typedef ::EnergyCorrelator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::EnergyCorrelator

namespace ROOT {
   static TClass *vectorlEParticlegR_Dictionary();
   static void vectorlEParticlegR_TClassManip(TClass*);
   static void *new_vectorlEParticlegR(void *p = 0);
   static void *newArray_vectorlEParticlegR(Long_t size, void *p);
   static void delete_vectorlEParticlegR(void *p);
   static void deleteArray_vectorlEParticlegR(void *p);
   static void destruct_vectorlEParticlegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Particle>*)
   {
      vector<Particle> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Particle>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Particle>", -2, "vector", 216,
                  typeid(vector<Particle>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEParticlegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<Particle>) );
      instance.SetNew(&new_vectorlEParticlegR);
      instance.SetNewArray(&newArray_vectorlEParticlegR);
      instance.SetDelete(&delete_vectorlEParticlegR);
      instance.SetDeleteArray(&deleteArray_vectorlEParticlegR);
      instance.SetDestructor(&destruct_vectorlEParticlegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Particle> >()));

      ::ROOT::AddClassAlternate("vector<Particle>","std::vector<Particle, std::allocator<Particle> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<Particle>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEParticlegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<Particle>*)0x0)->GetClass();
      vectorlEParticlegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEParticlegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEParticlegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Particle> : new vector<Particle>;
   }
   static void *newArray_vectorlEParticlegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Particle>[nElements] : new vector<Particle>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEParticlegR(void *p) {
      delete ((vector<Particle>*)p);
   }
   static void deleteArray_vectorlEParticlegR(void *p) {
      delete [] ((vector<Particle>*)p);
   }
   static void destruct_vectorlEParticlegR(void *p) {
      typedef vector<Particle> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<Particle>

namespace {
  void TriggerDictionaryInitialization_EnergyCorrelatorDict_Impl() {
    static const char* headers[] = {
"HistoManager.h",
"Particle.h",
"Jet.h",
"EnergyCorrelator.h",
0
    };
    static const char* includePaths[] = {
"/home/miyagu/ePIC-EEC-sims/include",
"/home/aidalagroup/root_w_pythia/root6/include/",
"/home/miyagu/ePIC-EEC-sims/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "EnergyCorrelatorDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$Particle.h")))  Particle;
class __attribute__((annotate("$clingAutoload$Jet.h")))  Jet;
class __attribute__((annotate("$clingAutoload$EnergyCorrelator.h")))  EnergyCorrelator;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "EnergyCorrelatorDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "HistoManager.h"
#include "Particle.h"
#include "Jet.h"
#include "EnergyCorrelator.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"EnergyCorrelator", payloadCode, "@",
"Jet", payloadCode, "@",
"Particle", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("EnergyCorrelatorDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_EnergyCorrelatorDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_EnergyCorrelatorDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_EnergyCorrelatorDict() {
  TriggerDictionaryInitialization_EnergyCorrelatorDict_Impl();
}
