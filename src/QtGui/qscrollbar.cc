// Copyright (c) 2012, Artur Adib
// All rights reserved.
//
// Author(s): Artur Adib <aadib@mozilla.com>
//
// You may use this file under the terms of the New BSD license as follows:
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Artur Adib nor the
//       names of contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ARTUR ADIB BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <node.h>
#include "qscrollbar.h"

using namespace v8;

Persistent<Function> QScrollBarWrap::constructor;

QScrollBarWrap::QScrollBarWrap(_NAN_METHOD_ARGS) : q_(NULL) {
}

QScrollBarWrap::~QScrollBarWrap() {
  // Since q_ is never new'd (it's always a pointer to an existing scrollbar),
  // don't delete it! It'll segfault.
}

void QScrollBarWrap::Initialize(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("QScrollBar"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("value"),
      FunctionTemplate::New(Value)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("setValue"),
      FunctionTemplate::New(SetValue)->GetFunction());

  NanAssignPersistent(Function, constructor, tpl->GetFunction());
  target->Set(String::NewSymbol("QScrollBar"), tpl->GetFunction());
}

NAN_METHOD(QScrollBarWrap::New) {
  NanScope();

  QScrollBarWrap* w = new QScrollBarWrap(args);
  w->Wrap(args.This());

  NanReturnValue(args.This());
}

Handle<Value> QScrollBarWrap::NewInstance(QScrollBar *q) {
  NanScope();

  Local<Object> instance = NanPersistentToLocal(constructor)->NewInstance(0, NULL);
  QScrollBarWrap* w = node::ObjectWrap::Unwrap<QScrollBarWrap>(instance);
  w->SetWrapped(q);

  return scope.Close(instance);
}

NAN_METHOD(QScrollBarWrap::Value) {
  NanScope();

  QScrollBarWrap* w = ObjectWrap::Unwrap<QScrollBarWrap>(args.This());
  QScrollBar* q = w->GetWrapped();

  NanReturnValue(Integer::New(q->value()));
}

NAN_METHOD(QScrollBarWrap::SetValue) {
  NanScope();

  QScrollBarWrap* w = ObjectWrap::Unwrap<QScrollBarWrap>(args.This());
  QScrollBar* q = w->GetWrapped();

  q->setValue(args[0]->IntegerValue());

  NanReturnValue(Undefined());
}
