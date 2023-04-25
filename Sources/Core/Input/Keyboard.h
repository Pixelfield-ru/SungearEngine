//
// Created by stuka on 26.04.2023.
//

#ifndef NATIVECORE_KEYBOARD_H
#define NATIVECORE_KEYBOARD_H

#include <GLFW/glfw3.h>
#include "../Main/Core.h"

struct SGKey
{
    int keyCode;
};

constexpr inline const SGKey KEY_SPACE { GLFW_KEY_SPACE };
constexpr inline const SGKey KEY_APOSTROPHE { GLFW_KEY_APOSTROPHE };
constexpr inline const SGKey KEY_COMMA { GLFW_KEY_COMMA };
constexpr inline const SGKey KEY_MINUS { GLFW_KEY_MINUS };
constexpr inline const SGKey KEY_PERIOD { GLFW_KEY_PERIOD };
constexpr inline const SGKey KEY_SLASH { GLFW_KEY_SLASH };
constexpr inline const SGKey KEY_0 { GLFW_KEY_0 };
constexpr inline const SGKey KEY_1 { GLFW_KEY_1 };
constexpr inline const SGKey KEY_2 { GLFW_KEY_2 };
constexpr inline const SGKey KEY_3 { GLFW_KEY_3 };
constexpr inline const SGKey KEY_4 { GLFW_KEY_4 };
constexpr inline const SGKey KEY_5 { GLFW_KEY_5 };
constexpr inline const SGKey KEY_6 { GLFW_KEY_6 };
constexpr inline const SGKey KEY_7 { GLFW_KEY_7 };
constexpr inline const SGKey KEY_8 { GLFW_KEY_8 };
constexpr inline const SGKey KEY_9 { GLFW_KEY_9 };
constexpr inline const SGKey KEY_SEMICOLON { GLFW_KEY_SEMICOLON };
constexpr inline const SGKey KEY_EQUAL { GLFW_KEY_EQUAL };
constexpr inline const SGKey KEY_A { GLFW_KEY_A };
constexpr inline const SGKey KEY_B { GLFW_KEY_B };
constexpr inline const SGKey KEY_C { GLFW_KEY_C };
constexpr inline const SGKey KEY_D { GLFW_KEY_D };
constexpr inline const SGKey KEY_E { GLFW_KEY_E };
constexpr inline const SGKey KEY_F { GLFW_KEY_F };
constexpr inline const SGKey KEY_G { GLFW_KEY_G };
constexpr inline const SGKey KEY_H { GLFW_KEY_H };
constexpr inline const SGKey KEY_I { GLFW_KEY_I };
constexpr inline const SGKey KEY_J { GLFW_KEY_J };
constexpr inline const SGKey KEY_K { GLFW_KEY_K };
constexpr inline const SGKey KEY_L { GLFW_KEY_L };
constexpr inline const SGKey KEY_M { GLFW_KEY_M };
constexpr inline const SGKey KEY_N { GLFW_KEY_N };
constexpr inline const SGKey KEY_O { GLFW_KEY_O };
constexpr inline const SGKey KEY_P { GLFW_KEY_P };
constexpr inline const SGKey KEY_Q { GLFW_KEY_Q };
constexpr inline const SGKey KEY_R { GLFW_KEY_R };
constexpr inline const SGKey KEY_S { GLFW_KEY_S };
constexpr inline const SGKey KEY_T { GLFW_KEY_T };
constexpr inline const SGKey KEY_U { GLFW_KEY_U };
constexpr inline const SGKey KEY_V { GLFW_KEY_V };
constexpr inline const SGKey KEY_W { GLFW_KEY_W };
constexpr inline const SGKey KEY_X { GLFW_KEY_X };
constexpr inline const SGKey KEY_Y { GLFW_KEY_Y };
constexpr inline const SGKey KEY_Z { GLFW_KEY_Z };
constexpr inline const SGKey KEY_LEFT_BRACKET { GLFW_KEY_LEFT_BRACKET };
constexpr inline const SGKey KEY_BACKSLASH { GLFW_KEY_BACKSLASH };
constexpr inline const SGKey KEY_RIGHT_BRACKET { GLFW_KEY_RIGHT_BRACKET };
constexpr inline const SGKey KEY_GRAVE_ACCENT { GLFW_KEY_GRAVE_ACCENT };
constexpr inline const SGKey KEY_WORLD_1 { GLFW_KEY_WORLD_1 };
constexpr inline const SGKey KEY_WORLD_2 { GLFW_KEY_WORLD_2 };
constexpr inline const SGKey KEY_ESCAPE { GLFW_KEY_ESCAPE };
constexpr inline const SGKey KEY_ENTER { GLFW_KEY_ENTER };
constexpr inline const SGKey KEY_TAB { GLFW_KEY_TAB };
constexpr inline const SGKey KEY_BACKSPACE { GLFW_KEY_BACKSPACE };
constexpr inline const SGKey KEY_INSERT { GLFW_KEY_INSERT };
constexpr inline const SGKey KEY_DELETE { GLFW_KEY_DELETE };
constexpr inline const SGKey KEY_RIGHT { GLFW_KEY_RIGHT };
constexpr inline const SGKey KEY_LEFT { GLFW_KEY_LEFT };
constexpr inline const SGKey KEY_DOWN { GLFW_KEY_DOWN };
constexpr inline const SGKey KEY_UP { GLFW_KEY_UP };
constexpr inline const SGKey KEY_PAGE_UP { GLFW_KEY_PAGE_UP };
constexpr inline const SGKey KEY_PAGE_DOWN { GLFW_KEY_PAGE_DOWN };
constexpr inline const SGKey KEY_HOME { GLFW_KEY_HOME };
constexpr inline const SGKey KEY_END { GLFW_KEY_END };
constexpr inline const SGKey KEY_CAPS_LOCK { GLFW_KEY_CAPS_LOCK };
constexpr inline const SGKey KEY_SCROLL_LOCK { GLFW_KEY_SCROLL_LOCK };
constexpr inline const SGKey KEY_NUM_LOCK { GLFW_KEY_NUM_LOCK };
constexpr inline const SGKey KEY_PRINT_SCREEN { GLFW_KEY_PRINT_SCREEN };
constexpr inline const SGKey KEY_PAUSE { GLFW_KEY_PAUSE };
constexpr inline const SGKey KEY_F1 { GLFW_KEY_F1 };
constexpr inline const SGKey KEY_F2 { GLFW_KEY_F2 };
constexpr inline const SGKey KEY_F3 { GLFW_KEY_F3 };
constexpr inline const SGKey KEY_F4 { GLFW_KEY_F4 };
constexpr inline const SGKey KEY_F5 { GLFW_KEY_F5 };
constexpr inline const SGKey KEY_F6 { GLFW_KEY_F6 };
constexpr inline const SGKey KEY_F7 { GLFW_KEY_F7 };
constexpr inline const SGKey KEY_F8 { GLFW_KEY_F8 };
constexpr inline const SGKey KEY_F9 { GLFW_KEY_F9 };
constexpr inline const SGKey KEY_F10 { GLFW_KEY_F10 };
constexpr inline const SGKey KEY_F11 { GLFW_KEY_F11 };
constexpr inline const SGKey KEY_F12 { GLFW_KEY_F12 };
constexpr inline const SGKey KEY_F13 { GLFW_KEY_F13 };
constexpr inline const SGKey KEY_F14 { GLFW_KEY_F14 };
constexpr inline const SGKey KEY_F15 { GLFW_KEY_F15 };
constexpr inline const SGKey KEY_F16 { GLFW_KEY_F16 };
constexpr inline const SGKey KEY_F17 { GLFW_KEY_F17 };
constexpr inline const SGKey KEY_F18 { GLFW_KEY_F18 };
constexpr inline const SGKey KEY_F19 { GLFW_KEY_F19 };
constexpr inline const SGKey KEY_F20 { GLFW_KEY_F20 };
constexpr inline const SGKey KEY_F21 { GLFW_KEY_F21 };
constexpr inline const SGKey KEY_F22 { GLFW_KEY_F22 };
constexpr inline const SGKey KEY_F23 { GLFW_KEY_F23 };
constexpr inline const SGKey KEY_F24 { GLFW_KEY_F24 };
constexpr inline const SGKey KEY_F25 { GLFW_KEY_F25 };
constexpr inline const SGKey KEY_KP_0 { GLFW_KEY_KP_0 };
constexpr inline const SGKey KEY_KP_1 { GLFW_KEY_KP_1 };
constexpr inline const SGKey KEY_KP_2 { GLFW_KEY_KP_2 };
constexpr inline const SGKey KEY_KP_3 { GLFW_KEY_KP_3 };
constexpr inline const SGKey KEY_KP_4 { GLFW_KEY_KP_4 };
constexpr inline const SGKey KEY_KP_5 { GLFW_KEY_KP_5 };
constexpr inline const SGKey KEY_KP_6 { GLFW_KEY_KP_6 };
constexpr inline const SGKey KEY_KP_7 { GLFW_KEY_KP_7 };
constexpr inline const SGKey KEY_KP_8 { GLFW_KEY_KP_8 };
constexpr inline const SGKey KEY_KP_9 { GLFW_KEY_KP_9 };
constexpr inline const SGKey KEY_KP_DECIMAL { GLFW_KEY_KP_DECIMAL };
constexpr inline const SGKey KEY_KP_DIVIDE { GLFW_KEY_KP_DIVIDE };
constexpr inline const SGKey KEY_KP_MULTIPLY { GLFW_KEY_KP_MULTIPLY };
constexpr inline const SGKey KEY_KP_SUBTRACT { GLFW_KEY_KP_SUBTRACT };
constexpr inline const SGKey KEY_KP_ADD { GLFW_KEY_KP_ADD };
constexpr inline const SGKey KEY_KP_ENTER { GLFW_KEY_KP_ENTER };
constexpr inline const SGKey KEY_KP_EQUAL { GLFW_KEY_KP_EQUAL };
constexpr inline const SGKey KEY_LEFT_SHIFT { GLFW_KEY_LEFT_SHIFT };
constexpr inline const SGKey KEY_LEFT_CONTROL { GLFW_KEY_LEFT_CONTROL };
constexpr inline const SGKey KEY_LEFT_ALT { GLFW_KEY_LEFT_ALT };
constexpr inline const SGKey KEY_LEFT_SUPER { GLFW_KEY_LEFT_SUPER };
constexpr inline const SGKey KEY_RIGHT_SHIFT { GLFW_KEY_RIGHT_SHIFT };
constexpr inline const SGKey KEY_RIGHT_CONTROL { GLFW_KEY_RIGHT_CONTROL };
constexpr inline const SGKey KEY_RIGHT_ALT { GLFW_KEY_RIGHT_ALT };
constexpr inline const SGKey KEY_RIGHT_SUPER { GLFW_KEY_RIGHT_SUPER };
constexpr inline const SGKey KEY_MENU { GLFW_KEY_MENU };
constexpr inline const SGKey KEY_LAST { GLFW_KEY_LAST };

namespace Keyboard
{
    inline bool keyDown(const SGKey& key, GLFWwindow* wnd = Core::Main::Core::getWindow().getHandler()) noexcept
    {
        return glfwGetKey(wnd, key.keyCode) == GLFW_PRESS;
    }
}

#endif //NATIVECORE_KEYBOARD_H
