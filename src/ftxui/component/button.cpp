#include <functional>  // for function
#include <memory>      // for shared_ptr
#include <utility>     // for move

#include "ftxui/component/animation.hpp"
#include "ftxui/component/captured_mouse.hpp"     // for CapturedMouse
#include "ftxui/component/component.hpp"          // for Make, Button
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for ButtonOption
#include "ftxui/component/event.hpp"              // for Event, Event::Return
#include "ftxui/component/mouse.hpp"  // for Mouse, Mouse::Left, Mouse::Pressed
#include "ftxui/component/screen_interactive.hpp"  // for Component
#include "ftxui/dom/elements.hpp"  // for operator|, Element, nothing, reflect, text, border, inverted
#include "ftxui/screen/box.hpp"  // for Box
#include "ftxui/util/ref.hpp"    // for ConstStringRef, Ref

namespace ftxui {

/// @brief Draw a button. Execute a function when clicked.
/// @param label The label of the button.
/// @param on_click The action to execute when clicked.
/// @param option Additional optional parameters.
/// @ingroup component
/// @see ButtonBase
///
/// ### Example
///
/// ```cpp
/// auto screen = ScreenInteractive::FitComponent();
/// std::string label = "Click to quit";
/// Component button = Button(&label, screen.ExitLoopClosure());
/// screen.Loop(button)
/// ```
///
/// ### Output
///
/// ```bash
/// ┌─────────────┐
/// │Click to quit│
/// └─────────────┘
/// ```
Component Button(ConstStringRef label,
                 std::function<void()> on_click,
                 Ref<ButtonOption> option) {
  class Impl : public ComponentBase {
   public:
    Impl(ConstStringRef label,
         std::function<void()> on_click,
         Ref<ButtonOption> option)
        : label_(label), on_click_(on_click), option_(std::move(option)) {}

    // Component implementation:
    Element Render() override {
      auto style = Focused() ? inverted : nothing;
      auto my_border = option_->border ? border : nothing;
      return text(*label_) | my_border | style | reflect(box_);
    }

    bool OnEvent(Event event) override {
      if (event.is_mouse() && box_.Contain(event.mouse().x, event.mouse().y)) {
        if (!CaptureMouse(event))
          return false;

        TakeFocus();

        if (event.mouse().button == Mouse::Left &&
            event.mouse().motion == Mouse::Pressed) {
          on_click_();
          return true;
        }

        return false;
      }

      if (event == Event::Return) {
        on_click_();
        return true;
      }
      return false;
    }

    bool Focusable() const final { return true; }

   private:
    ConstStringRef label_;
    std::function<void()> on_click_;
    Box box_;
    Ref<ButtonOption> option_;
  };

  return Make<Impl>(label, std::move(on_click), std::move(option));
}

/// @brief Draw a button. Execute a function when clicked.
/// @param label The label of the button.
/// @param on_click The action to execute when clicked.
/// @param option Additional optional parameters.
/// @ingroup component
/// @see ButtonBase
///
/// ### Example
///
/// ```cpp
/// auto screen = ScreenInteractive::FitComponent();
/// std::string label = "Click to quit";
/// Component button = Button(&label, screen.ExitLoopClosure());
/// screen.Loop(button)
/// ```
///
/// ### Output
///
/// ```bash
/// ┌─────────────┐
/// │Click to quit│
/// └─────────────┘
/// ```
Component ButtonAnimated(ConstStringRef label,
                         std::function<void()> on_click,
                         Ref<ButtonAnimatedOption> option) {
  class Impl : public ComponentBase {
   public:
    Impl(ConstStringRef label,
         std::function<void()> on_click,
         Ref<ButtonAnimatedOption> option)
        : label_(label), on_click_(on_click), option_(std::move(option)) {}

    // Component implementation:
    Element Render() override {
      float target = Focused() ? 1.0 : 0.f;
      if (target != animator_.to())
        SetAnimationTarget(target);

      auto style =
          color(Color::Interpolate(animation_, option_->foreground_color,
                                   option_->foreground_color_focused)) |
          bgcolor(Color::Interpolate(animation_, option_->background_color,
                                     option_->background_color_focused));
      auto my_border = borderEmpty;
      return text(*label_) | my_border | style | reflect(box_);
    }

    void SetAnimationTarget(float target) {
      animator_ =
          animation::Animator(&animation_, target, option_->animation_duration,
                              option_->animation_easing);
    }

    void OnAnimation(animation::Params& p) override {
      animator_.OnAnimation(p);
    }

    void OnClick() {
      on_click_();
      animation_ = 0.5f;
      SetAnimationTarget(1.f);
    }

    bool OnEvent(Event event) override {
      if (event.is_mouse() && box_.Contain(event.mouse().x, event.mouse().y)) {
        if (!CaptureMouse(event))
          return false;

        TakeFocus();

        if (event.mouse().button == Mouse::Left &&
            event.mouse().motion == Mouse::Pressed) {
          OnClick();
          return true;
        }

        return false;
      }

      if (event == Event::Return) {
        OnClick();
        return true;
      }
      return false;
    }

    bool Focusable() const final { return true; }

   private:
    ConstStringRef label_;
    std::function<void()> on_click_;
    Box box_;
    Ref<ButtonAnimatedOption> option_;
    float animation_ = 0;
    animation::Animator animator_ = animation::Animator(&animation_);
  };

  return Make<Impl>(label, std::move(on_click), std::move(option));
}

}  // namespace ftxui

// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
