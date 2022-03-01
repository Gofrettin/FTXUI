#include <memory>  // for allocator, __shared_ptr_access, shared_ptr
#include <string>  // for string, basic_string
#include <vector>  // for vector

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Radiobox, Renderer, Tab, Toggle, Vertical
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for Element, separator, operator|, vbox, border

using namespace ftxui;

Component DummyComponent(int id) {
  return Renderer([id](bool focused) {
    auto t = text("component " + std::to_string(id));
    if (focused)
      t = t | inverted;
    return t;
  });
}

Component Text(const std::string& t) {
  return Renderer([t] { return text(t) | borderEmpty; });
}

int main(int argc, const char* argv[]) {
  std::vector<std::string> tab_values{
      "Tab 1",
      "Tab 2",
      "Tab 3",
      "A very very long tab",
      "탭",
  };
  int tab_selected = 0;

  auto container = Container::Vertical({});

  {
    container->Add(
        Text("This demonstrate the ToggleAnimatedUnderline component"));
    container->Add(ToggleAnimatedUnderline(&tab_values, &tab_selected));
  }

  {
    container->Add(Text("Set underline color to red"));
    ToggleAnimatedUnderlineOption option;
    option.underline_color_active = Color::Red;
    container->Add(ToggleAnimatedUnderline(&tab_values, &tab_selected, option));
  }

  {
    container->Add(Text("Set underline active color to blue"));
    ToggleAnimatedUnderlineOption option;
    option.underline_color = Color::Blue;
    container->Add(ToggleAnimatedUnderline(&tab_values, &tab_selected, option));
  }

  {
    container->Add(Text("Set animation duration to 0ms"));
    ToggleAnimatedUnderlineOption option;
    option.SetUnderlineAnimationDuration(std::chrono::milliseconds(0));
    container->Add(ToggleAnimatedUnderline(&tab_values, &tab_selected, option));
  }

  {
    container->Add(Text("Set animation easing function to back-out"));
    ToggleAnimatedUnderlineOption option;
    option.SetUnderlineAnimationFunction(animation::easing::BackOut);
    option.SetUnderlineAnimationDuration(std::chrono::milliseconds(350));
    container->Add(ToggleAnimatedUnderline(&tab_values, &tab_selected, option));
  }

  // option.underline_animation_follower_delay = 250ms
  {
    container->Add(Text("Add delay to desynchronize animation"));
    ToggleAnimatedUnderlineOption option;
    option.underline_animation_follower_delay = std::chrono::milliseconds(250);
    container->Add(ToggleAnimatedUnderline(&tab_values, &tab_selected, option));
  }

  container->Add(Container::Tab(
      {
          DummyComponent(1),
          DummyComponent(2),
          DummyComponent(3),
          DummyComponent(4),
          DummyComponent(5),
          DummyComponent(6),
          DummyComponent(7),
          DummyComponent(8),
          DummyComponent(9),
          DummyComponent(10),
          DummyComponent(11),
          DummyComponent(12),
          DummyComponent(13),
      },
      &tab_selected));

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(container);
}

// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
