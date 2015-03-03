#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <nan.h>
#include <RInside.h>                    // for the embedded R via RInside

#include "rlink.h"

using namespace v8;

void Initialize(Handle<Object> target) {
  RWrap::Initialize(target);
}

NODE_MODULE(R, Initialize)
