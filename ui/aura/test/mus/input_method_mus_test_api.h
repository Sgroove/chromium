// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_TEST_MUS_INPUT_METHOD_MUS_TEST_API_H_
#define UI_AURA_TEST_MUS_INPUT_METHOD_MUS_TEST_API_H_

#include "base/macros.h"
#include "ui/aura/mus/input_method_mus.h"

namespace aura {

class InputMethodMusTestApi {
 public:
  static void SetInputMethod(InputMethodMus* input_method_mus,
                             ui::mojom::InputMethod* input_method) {
    input_method_mus->input_method_ = input_method;
  }

  static void CallSendKeyEventToInputMethod(
      InputMethodMus* input_method_mus,
      const ui::KeyEvent& event,
      std::unique_ptr<InputMethodMus::EventResultCallback> ack_callback) {
    input_method_mus->SendKeyEventToInputMethod(event, std::move(ack_callback));
  }

  static void Disable(InputMethodMus* input_method) {
    DCHECK(input_method->pending_callbacks_.empty());
    input_method->ime_server_.reset();
  }

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(InputMethodMusTestApi);
};

}  // namespace aura

#endif  // UI_AURA_TEST_MUS_INPUT_METHOD_MUS_TEST_API_H_
