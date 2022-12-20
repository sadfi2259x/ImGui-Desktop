/*
This is a simple items addons for Dear ImGui
Credits: 
SADFI2259X: Main creator
HankiDesign: Awesome Dear ImGui
*/

#include "ImAddons.h"

#include "imgui.h"
#include "imgui_internal.h"

#include <iostream>


void ImAdd::ToggleButton(const char* str_id, bool* v, bool label)
{
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = 20;
    float width = 44;
    float border = 2;
    float radius = 5 * 2;
    float rounding = 10.0;
    //float rounding = int_slider; // remove me later

    if (ImGui::InvisibleButton(str_id, ImVec2(width, height)))
        *v = !*v;

    ImU32 col_bg;
    ImU32 cir_bg;
    if (ImGui::IsItemHovered())
    {
        if (*v)
        {
            col_bg = IM_COL32(204, 204, 204, 255);
            cir_bg = IM_COL32(100, 100, 100, 255);
        }
        else
        {
            col_bg = IM_COL32(100, 100, 100, 255);
            cir_bg = IM_COL32(204, 204, 204, 255);
        }
    }
    else
    {
        if (*v)
        {
            col_bg = IM_COL32(204, 204, 204, 255);
            cir_bg = IM_COL32(100, 100, 100, 255);
        }
        else
        {
            col_bg = IM_COL32(100, 100, 100, 255);
            cir_bg = IM_COL32(204, 204, 204, 255);
        }
    }

    if (*v)
    {
        draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + width, p.y + (height)), cir_bg, rounding); // This is the white out lines
    }
    else
    {
        draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + width, p.y + (height)), cir_bg, rounding); // This is the white out lines
        draw_list->AddRectFilled(ImVec2(p.x + border, p.y + border), ImVec2((p.x + width) - border, (p.y + height) - border), col_bg, rounding);
    }

    draw_list->AddCircleFilled(ImVec2(*v ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius - ((height - radius) / 2), IM_COL32(255, 255, 255, 255));

    if (label)
    {
        ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 3.0f));
        ImGui::Spacing();
        if (*v)
        {
            ImGui::Text("On");
        }
        else
        {
            ImGui::Text("Off");
        }
        ImGui::Spacing();
        ImGui::PopStyleVar();
        ImGui::EndGroup();
    }
}

void ImAdd::ToggleButtonLabel(const char* str_id, const char* text, bool* v, bool label)
{
    ImGui::Text(text);

    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = 20;
    float width = 44;
    float border = 2;
    float radius = 5 * 2;
    float rounding = 10.0;
    //float rounding = int_slider; // remove me later

    if (ImGui::InvisibleButton(str_id, ImVec2(width, height)))
        *v = !*v;

    ImU32 col_bg;
    ImU32 cir_bg;
    if (ImGui::IsItemHovered())
    {
        if (*v)
        {
            col_bg = IM_COL32(204, 204, 204, 255);
            cir_bg = IM_COL32(100, 100, 100, 255);
        }
        else
        {
            col_bg = IM_COL32(100, 100, 100, 255);
            cir_bg = IM_COL32(204, 204, 204, 255);
        }
    }
    else
    {
        if (*v)
        {
            col_bg = IM_COL32(204, 204, 204, 255);
            cir_bg = IM_COL32(100, 100, 100, 255);
        }
        else
        {
            col_bg = IM_COL32(100, 100, 100, 255);
            cir_bg = IM_COL32(204, 204, 204, 255);
        }
    }

    if (*v)
    {
        draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + width, p.y + (height)), cir_bg, rounding); // This is the white out lines
    }
    else
    {
        draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + width, p.y + (height)), cir_bg, rounding); // This is the white out lines
        draw_list->AddRectFilled(ImVec2(p.x + border, p.y + border), ImVec2((p.x + width) - border, (p.y + height) - border), col_bg, rounding);
    }

    draw_list->AddCircleFilled(ImVec2(*v ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius - ((height - radius) / 2), IM_COL32(255, 255, 255, 255));

    if (label)
    {
        ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 3.0f));
        ImGui::Spacing();
        if (*v)
        {
            ImGui::Text("On");
        }
        else
        {
            ImGui::Text("Off");
        }
        ImGui::Spacing();
        ImGui::PopStyleVar();
        ImGui::EndGroup();
    }
}
    
