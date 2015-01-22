// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_TEST_TEST_CURSOR_CLIENT_H_
#define UI_AURA_TEST_TEST_CURSOR_CLIENT_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/observer_list.h"
#include "azer/ui/aura/client/cursor_client.h"

namespace ui {
class KeyEvent;
}

namespace aura {
namespace test {

class TestCursorClient : public aura::client::CursorClient {
 public:
  explicit TestCursorClient(aura::Window* root_window);
  ~TestCursorClient() override;

  // Used to track the number of times SetCursor() was called.
  int calls_to_set_cursor() const { return calls_to_set_cursor_; }
  void reset_calls_to_set_cursor() { calls_to_set_cursor_ = 0; }

  // Set whether or not to hide cursor on key events.
  void set_should_hide_cursor_on_key_event(bool hide) {
    should_hide_cursor_on_key_event_ = hide;
  }

  // Overridden from aura::client::CursorClient:
  void SetCursor(gfx::NativeCursor cursor) override;
  gfx::NativeCursor GetCursor() const override;
  void ShowCursor() override;
  void HideCursor() override;
  void SetCursorSet(ui::CursorSetType cursor_set) override;
  ui::CursorSetType GetCursorSet() const override;
  bool IsCursorVisible() const override;
  void EnableMouseEvents() override;
  void DisableMouseEvents() override;
  bool IsMouseEventsEnabled() const override;
  void SetDisplay(const gfx::Display& display) override;
  void LockCursor() override;
  void UnlockCursor() override;
  bool IsCursorLocked() const override;
  void AddObserver(aura::client::CursorClientObserver* observer) override;
  void RemoveObserver(aura::client::CursorClientObserver* observer) override;
  bool ShouldHideCursorOnKeyEvent(const ui::KeyEvent& event) const override;

 private:
  bool visible_;
  bool should_hide_cursor_on_key_event_;
  bool mouse_events_enabled_;
  int cursor_lock_count_;
  int calls_to_set_cursor_;
  ObserverList<aura::client::CursorClientObserver> observers_;
  aura::Window* root_window_;

  DISALLOW_COPY_AND_ASSIGN(TestCursorClient);
};

}  // namespace test
}  // namespace aura

#endif // UI_AURA_TEST_TEST_CURSOR_CLIENT_H_
