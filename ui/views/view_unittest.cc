// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <map>

#include "base/memory/scoped_ptr.h"
#include "base/rand_util.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/clipboard/clipboard.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/events/event.h"
#include "ui/events/keycodes/keyboard_codes.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/path.h"
#include "ui/gfx/transform.h"
#include "ui/strings/grit/ui_strings.h"

#include "azer/ui/compositor/layer.h"
#include "azer/ui/views/background.h"
#include "azer/ui/views/focus/view_storage.h"
#include "azer/ui/views/test/views_test_base.h"
#include "azer/ui/views/view.h"
#include "azer/ui/views/widget/root_view.h"

using base::ASCIIToUTF16;

namespace {

// Returns true if |ancestor| is an ancestor of |layer|.
bool LayerIsAncestor(const ui::Layer* ancestor, const ui::Layer* layer) {
  while (layer && layer != ancestor)
    layer = layer->parent();
  return layer == ancestor;
}

// Convenience functions for walking a View tree.
const views::View* FirstView(const views::View* view) {
  const views::View* v = view;
  while (v->has_children())
    v = v->child_at(0);
  return v;
}

const views::View* NextView(const views::View* view) {
  const views::View* v = view;
  const views::View* parent = v->parent();
  if (!parent)
    return NULL;
  int next = parent->GetIndexOf(v) + 1;
  if (next != parent->child_count())
    return FirstView(parent->child_at(next));
  return parent;
}

// Convenience functions for walking a Layer tree.
const ui::Layer* FirstLayer(const ui::Layer* layer) {
  const ui::Layer* l = layer;
  while (l->children().size() > 0)
    l = l->children()[0];
  return l;
}

const ui::Layer* NextLayer(const ui::Layer* layer) {
  const ui::Layer* parent = layer->parent();
  if (!parent)
    return NULL;
  const std::vector<ui::Layer*> children = parent->children();
  size_t index;
  for (index = 0; index < children.size(); index++) {
    if (children[index] == layer)
      break;
  }
  size_t next = index + 1;
  if (next < children.size())
    return FirstLayer(children[next]);
  return parent;
}

// Given the root nodes of a View tree and a Layer tree, makes sure the two
// trees are in sync.
bool ViewAndLayerTreeAreConsistent(const views::View* view,
                                   const ui::Layer* layer) {
  const views::View* v = FirstView(view);
  const ui::Layer* l = FirstLayer(layer);
  while (v && l) {
    // Find the view with a layer.
    while (v && !v->layer())
      v = NextView(v);
    EXPECT_TRUE(v);
    if (!v)
      return false;

    // Check if the View tree and the Layer tree are in sync.
    EXPECT_EQ(l, v->layer());
    if (v->layer() != l)
      return false;

    // Check if the visibility states of the View and the Layer are in sync.
    EXPECT_EQ(l->IsDrawn(), v->IsDrawn());
    if (v->IsDrawn() != l->IsDrawn()) {
      for (const views::View* vv = v; vv; vv = vv->parent())
        LOG(ERROR) << "V: " << vv << " " << vv->visible() << " "
                   << vv->IsDrawn() << " " << vv->layer();
      for (const ui::Layer* ll = l; ll; ll = ll->parent())
        LOG(ERROR) << "L: " << ll << " " << ll->IsDrawn();
      return false;
    }

    // Check if the size of the View and the Layer are in sync.
    EXPECT_EQ(l->bounds(), v->bounds());
    if (v->bounds() != l->bounds())
      return false;

    if (v == view || l == layer)
      return v == view && l == layer;

    v = NextView(v);
    l = NextLayer(l);
  }

  return false;
}

// Constructs a View tree with the specified depth.
void ConstructTree(views::View* view, int depth) {
  if (depth == 0)
    return;
  int count = base::RandInt(1, 5);
  for (int i = 0; i < count; i++) {
    views::View* v = new views::View;
    view->AddChildView(v);
    if (base::RandDouble() > 0.5)
      v->SetPaintToLayer(true);
    if (base::RandDouble() < 0.2)
      v->SetVisible(false);

    ConstructTree(v, depth - 1);
  }
}

void ScrambleTree(views::View* view) {
  int count = view->child_count();
  if (count == 0)
    return;
  for (int i = 0; i < count; i++) {
    ScrambleTree(view->child_at(i));
  }

  if (count > 1) {
    int a = base::RandInt(0, count - 1);
    int b = base::RandInt(0, count - 1);

    views::View* view_a = view->child_at(a);
    views::View* view_b = view->child_at(b);
    view->ReorderChildView(view_a, b);
    view->ReorderChildView(view_b, a);
  }

  if (!view->layer() && base::RandDouble() < 0.1)
    view->SetPaintToLayer(true);

  if (base::RandDouble() < 0.1)
    view->SetVisible(!view->visible());
}

}  // namespace

