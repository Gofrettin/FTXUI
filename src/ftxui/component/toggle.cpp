#include <algorithm>   // for max
#include <functional>  // for function
#include <memory>      // for shared_ptr, allocator_traits<>::value_type
#include <utility>     // for move
#include <vector>      // for vector

#include "ftxui/component/captured_mouse.hpp"  // for CapturedMouse
#include "ftxui/component/component.hpp"       // for Make, Toggle
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/component_options.hpp"  // for ToggleOption
#include "ftxui/component/event.hpp"  // for Event, Event::ArrowLeft, Event::ArrowRight, Event::Return, Event::Tab, Event::TabReverse
#include "ftxui/component/mouse.hpp"  // for Mouse, Mouse::Left, Mouse::Pressed
#include "ftxui/dom/elements.hpp"  // for operator|, Element, Elements, hbox, reflect, separator, text, focus, nothing, select
#include "ftxui/screen/box.hpp"    // for Box
#include "ftxui/screen/util.hpp"   // for clamp
#include "ftxui/util/ref.hpp"      // for Ref, ConstStringListRef

namespace ftxui {

namespace {

/// @brief An horizontal list of elements. The user can navigate through them.
/// @ingroup component
class ToggleBase : public ComponentBase {
 public:
  ToggleBase(ConstStringListRef entries, int* selected)
      : entries_(entries), selected_(selected) {}

 protected:
  virtual void OnChange() = 0;
  virtual void OnEnter() = 0;
  virtual int& FocusedEntry() = 0;

  bool OnEvent(Event event) override {
    Clamp();
    if (event.is_mouse())
      return OnMouseEvent(event);

    int old_selected = *selected_;
    if (event == Event::ArrowLeft || event == Event::Character('h'))
      (*selected_)--;
    if (event == Event::ArrowRight || event == Event::Character('l'))
      (*selected_)++;
    if (event == Event::Tab && size())
      *selected_ = (*selected_ + 1) % size();
    if (event == Event::TabReverse && size())
      *selected_ = (*selected_ + size() - 1) % size();

    *selected_ = util::clamp(*selected_, 0, size() - 1);

    if (old_selected != *selected_) {
      FocusedEntry() = *selected_;
      OnChange();
      return true;
    }

    if (event == Event::Return) {
      OnEnter();
      return true;
    }

    return false;
  }

  bool OnMouseEvent(Event event) {
    if (!CaptureMouse(event))
      return false;
    for (int i = 0; i < size(); ++i) {
      if (!boxes_[i].Contain(event.mouse().x, event.mouse().y))
        continue;

      TakeFocus();
      FocusedEntry() = i;
      if (event.mouse().button == Mouse::Left &&
          event.mouse().motion == Mouse::Pressed) {
        TakeFocus();
        if (*selected_ != i) {
          *selected_ = i;
          OnChange();
        }
        return true;
      }
    }
    return false;
  }

  void Clamp() {
    boxes_.resize(size());
    *selected_ = util::clamp(*selected_, 0, size() - 1);
    FocusedEntry() = util::clamp(FocusedEntry(), 0, size() - 1);
  }

  bool Focusable() const final { return size(); }
  int size() const { return entries_.size(); }

  ConstStringListRef entries_;
  int* selected_ = 0;

