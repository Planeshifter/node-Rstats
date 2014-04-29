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
  
  double value = args[1]->NumberValue();
  q->assign(value, name);

  return scope.Close(Undefined());
}