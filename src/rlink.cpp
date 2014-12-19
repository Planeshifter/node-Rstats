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

  tpl->PrototypeTemplate()->Set(String::NewSymbol("parseEval"),
    FunctionTemplate::New(parseEval)->GetFunction());

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

  RInside* q = w->GetWrapped();
  std::string load_command = "library(RJSONIO, quietly=TRUE);";
  q->parseEvalQ(load_command);

  w->Wrap(args.This());

  return args.This();
}


Handle<Value> RWrap::parseEval(const Arguments& args) {
  HandleScope scope;

  RWrap* r = ObjectWrap::Unwrap<RWrap>(args.This());
  RInside* q = r->GetWrapped();

  v8::String::Utf8Value param(args[0]->ToString());
  std::string command = std::string(*param);
  std::string wrapper_before = "toJSON({";
  std::string wrapper_after = "});";
  std::string full_command = wrapper_before + command + wrapper_after;

  Handle<Object> global = Context::GetCurrent()->Global();
  Handle<Object> JSON = Handle<Object>::Cast(global->Get(String::New("JSON")));
  Handle<Function> parse = Handle<Function>::Cast(
	  JSON->Get(String::New("parse")));

  try {
    std::string ret = q->parseEval(full_command);
    Handle<Value> ret_V8 = String::New( ret.c_str() );
    Handle<Value> result = Handle<String>::Cast(parse->Call(JSON, 1, &ret_V8));
    return scope.Close(result);
  } catch(std::exception& ex) {
	std::string errorMessage(ex.what());
	ThrowException(Exception::Error(String::New(errorMessage.c_str())));
    return scope.Close(Undefined());
  } catch(...) {
	ThrowException(Exception::Error(String::New("Unknown error encountered")));
    return scope.Close(Undefined());
  }
}

Handle<Value> RWrap::parseEvalQ(const Arguments& args) {
  HandleScope scope;

  RWrap* r = ObjectWrap::Unwrap<RWrap>(args.This());
  RInside* q = r->GetWrapped();

  v8::String::Utf8Value param(args[0]->ToString());
  std::string command = std::string(*param);

  try {
    q->parseEvalQ(command);
  }
  catch(std::exception& ex) {
	std::string errorMessage(ex.what());
	ThrowException(Exception::Error(String::New(errorMessage.c_str())));
  } catch(...) {
	ThrowException(Exception::Error(String::New("Unknown error encountered")));
  }

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
  else if (args[1]->IsArray() || args[1]->IsObject())
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

    std::string command_pt2 = " = fromJSON(JSON_container); rm(JSON_container);";
    std::string full_command = name + command_pt2;
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
  std::string command_pt2 = ", digits=50);";
  std::string full_command = command_pt1 + name + command_pt2;

  Handle<Object> global = Context::GetCurrent()->Global();
  Handle<Object> JSON = Handle<Object>::Cast(global->Get(String::New("JSON")));
  Handle<Function> parse = Handle<Function>::Cast(
  JSON->Get(String::New("parse")));

  try{
    std::string ret = q->parseEval(full_command);
    Handle<Value> ret_V8 = String::New( ret.c_str() );
    Handle<Value> result = Handle<String>::Cast(parse->Call(JSON, 1, &ret_V8));
    return scope.Close(result);
  } catch(...) {
    ThrowException(Exception::ReferenceError(String::New("The requested variable could not be retrieved.")));
    return scope.Close(Undefined());
  }
}
