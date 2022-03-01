#ifndef FTXUI_COMPONENT_COMPONENT_OPTIONS_HPP
#define FTXUI_COMPONENT_COMPONENT_OPTIONS_HPP

#include <ftxui/component/animation.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/util/ref.hpp>

namespace ftxui {

/// @brief Option for the Menu component.
/// @ingroup component
struct MenuOption {
  Decorator style_normal = nothing;    ///< style.
  Decorator style_focused = inverted;  ///< Style when focused.
  Decorator style_selected = bold;     ///< Style when selected.
  Decorator style_selected_focused =
      Decorator(inverted) | bold;  ///< Style when selected and focused.

  /// Called when the selected entry changes.
  std::function<void()> on_change = [] {};
  /// Called when the user presses enter.
  std::function<void()> on_enter = [] {};

  Ref<int> focused_entry = 0;
};

/// @brief Option for the MenuEntry component.
/// @ingroup component
struct MenuEntryOption {
  Decorator style_normal = nothing;    ///< style.
  Decorator style_focused = inverted;  ///< Style when focused.
  Decorator style_selected = bold;     ///< Style when selected.
  Decorator style_selected_focused =
      Decorator(inverted) | bold;  ///< Style when selected and focused.
};

/// @brief Option for the MenuEntryAnimated component.
/// @ingroup component
struct MenuEntryAnimatedOption {
  Color foreground_color_focused = Color::White;
  Color foreground_color = Color::GrayLight;
  animation::easing::Function foreground_color_animation_easing =
      animation::easing::QuadraticOut;
  animation::Duration foreground_color_animation_duration =
      std::chrono::milliseconds(250);

  Color background_color_focused= Color::GrayDark;
  Color background_color = Color::Black;
  animation::easing::Function background_color_animation_easing =
      animation::easing::QuadraticOut;
  animation::Duration background_color_animation_duration =
      std::chrono::milliseconds(250);
};

/// @brief Option for the AnimatedButton component.
/// @ingroup component
struct ButtonOption {
  /// Whether to show a border around the button.
  bool border = true;
};

/// @brief Option for the ButtonAnimatedOption component.
/// @ingroup component
struct ButtonAnimatedOption {
  std::optional<BorderStyle> border = LIGHT;
  Color foreground_color_focused = Color::White;
  Color foreground_color = Color::GrayLight;

  Color background_color_focused= Color::GrayDark;
  Color background_color = Color::Black;
  animation::easing::Function animation_easing =
      animation::easing::QuadraticOut;
  animation::Duration animation_duration = std::chrono::milliseconds(200);
};

/// @brief Option for the Checkbox component.
/// @ingroup component
struct CheckboxOption {
  std::string style_checked = "▣ ";    ///< Prefix for a "checked" state.
  std::string style_unchecked = "☐ ";  ///< Prefix for a "unchecked" state.
  Decorator style_normal = nothing;    ///< style.
  Decorator style_focused = inverted;  ///< Style when focused.
  Decorator style_selected = bold;     ///< Style when selected.
  Decorator style_selected_focused =
      Decorator(inverted) | bold;  ///< Style when selected and focused.

  /// Called when the user change the state.
  std::function<void()> on_change = []() {};
};

/// @brief Option for the Input component.
/// @ingroup component
struct InputOption {
  /// Called when the content changes.
  std::function<void()> on_change = [] {};
  /// Called when the user presses enter.
  std::function<void()> on_enter = [] {};

  /// Obscure the input content using '*'.
  Ref<bool> password = false;

  /// When set different from -1, this attributes is used to store the cursor
  /// position.
  Ref<int> cursor_position = -1;
};

/// @brief Option for the Radiobox component.
/// @ingroup component
struct RadioboxOption {
  std::string style_checked = "◉ ";    ///< Prefix for a "checked" state.
  std::string style_unchecked = "○ ";  ///< Prefix for a "unchecked" state.
  Decorator style_normal = nothing;    ///< style.
  Decorator style_focused = inverted;  ///< Style when focused.
  Decorator style_selected = bold;     ///< Style when selected.
  Decorator style_selected_focused =
      Decorator(inverted) | bold;  ///< Style when selected and focused.

  /// Called when the selected entry changes.
  std::function<void()> on_change = []() {};

  Ref<int> focused_entry = 0;
};

/// @brief Option for the Toggle component.
/// @ingroup component
struct ToggleOption {
  Decorator style_normal = nothing;    ///< style.
  Decorator style_focused = inverted;  ///< Style when focused.
  Decorator style_selected = bold;     ///< Style when selected.
  Decorator style_selected_focused =
      Decorator(inverted) | bold;  ///< Style when selected and focused.

  /// Called when the selected entry changes.
  std::function<void()> on_change = [] {};
  /// Called when the user presses enter.
  std::function<void()> on_enter = [] {};

  Ref<int> focused_entry = 0;
};

/// @brief Option for the Toggle component.
/// @ingroup component
struct ToggleAnimatedUnderlineOption : ToggleOption {
  Color underline_color = Color::GrayDark;
  Color underline_color_active = Color::White;

  animation::easing::Function underline_animation_leader_function =
      animation::easing::QuadraticInOut;
  animation::Duration underline_animation_leader_duration =
      std::chrono::milliseconds(200);
  animation::Duration underline_animation_leader_delay =
      std::chrono::milliseconds(0);

  animation::easing::Function underline_animation_follower_function =
      animation::easing::QuadraticInOut;
  animation::Duration underline_animation_follower_duration =
      std::chrono::milliseconds(200);
  animation::Duration underline_animation_follower_delay =
      std::chrono::milliseconds(0);

  std::function<Element()> element_left = [] { return emptyElement(); };
  std::function<Element()> element_middle = [] { return text(" "); };
  std::function<Element()> element_right = [] { return emptyElement(); };

  void SetUnderlineAnimation(animation::Duration d,
                             animation::easing::Function f) {
    SetUnderlineAnimationDuration(d);
    SetUnderlineAnimationFunction(f);
  }

  void SetUnderlineAnimationDuration(animation::Duration d) {
    underline_animation_leader_duration = d;
    underline_animation_follower_duration = d;
  }

  void SetUnderlineAnimationFunction(animation::easing::Function f) {
    underline_animation_leader_function = f;
    underline_animation_follower_function = f;
  }

  void SetUnderlineAnimationFunction(animation::easing::Function f_leader,
                                     animation::easing::Function f_follower) {
    underline_animation_leader_function = f_leader;
    underline_animation_follower_function = f_follower;
  }
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_COMPONENT_COMPONENT_OPTIONS_HPP */

// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
