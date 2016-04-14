#include "engine/system/platform/Keyboard.h"

namespace ds_platform
{
std::string Keyboard::PrintKey(Key key)
{
    std::string str = "UNKNOWN KEY";
    
    // If printable character
    if (key >= Key::Key_Exclamation && key <= Key::Key_z)
    {
        // Convert key to char and print
        str = std::string(1, (char)key);
    }
    // Non-printable or special characters
    else
    {
        switch (key)
        {
            case Key::Key_First:
                str = "RESERVED KEY - FIRST";
                break;
            case Key::Key_Last:
                str = "RESERVED KEY - LAST";
                break;
            case Key::Key_Backspace:
                str = "Backspace";
                break;
            case Key::Key_Tab:
                str = "Tab";
                break;
            case Key::Key_Return:
                str = "Return";
                break;
            case Key::Key_Escape:
                str = "Escape";
                break;
            case Key::Key_Space:
                str = "Spacebar";
                break;
            case Key::Key_Delete:
                str = "Delete";
                break;
            case Key::Key_CapsLock:
                str = "CapsLock";
                break;
            case Key::Key_F1:
                str = "F1";
                break;
            case Key::Key_F2:
                str = "F2";
                break;
            case Key::Key_F3:
                str = "F3";
                break;
            case Key::Key_F4:
                str = "F4";
                break;
            case Key::Key_F5:
                str = "F5";
                break;
            case Key::Key_F6:
                str = "F6";
                break;
            case Key::Key_F7:
                str = "F7";
                break;
            case Key::Key_F8:
                str = "F8";
                break;
            case Key::Key_F9:
                str = "F9";
                break;
            case Key::Key_F10:
                str = "F10";
                break;
            case Key::Key_F11:
                str = "F11";
                break;
            case Key::Key_F12:
                str = "F12";
                break;
            case Key::Key_PrintScreen:
                str = "PrintScreen";
                break;
            case Key::Key_ScrollLock:
                str = "ScrollLock";
                break;
            case Key::Key_Pause:
                str = "Pause";
                break;
            case Key::Key_Insert:
                str = "Insert";
                break;
            case Key::Key_Home:
                str = "Home";
                break;
            case Key::Key_PageUp:
                str = "PageUp";
                break;
            case Key::Key_End:
                str = "End";
                break;
            case Key::Key_PageDown:
                str = "PageDown";
                break;
            case Key::Key_RightArrow:
                str = "RightArrow";
                break;
            case Key::Key_LeftArrow:
                str = "LeftArrow";
                break;
            case Key::Key_DownArrow:
                str = "DownArrow";
                break;
            case Key::Key_UpArrow:
                str = "UpArrow";
                break;
            case Key::Key_LCtrl:
                str = "LeftCtrl";
                break;
            case Key::Key_LShift:
                str = "LeftShift";
                break;
            case Key::Key_LAlt:
                str = "LeftAlt";
                break;
            case Key::Key_RCtrl:
                str = "RightCtrl";
                break;
            case Key::Key_RShift:
                str = "RightShift";
                break;
            case Key::Key_RAlt:
                str = "RightAlt";
                break;
            default:
                break;
        }
    }

    return str;
}
}
