#ifndef RLINK_H
#define RLINK_H

#include <node.h>
#include <nan.h>
#include <RInside.h>                    // for the embedded R via RInside
#include <Callbacks.h>
#include <iostream>
#include <vector>
#include <algorithm>

#if !defined(RINSIDE_CALLBACKS)
#error "RInside was not compiled with RINSIDE_CALLBACKS"
#endif
  
class RWrap : public node::ObjectWrap {
  friend class RWrapCallbacks;
  
  public:
    static void Initialize(v8::Handle<v8::Object> target);
    RInside* GetWrapped() const { return q_; };
    void SetWrapped(RInside q) {
      if (q_) delete q_;
      q_ = new RInside(q);
      };

        static v8::Handle<v8::Value> NewInstance(RInside q);

  private:
     RWrap();
    ~RWrap();
    static Nan::Persistent<v8::FunctionTemplate> constructor_template;
    static NAN_METHOD(New);

    // Wrapped methods
    static NAN_METHOD(setCallbackShowMessage);
    static NAN_METHOD(setCallbackSuicide);
    static NAN_METHOD(setCallbackWriteConsole);
    static NAN_METHOD(parseEval);
    static NAN_METHOD(parseEvalQ);
    static NAN_METHOD(parseEvalQNT);
    static NAN_METHOD(assign);
    static NAN_METHOD(get);

    // Wrapped callbacks, NodeJS side
    Nan::Persistent<v8::Function> cb_ShowMessage;
    Nan::Persistent<v8::Function> cb_Suicide;
    Nan::Persistent<v8::Function> cb_WriteConsole;
    
    // Callbacks class instance
    class RWrapCallbacks * callbacks_;
    
    // Wrapped object
    RInside* q_;
};

class RWrapCallbacks : public Callbacks {
  public:
    RWrapCallbacks(class RWrap* parent)
    {
      _parent = parent;
    }
    
    // see inst/includes/Callbacks.h for a list of all overrideable methods
    virtual void WriteConsole(const std::string& line, int type);
    virtual void ShowMessage(const char* message);
    virtual void Suicide(const char* message);
    
    virtual bool has_WriteConsole() { return true; };
    virtual bool has_ShowMessage() { return true; };
    virtual bool has_Suicide() { return true; };
  private:
    class RWrap * _parent;
};

#endif
