#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <iostream>
#include <vector>
#include <algorithm> 

#include "rlink.h"
#include "helper.h"

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
    
  tpl->PrototypeTemplate()->Set(String::NewSymbol("parseEvalQNT"),
    FunctionTemplate::New(parseEvalQNT)->GetFunction());
    
  tpl->PrototypeTemplate()->Set(String::NewSymbol("assign"),
    FunctionTemplate::New(assign)->GetFunction());
    
  tpl->PrototypeTemplate()->Set(String::NewSymbol("get"),
    FunctionTemplate::New(get)->GetFunction());
        
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

Handle<Value> RWrap::parseEvalQNT(const Arguments& args) {
  HandleScope scope;

  RWrap* r = ObjectWrap::Unwrap<RWrap>(args.This());
  RInside* q = r->GetWrapped();
  
  v8::String::Utf8Value param(args[0]->ToString());
  std::string command = std::string(*param);
  
   q->parseEvalQNT(command);
  
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
  else if (args[1]->IsObject()) 
    {
    Handle<Object> object = Handle<Object>::Cast(args[1]); 
    
    Handle<Object> global = Context::GetCurrent()->Global();
    Handle<Object> JSON = Handle<Object>::Cast(global->Get(String::New("JSON")));
    Handle<Function> stringify = Handle<Function>::Cast(
    JSON->Get(String::New("stringify")));
    
    Handle<Value> stringifyable[] = { object };
    Handle<String> result = Handle<String>::Cast(stringify->Call(JSON, 1, stringifyable));
    v8::String::Utf8Value value(result);
    std::string value_str = std::string(*value);
    q->assign(value_str, "JSON_container");
    
    std::string command_pt1 = "require(RJSONIO,quietly=TRUE);";
    std::string command_pt2 = " = fromJSON(JSON_container); rm(JSON_container);";
    std::string full_command = command_pt1 + name + command_pt2; 
    q->parseEvalQ(full_command);
    
    }
  return scope.Close(Undefined());
}

Handle<Value> RWrap::get(const Arguments& args) {
  HandleScope scope;

  RWrap* r = ObjectWrap::Unwrap<RWrap>(args.This());
  RInside* q = r->GetWrapped();
  
  v8::String::Utf8Value param(args[0]->ToString());
  std::string name = std::string(*param);
  std::string command_pt1 = "toJSON(";
  std::string command_pt2 = ");";
  std::string full_command = command_pt1 + name + command_pt2;
  
  Handle<Object> global = Context::GetCurrent()->Global();
  Handle<Object> JSON = Handle<Object>::Cast(global->Get(String::New("JSON")));
  Handle<Function> parse = Handle<Function>::Cast(
  JSON->Get(String::New("parse")));
  
  std::string ret = q->parseEval(full_command);
  Handle<Value> ret_V8 = String::New( ret.c_str() );
  Handle<Value> result = Handle<String>::Cast(parse->Call(JSON, 1, &ret_V8));
  
  return scope.Close(result);
}