#include <node.h>
#include <v8.h>
#include <RInside.h>                    // for the embedded R via RInside
using namespace v8;

Handle<Value> Method(const Arguments& args) {
  HandleScope scope;
  int argc = args.Length();
  
  const char* argv = *v8::String::Utf8Value(args[0]->ToString());
  char *argv_init[argc];
  
  RInside R(argc, argv_init);              // create an embedded R instance 

  R["txt"] = "Hello, world!\n";  // assign a char* (string) to 'txt' 
  R.parseEvalQ("cat(txt)");           // eval the init string, ignoring any returns

  return scope.Close(String::New("world"));
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("hello"),
      FunctionTemplate::New(Method)->GetFunction());
}

NODE_MODULE(R, init)