  std::vector<Box> boxes_;
};

}  // namespace

/// @brief An horizontal list of elements. The user can navigate through them.
/// @param entries The list of selectable entries to display.
/// @param selected Reference the selected entry.
/// @param option Additional optional parameters.
/// @ingroup component
Component Toggle(ConstStringListRef entries,
                 int* selected,
                 Ref<ToggleOption> option) {
  class Impl : public ToggleBase {
   public:
    Impl(ConstStringListRef entries, int* selected, Ref<ToggleOption> option)
        : ToggleBase(std::move(entries), selected),
          option_(std::move(option)) {}

   private:
    void OnChange() override { option_->on_change(); }
    void OnEnter() override { option_->on_enter(); }
    int& FocusedEntry() override { return option_->focused_entry(); }

    Element Render() override {
      Clamp();
      Elements children;
      bool is_toggle_focused = Focused();
      for (int i = 0; i < size(); ++i) {
        // Separator.
        if (i != 0)
          children.push_back(separator());

        bool is_focused = (FocusedEntry() == i) && is_toggle_focused;
        bool is_selected = (*selected_ == i);

        auto style = is_selected ? (is_focused ? option_->style_selected_focused
                                               : option_->style_selected)
                                 : (is_focused ? option_->style_focused
                                               : option_->style_normal);
        auto focus_management = !is_selected        ? nothing
                                : is_toggle_focused ? focus
                                                    : select;
        children.push_back(text(entries_[i]) | style | focus_management |
                           reflect(boxes_[i]));
      }
      return hbox(std::move(children));
    }

    Ref<ToggleOption> option_;
  };
  return Make<Impl>(entries, selected, std::move(option));
}

/// @brief An horizontal list of elements. The user can navigate through them.
/// @param entries The list of selectable entries to display.
/// @param selected Reference the selected entry.
/// @param option Additional optional parameters.
/// @ingroup component
Component ToggleAnimatedUnderline(ConstStringListRef entries,
                                  int* selected,
                                  Ref<ToggleAnimatedUnderlineOption> option) {
  class Impl : public ToggleBase {
   public:
    Impl(ConstStringListRef entries,
         int* selected,
         Ref<ToggleAnimatedUnderlineOption> option)
        : ToggleBase(std::move(entries), selected),
          option_(std::move(option)) {}

   private:
    void OnChange() override { option_->on_change(); }
    void OnEnter() override { option_->on_enter(); }
    int& FocusedEntry() override { return option_->focused_entry(); }

    void OnAnimation(animation::Params& param) override {
      animator_left_.OnAnimation(param);
      animator_right_.OnAnimation(param);
    }

    Element Render() override {
      Clamp();
      Elements children;
      bool is_toggle_focused = Focused();
      children.push_back(option_->element_left());
      for (int i = 0; i < size(); ++i) {
        // Separator.
        if (i != 0)
          children.push_back(option_->element_middle());

        bool is_focused = (FocusedEntry() == i) && is_toggle_focused;
        bool is_selected = (*selected_ == i);

        auto style = is_selected ? (is_focused ? option_->style_selected_focused
                                               : option_->style_selected)
                                 : (is_focused ? option_->style_focused
                                               : option_->style_normal);
        auto focus_management = !is_selected        ? nothing
                                : is_toggle_focused ? focus
                                                    : select;
        children.push_back(text(entries_[i]) | style | focus_management |
                           reflect(boxes_[i]));
      }
      children.push_back(option_->element_right());

      if (LeftTarget() != animator_left_.to() ||
          RightTarget() != animator_right_.to()) {
        if (LeftTarget() >= animator_left_.to()) {
          animator_left_ = animation::Animator(
              &left_, LeftTarget(),
              option_->underline_animation_follower_duration,
              option_->underline_animation_follower_function,
              option_->underline_animation_follower_delay);

          animator_right_ =
              animation::Animator(&right_, RightTarget(),
                                  option_->underline_animation_leader_duration,
                                  option_->underline_animation_leader_function,
                                  option_->underline_animation_leader_delay);
        } else {
          animator_left_ =
              animation::Animator(&left_, LeftTarget(),
                                  option_->underline_animation_leader_duration,
                                  option_->underline_animation_leader_function,
                                  option_->underline_animation_leader_delay);

          animator_right_ = animation::Animator(
              &right_, RightTarget(),
              option_->underline_animation_follower_duration,
              option_->underline_animation_follower_function,
              option_->underline_animation_follower_delay);
        }
      }

      return vbox({
          hbox(std::move(children)),
          separatorHSelector(left_, right_, option_->underline_color_active,
                             option_->underline_color) |
              reflect(box_),
      });
    }

    int LeftTarget() { return boxes_[*selected_].x_min - box_.x_min; }
    int RightTarget() { return boxes_[*selected_].x_max - box_.x_min; }

    float left_ = 0.f;
    float right_ = 0.f;
    animation::Animator animator_left_ = animation::Animator(&left_, 0.f);
    animation::Animator animator_right_ = animation::Animator(&right_, 0.f);
    Ref<ToggleAnimatedUnderlineOption> option_;
    Box box_;
  };
  return Make<Impl>(entries, selected, std::move(option));
}

}  // namespace ftxui

// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