bool ImAdd::ProgressBarBar(const char* label, float value,  const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;
        
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = size_arg;
    size.x -= style.FramePadding.x * 2;
        
    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;
        
    // Render
    const float circleStart = size.x * 0.7f;
    const float circleEnd = size.x;
    const float circleWidth = circleEnd - circleStart;
        
    window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), bg_col);
    window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart*value, bb.Max.y), fg_col);
    
    /*
    const float t = g.Time;
    const float r = size.y / 2;
    const float speed = 1.5f;
        
    const float a = speed*0;
    const float b = speed*0.333f;
    const float c = speed*0.666f;
        
    const float o1 = (circleWidth+r) * (t+a - speed * (int)((t+a) / speed)) / speed;
    const float o2 = (circleWidth+r) * (t+b - speed * (int)((t+b) / speed)) / speed;
    const float o3 = (circleWidth+r) * (t+c - speed * (int)((t+c) / speed)) / speed;
        
    window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, bg_col);
    window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, bg_col);
    window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, bg_col);
    */
}

bool ImAdd::Spinner(const char* label, float radius, int thickness, const ImU32& color) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;
        
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
        
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size((radius )*2, (radius + style.FramePadding.y)*2);
        
    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;
        
    // Render
    window->DrawList->PathClear();
        
    int num_segments = 30;
    int start = abs(ImSin(g.Time*1.8f)*(num_segments-5));
        
    const float a_min = IM_PI*2.0f * ((float)start) / (float)num_segments;
    const float a_max = IM_PI*2.0f * ((float)num_segments-3) / (float)num_segments;

    const ImVec2 centre = ImVec2(pos.x+radius, pos.y+radius+style.FramePadding.y);
        
    for (int i = 0; i < num_segments; i++) {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a+g.Time*8) * radius,
                                            centre.y + ImSin(a+g.Time*8) * radius));
    }

    window->DrawList->PathStroke(color, false, thickness);
}

bool ImAdd::Knob(const char* label, float* p_value, float v_min, float v_max)
{
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();

    float radius_outer = 20.0f;
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 center = ImVec2(pos.x + radius_outer, pos.y + radius_outer);
    float line_height = ImGui::GetTextLineHeight();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float ANGLE_MIN = 3.141592f * 0.75f;
    float ANGLE_MAX = 3.141592f * 2.25f;

    ImGui::InvisibleButton(label, ImVec2(radius_outer * 2, radius_outer * 2 + line_height + style.ItemInnerSpacing.y));
    bool value_changed = false;
    bool is_active = ImGui::IsItemActive();
    bool is_hovered = ImGui::IsItemActive();
    if (is_active && io.MouseDelta.x != 0.0f)
    {
        float step = (v_max - v_min) / 200.0f;
        *p_value += io.MouseDelta.x * step;
        if (*p_value < v_min) *p_value = v_min;
        if (*p_value > v_max) *p_value = v_max;
        value_changed = true;
    }

    float t = (*p_value - v_min) / (v_max - v_min);
    float angle = ANGLE_MIN + (ANGLE_MAX - ANGLE_MIN) * t;
    float angle_cos = cosf(angle), angle_sin = sinf(angle);
    float radius_inner = radius_outer * 0.40f;
    draw_list->AddCircleFilled(center, radius_outer, ImGui::GetColorU32(ImGuiCol_FrameBg), 16);
    draw_list->AddLine(ImVec2(center.x + angle_cos * radius_inner, center.y + angle_sin * radius_inner), ImVec2(center.x + angle_cos * (radius_outer - 2), center.y + angle_sin * (radius_outer - 2)), ImGui::GetColorU32(ImGuiCol_SliderGrabActive), 2.0f);
    draw_list->AddCircleFilled(center, radius_inner, ImGui::GetColorU32(is_active ? ImGuiCol_FrameBgActive : is_hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), 16);
    draw_list->AddText(ImVec2(pos.x, pos.y + radius_outer * 2 + style.ItemInnerSpacing.y), ImGui::GetColorU32(ImGuiCol_Text), label);

    if (is_active || is_hovered)
    {
        ImGui::SetNextWindowPos(ImVec2(pos.x - style.WindowPadding.x, pos.y - line_height - style.ItemInnerSpacing.y - style.WindowPadding.y));
        ImGui::BeginTooltip();
        ImGui::Text("%.3f", *p_value);
        ImGui::EndTooltip();
    }

    return value_changed;
}