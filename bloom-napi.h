#ifndef BLOOM_NAPI_H
#define BLOOM_NAPI_H
//#define _use_bf 1

#include <napi.h>
#ifdef _use_bf
#include "bf/all.hpp"
#else
#include "bloom.h"
#endif

class Bff : public Napi::ObjectWrap<Bff> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  Bff(const Napi::CallbackInfo& info);
  ~Bff();

 private:
  void add(const Napi::CallbackInfo& info);
  Napi::Value lookup(const Napi::CallbackInfo& info);

  static Napi::FunctionReference constructor;
  #ifdef _use_bf
  bf::basic_bloom_filter *filter;
  #else 
  struct bloom bloom;
  #endif
};

#endif
