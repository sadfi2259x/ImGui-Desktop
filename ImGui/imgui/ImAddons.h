#pragma once
#include "imgui.h"
namespace ImAdd
{
    // voids
    IMGUI_API void          ToggleButton(const char* str_id, bool* v, bool label);
    IMGUI_API void          ToggleButtonLabel(const char* str_id, const char* text, bool* v, bool label);

    // bools
    IMGUI_API bool          ProgressBarBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col);
    IMGUI_API bool          Spinner(const char* label, float radius, int thickness, const ImU32& color);
    IMGUI_API bool          Knob(const char* label, float* p_value, float v_min, float v_max);
}