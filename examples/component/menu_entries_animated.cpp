#include <iostream>  // for basic_ostream::operator<<, operator<<, endl, basic_ostream, basic_ostream<>::__ostream_type, cout, ostream
#include <memory>    // for shared_ptr, __shared_ptr_access
#include <string>    // for to_string, allocator

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for MenuEntryAnimated, Renderer, Vertical
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/component_options.hpp"   // for MenuEntryAnimated
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for operator|, separator, Element, Decorator, color, text, hbox, size, bold, frame, inverted, vbox, HEIGHT, LESS_THAN, border
#include "ftxui/screen/color.hpp"  // for Color, Color::Blue, Color::Cyan, Color::Green, Color::Red, Color::Yellow

using namespace ftxui;

// Define a special style for some menu entry.
MenuEntryAnimatedOption Colored(ftxui::Color c) {
  MenuEntryAnimatedOption style;
  style.background_color = Color::Black;
  style.background_color_focused = c;
  style.foreground_color = c;
  style.foreground_color_focused = Color::White;
  return style;
}

int main(int argc, const char* argv[]) {
  auto screen = ScreenInteractive::TerminalOutput();

  int selected = 0;
  auto menu = Container::Vertical(
      {
          MenuEntryAnimated(" 1. rear", Colored(Color::Red)),
          MenuEntryAnimated(" 2. drown", Colored(Color::Yellow)),
          MenuEntryAnimated(" 3. nail", Colored(Color::Green)),
          MenuEntryAnimated(" 4. quit", Colored(Color::Cyan)),
          MenuEntryAnimated(" 5. decorative", Colored(Color::Blue)),
          MenuEntryAnimated(" 7. costume"),
          MenuEntryAnimated(" 8. pick"),
          MenuEntryAnimated(" 9. oral"),
          MenuEntryAnimated("11. minister"),
          MenuEntryAnimated("12. football"),
          MenuEntryAnimated("13. welcome"),
          MenuEntryAnimated("14. copper"),
          MenuEntryAnimated("15. inhabitant"),
      },
      &selected);

  // Display together the menu with a border
  auto renderer = Renderer(menu, [&] {
    return vbox({
               hbox(text("selected = "), text(std::to_string(selected))),
               separator(),
               menu->Render() | frame,
           }) |
           border | bgcolor(Color::Black);
  });

  screen.Loop(renderer);

  std::cout << "Selected element = " << selected << std::endl;
}

// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
