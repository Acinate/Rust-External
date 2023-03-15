#include "custom_elements.h"

bool c_gui::tab(const char* name, bool active, ImVec2 size_arg) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = CalcTextSize(name, NULL, true);
    DWORD flags = 0;
    ImVec2 pos = window->DC.CursorPos;
    if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    if (ImGuiItemFlags_ButtonRepeat)
        flags |= ImGuiButtonFlags_Repeat;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);
    if (pressed)
        MarkItemEdited(id);
    ImVec4 color = ImVec4(1.f, 1.f, 1.f, 1.f);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, ImColor(54, 54, 54, 255));
    if (active) {
        window->DrawList->AddRectFilled(bb.Min, bb.Max - ImVec2(134, 0), ImColor(100, 149, 237, 255));



        window->DrawList->AddRectFilled(bb.Min + ImVec2(134, 0), bb.Max, ImColor(100, 149, 237, 255));

        color = ImVec4(0 / 255.f, 190 / 255.f, 250 / 255.f, 1.f);
    }

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.f, 0.f, 0.f, 1.f));
    // RenderTextClipped(bb.Min + ImVec2(1, 1.5), bb.Max - style.FramePadding, name, NULL, &label_size, style.ButtonTextAlign, &bb);
     //RenderTextClipped(bb.Min, bb.Max - ImVec2(1, -2), name, NULL, &label_size, style.ButtonTextAlign, &bb);
    PopStyleColor();

    PushStyleColor(ImGuiCol_Text, color);

    RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, name, NULL, &label_size, style.ButtonTextAlign, &bb);

    PopStyleColor();

    return pressed;
}
bool c_gui::checkbox(const char* name, bool* active) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = CalcTextSize(name, NULL, true);

    const float square_sz = GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
        *active = !(*active);

    const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));

    const float pad = ImMax(1.0f, (float)(int)(square_sz / 6.5f));
    window->DrawList->AddRect(check_bb.Min + ImVec2(pad, pad), check_bb.Max - ImVec2(pad, pad), ImColor(64, 64, 64, 255), 0.f, ImDrawCornerFlags_All, 2.f);
    if (*active)
    {
        const float pad = ImMax(1.0f, (float)(int)(square_sz / 4.0f));
        window->DrawList->AddRectFilledMultiColor(check_bb.Min + ImVec2(pad, pad), check_bb.Max - ImVec2(pad, pad),
            ImColor(0, 170, 255, 255),
            ImColor(0, 170, 255, 255),
            ImColor(0, 100, 255, 255),
            ImColor(0, 100, 255, 255)
        );
    }

    if (label_size.x > 0.0f)
        RenderText(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y), name);

    return pressed;
}
bool c_gui::slider_button(const char* name, ImVec2 size_arg) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = CalcTextSize(name, NULL, true);
    DWORD flags = 0;
    ImVec2 pos = window->DC.CursorPos;
    if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    if (ImGuiItemFlags_ButtonRepeat)
        flags |= ImGuiButtonFlags_Repeat;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);
    if (pressed)
        MarkItemEdited(id);

    RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, name, NULL, &label_size, style.ButtonTextAlign, &bb);


    return pressed;
}

bool c_gui::slider_float(std::string label, float* v, float v_min, float v_max, const char* format) {
    char var[64];
    const char* value_buf_end = var + c_gui_DataTypeFormatString(var, IM_ARRAYSIZE(var), ImGuiDataType_Float, v, format);
    std::string text = "##" + label,
        value = label + ":";

    ImGui::Text(value.c_str());
    ImGui::SameLine();
    ImGui::Text(var);

    ImGui::SliderFloat(text.c_str(), v, v_min, v_max, format);
    return 0;
}
bool c_gui::slider_int(std::string label, int* v, int v_min, int v_max) {
    char var[64];
    const char* value_buf_end = var + c_gui_DataTypeFormatString(var, IM_ARRAYSIZE(var), ImGuiDataType_S32, v, "%d");
    std::string text = "##" + label,
        value = label + ":";

    ImGui::Text(value.c_str());
    ImGui::SameLine();
    ImGui::Text(var);
    ImGui::SliderInt(text.c_str(), v, v_min, v_max);
    return 0;
}

void c_gui::move_item(const ImVec2& moveVec)
{
    ImVec2 pos = ImGui::GetCursorScreenPos();
    SetCursorScreenPos({ pos.x + moveVec.x,pos.y + moveVec.y });
}