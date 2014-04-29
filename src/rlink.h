#ifndef RLINK_H
#define RLINK_H

#include <node.h>
#include <RInside.h>                    // for the embedded R via RInside
#include <iostream>
#include <vector>
#include <algorithm>

class RWrap : public node::ObjectWrap {
  
  public:  
    static void Initialize(v8::Handle<v8::Object> target);
    RInside* GetWrapped() const { return q_; };
    void SetWrapped(RInside q) {
      if (q_) delete q_;
      q_ = new RInside(q);
      };
      
        static v8::Handle<v8::Value> NewInstance(RInside q);
    
  private:
     RWrap(const v8::Arguments& args);
    ~RWrap();
    static v8::Persistent<v8::Function> constructor;
    static v8::Handle<v8::Value> New(const v8::Arguments& args);

  // Wrapped methods
  static v8::Handle<v8::Value> parseEvalQ(const v8::Arguments& args);
  static v8::Handle<v8::Value> parseEval(const v8::Arguments& args);
  static v8::Handle<v8::Value> assign(const v8::Arguments& args);
  
  // Wrapped object
     RInside* q_;
};


#endif