#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <iostream>
#include <vector>
#include <algorithm> 

#include "rlink.h"

using namespace v8;
using namespace std;

Persistent<Function> RWrap::constructor;

// Supported implementations:
// mat ( )

RWrap::RWrap(const Arguments& args) : q_(NULL) {
  
    q_ = new RInside();
  
}

RWrap::~RWrap() {
  delete q_;
}


void RWrap::Initialize(Handle<Object> target) {
   // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("session"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1); 
  
  tpl->PrototypeTemplate()->Set(String::NewSymbol("parseEvalQ"),
    FunctionTemplate::New(parseEvalQ)->GetFunction());
    
  tpl->PrototypeTemplate()->Set(String::NewSymbol("parseEval"),
    FunctionTemplate::New(parseEval)->GetFunction());
    
  tpl->PrototypeTemplate()->Set(String::NewSymbol("assign"),
    FunctionTemplate::New(assign)->GetFunction());
      
  
  // Prototype
  
  constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewSymbol("session"), constructor);
}


Handle<Value> RWrap::New(const Arguments& args) {
  HandleScope scope;

  RWrap* w = new RWrap(args);
  w->Wrap(args.This());

  return args.This();
}

Handle<Value> RWrap::parseEvalQ(const Arguments& args) {
  HandleScope scope;

  RWrap* r = ObjectWrap::Unwrap<RWrap>(args.This());
  RInside* q = r->GetWrapped();
  
  v8::String::Utf8Value param(args[0]->ToString());
  std::string command = std::string(*param);
  
  q->parseEvalQ(command);
  
  return scope.Close(Undefined());
}

Handle<Value> RWrap::parseEval(const Arguments& args) {
  HandleScope scope;

  RWrap* r = ObjectWrap::Unwrap<RWrap>(args.This());
  RInside* q = r->GetWrapped();
  
  v8::String::Utf8Value param(args[0]->ToString());
  std::string command = std::string(*param);
  
  return scope.Close(Undefined());
}

Handle<Value> RWrap::assign(const Arguments& args) {
  HandleScope scope;

  RWrap* r = ObjectWrap::Unwrap<RWrap>(args.This());
  RInside* q = r->GetWrapped();
  v8::String::Utf8Value param1(args[0]->ToString());
  std::string name = std::string(*param1);
  
  if (args[1]->IsNumber())
    {
    double value = args[1]->NumberValue();
    q->assign(value, name);
    }
  else if (args[1]->IsString())
    {
    v8::String::Utf8Value value(args[1]->ToString());
    std::string value_str = std::string(*value);
    q->assign(value_str, name);
    }
  else if (args[1]->IsArray())
    {
    v8::Handle<v8::Array> arr = v8::Handle<v8::Array>::Cast(args[1]); 
    
    int length=arr->Length();
    std::vector<double> num_vec(length); 
    
    for (int i=0;i<length;i++) 
      {
      num_vec[i] = arr->Get(i)->NumberValue();
      }
    q->assign(num_vec, name);
    }
  return scope.Close(Undefined());
}