namespace views {

typedef ViewsTestBase ViewTest;

// A derived class for testing purpose.
class TestView : public View {
 public:
  TestView()
      : View(),
        delete_on_pressed_(false),
        native_theme_(NULL),
        can_process_events_within_subtree_(true) {}
  ~TestView() override {}

  // Reset all test state
  void Reset() {
    did_change_bounds_ = false;
    last_mouse_event_type_ = 0;
    location_.SetPoint(0, 0);
    received_mouse_enter_ = false;
    received_mouse_exit_ = false;
    last_clip_.setEmpty();
    accelerator_count_map_.clear();
    can_process_events_within_subtree_ = true;
  }

  // Exposed as public for testing.
  void DoFocus() {
    views::View::Focus();
  }

  void DoBlur() {
    views::View::Blur();
  }

  bool focusable() const { return View::focusable(); }

  void set_can_process_events_within_subtree(bool can_process) {
    can_process_events_within_subtree_ = can_process;
  }

  bool CanProcessEventsWithinSubtree() const override {
    return can_process_events_within_subtree_;
  }

  void OnBoundsChanged(const gfx::Rect& previous_bounds) override;
  bool OnMousePressed(const ui::MouseEvent& event) override;
  bool OnMouseDragged(const ui::MouseEvent& event) override;
  void OnMouseReleased(const ui::MouseEvent& event) override;
  void OnMouseEntered(const ui::MouseEvent& event) override;
  void OnMouseExited(const ui::MouseEvent& event) override;

  void Paint(gfx::Canvas* canvas, const CullSet& cull_set) override;
  void SchedulePaintInRect(const gfx::Rect& rect) override;
  bool AcceleratorPressed(const ui::Accelerator& accelerator) override;

  void OnNativeThemeChanged(const ui::NativeTheme* native_theme) override;

  // OnBoundsChanged.
  bool did_change_bounds_;
  gfx::Rect new_bounds_;

  // MouseEvent.
  int last_mouse_event_type_;
  gfx::Point location_;
  bool received_mouse_enter_;
  bool received_mouse_exit_;
  bool delete_on_pressed_;

  // Painting.
  std::vector<gfx::Rect> scheduled_paint_rects_;

  // Painting.
  SkRect last_clip_;

  // Accelerators.
  std::map<ui::Accelerator, int> accelerator_count_map_;

  // Native theme.
  const ui::NativeTheme* native_theme_;

  // Value to return from CanProcessEventsWithinSubtree().
  bool can_process_events_within_subtree_;
};

////////////////////////////////////////////////////////////////////////////////
// OnBoundsChanged
////////////////////////////////////////////////////////////////////////////////

void TestView::OnBoundsChanged(const gfx::Rect& previous_bounds) {
  did_change_bounds_ = true;
  new_bounds_ = bounds();
}

TEST_F(ViewTest, OnBoundsChanged) {
  TestView v;

  gfx::Rect prev_rect(0, 0, 200, 200);
  gfx::Rect new_rect(100, 100, 250, 250);

  v.SetBoundsRect(prev_rect);
  v.Reset();
  v.SetBoundsRect(new_rect);

  EXPECT_TRUE(v.did_change_bounds_);
  EXPECT_EQ(v.new_bounds_, new_rect);
  EXPECT_EQ(v.bounds(), new_rect);
}
}  // namespace views
