#pragma once
#include <string>

namespace glfw
{
    enum Keys
    {
        Unknown = -1,
        Space = 32,
        Apostrophe = 39,
        Comma = 44,
        Minus = 45,
        Period = 46,
        Slash = 47,
        Alpha0 = 48,
        Alpha1 = 49,
        Alpha2 = 50,
        Alpha3 = 51,
        Alpha4 = 52,
        Alpha5 = 53,
        Alpha6 = 54,
        Alpha7 = 55,
        Alpha8 = 56,
        Alpha9 = 57,
        SemiColon = 59,
        Equal = 61,
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        LeftBracket = 91,
        Backslash = 92,
        RightBracket = 93,
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        Numpad0 = 320,
        Numpad1 = 321,
        Numpad2 = 322,
        Numpad3 = 323,
        Numpad4 = 324,
        Numpad5 = 325,
        Numpad6 = 326,
        Numpad7 = 327,
        Numpad8 = 328,
        Numpad9 = 329,
        NumpadDecimal = 330,
        NumpadDivide = 331,
        NumpadMultiply = 332,
        NumpadSubtract = 333,
        NumpadAdd = 334,
        NumpadEnter = 335,
        NumpadEqual = 336,
        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,
        Menu = 348
    };
    
    inline std::string KeyToString(Keys key)
    {
        switch (key)
        {
        case Keys::Unknown: return "";
        case Keys::Space: return "Space";
        case Keys::Apostrophe: return "\'";
        case Keys::Comma: return ",";
        case Keys::Minus: return "-";
        case Keys::Period: return ".";
        case Keys::Slash: return "/";
        case Keys::Alpha0: return "0";
        case Keys::Alpha1: return "1";
        case Keys::Alpha2: return "2";
        case Keys::Alpha3: return "3";
        case Keys::Alpha4: return "4";
        case Keys::Alpha5: return "5";
        case Keys::Alpha6: return "6";
        case Keys::Alpha7: return "7";
        case Keys::Alpha8: return "8";
        case Keys::Alpha9: return "9";
        case Keys::SemiColon: return ";";
        case Keys::Equal: return "=";
        case Keys::A: return "A";
        case Keys::B: return "B";
        case Keys::C: return "C";
        case Keys::D: return "D";
        case Keys::E: return "E";
        case Keys::F: return "F";
        case Keys::G: return "G";
        case Keys::H: return "H";
        case Keys::I: return "I";
        case Keys::J: return "J";
        case Keys::K: return "K";
        case Keys::L: return "L";
        case Keys::M: return "M";
        case Keys::N: return "N";
        case Keys::O: return "O";
        case Keys::P: return "P";
        case Keys::Q: return "Q";
        case Keys::R: return "R";
        case Keys::S: return "S";
        case Keys::T: return "T";
        case Keys::U: return "U";
        case Keys::V: return "V";
        case Keys::W: return "W";
        case Keys::X: return "X";
        case Keys::Y: return "Y";
        case Keys::Z: return "Z";
        case Keys::LeftBracket: return "[";
        case Keys::Backslash: return "\\";
        case Keys::RightBracket: return "]";
        case Keys::Escape: return "Escape";
        case Keys::Enter: return "Enter";
        case Keys::Tab: return "Tab";
        case Keys::Backspace: return "Backspace";
        case Keys::Insert: return "Insert";
        case Keys::Delete: return "Delete";
        case Keys::Right: return "Right";
        case Keys::Left: return "Left";
        case Keys::Down: return "Down";
        case Keys::Up: return "Up";
        case Keys::PageUp: return "PageUp";
        case Keys::PageDown: return "PageDown";
        case Keys::Home: return "Home";
        case Keys::End: return "End";
        case Keys::CapsLock: return "CapsLock";
        case Keys::ScrollLock: return "ScrollLock";
        case Keys::NumLock: return "NumLock";
        case Keys::PrintScreen: return "PrntScr";
        case Keys::Pause: return "Pause";
        case Keys::F1: return "F1";
        case Keys::F2: return "F2";
        case Keys::F3: return "F3";
        case Keys::F4: return "F4";
        case Keys::F5: return "F5";
        case Keys::F6: return "F6";
        case Keys::F7: return "F7";
        case Keys::F8: return "F8";
        case Keys::F9: return "F9";
        case Keys::F10: return "F10";
        case Keys::F11: return "F11";
        case Keys::F12: return "F12";
        case Keys::F13: return "F13";
        case Keys::F14: return "F14";
        case Keys::F15: return "F15";
        case Keys::F16: return "F16";
        case Keys::F17: return "F17";
        case Keys::F18: return "F18";
        case Keys::F19: return "F19";
        case Keys::F20: return "F20";
        case Keys::F21: return "F21";
        case Keys::F22: return "F22";
        case Keys::Numpad0: return "N0";
        case Keys::Numpad1: return "N1";
        case Keys::Numpad2: return "N2";
        case Keys::Numpad3: return "N3";
        case Keys::Numpad4: return "N4";
        case Keys::Numpad5: return "N5";
        case Keys::Numpad6: return "N6";
        case Keys::Numpad7: return "N7";
        case Keys::Numpad8: return "N8";
        case Keys::Numpad9: return "N9";
        case Keys::NumpadDecimal: return "N.";
        case Keys::NumpadDivide: return "N/";
        case Keys::NumpadMultiply: return "N*";
        case Keys::NumpadSubtract: return "N-";
        case Keys::NumpadAdd: return "N+";
        case Keys::NumpadEnter: return "NEnter";
        case Keys::NumpadEqual: return "N=";
        case Keys::LeftShift: return "LShift";
        case Keys::LeftControl: return "LCtrl";
        case Keys::LeftAlt: return "LAlt";
        case Keys::LeftSuper: return "LWin";
        case Keys::RightShift: return "RShift";
        case Keys::RightControl: return "RCtrl";
        case Keys::RightAlt: return "RAlt";
        case Keys::RightSuper: return "RWin";
        case Keys::Menu: return "Menu";
        default: return "";
        }
    }
}
