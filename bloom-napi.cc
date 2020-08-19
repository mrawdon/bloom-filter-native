


#include "bloom-napi.h"
#ifdef _use_bf
#include "bf/all.hpp"
#else
#include "bloom.h"
#endif
#include <napi.h>



Napi::FunctionReference Bff::constructor;

Napi::String Method(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, "world");
}

Napi::Object Bff::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "BffFilter", {
    InstanceMethod("add", &Bff::add),
    InstanceMethod("lookup", &Bff::lookup)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Bff", func);
  return exports;
}

Bff::Bff(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Bff>(info)  {
  const Napi::Env env = info.Env();

  if (info.Length() != 2) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsNumber() || !info[1].IsNumber()) {
    Napi::TypeError::New(env, "You must pass the filter size").ThrowAsJavaScriptException();
    return;
  }

  Napi::Number num1 = info[0].As<Napi::Number>();
  Napi::Number num2 = info[1].As<Napi::Number>();
  
#ifdef _use_bf
  this->filter = new bf::basic_bloom_filter(num1.DoubleValue(), num2.Int32Value());
#else
  bloom_init(&this->bloom, num2.Int32Value(), num1.DoubleValue());
#endif
}

Bff::~Bff() {
#ifdef _use_bf
  delete this->filter;
#endif
}

void Bff::add(const Napi::CallbackInfo& info) {
  const Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Only strings are supported").ThrowAsJavaScriptException();
  }

  Napi::String str = info[0].As<Napi::String>();

  // do not add twice, we cannot support unlimited adding
  // it will fail after ~10 add with the same parameter
  // otherwise
#ifdef _use_bf
  size_t lookupVal = this->filter->lookup(str);
  if (lookupVal == 0) {
      this->filter->add(str);
  }
#else
  std::string val = str.Utf8Value();
  bloom_add(&this->bloom, val.c_str(), val.length());
#endif
}

Napi::Value Bff::lookup(const Napi::CallbackInfo& info) {
  const Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Only strings are supported").ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::String str = info[0].As<Napi::String>();

  

  #ifdef _use_bf
  if (this->filter->lookup(str) > 0) {
    return Napi::Boolean::New(env, true);
  } 
#else
  std::string val = str.Utf8Value();
  if(bloom_check(&this->bloom, val.c_str(), val.length()) > 0){
    return Napi::Boolean::New(env, true);
  }
#endif
  return Napi::Boolean::New(env, false);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  return Bff::Init(env, exports);
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)

