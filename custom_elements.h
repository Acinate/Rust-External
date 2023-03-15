#pragma once
#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS

#include <windows.h>
#include <string>
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

using namespace ImGui;

class c_gui {
public:
    bool tab(const char* name, bool active, ImVec2 size);
    bool slider_button(const char* name, ImVec2 size = ImVec2(0, 0));
    bool slider_float(std::string label, float* v, float v_min, float v_max, const char* format = "%.3f");
    bool slider_int(std::string label, int* v, int v_min, int v_max);
    bool combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items);
    bool checkbox(const char* name, bool* active);
    bool keybind(const char* name, int button);
    bool colorpicker(const char* label, float col[4]);

    void move_item(const ImVec2& moveVec);
};

static inline int c_gui_DataTypeFormatString(char* buf, int buf_size, ImGuiDataType data_type, const void* data_ptr, const char* format)
{
    // Signedness doesn't matter when pushing integer arguments
    if (data_type == ImGuiDataType_S32 || data_type == ImGuiDataType_U32)
        return ImFormatString(buf, buf_size, format, *(const ImU32*)data_ptr);
    if (data_type == ImGuiDataType_S64 || data_type == ImGuiDataType_U64)
        return ImFormatString(buf, buf_size, format, *(const ImU64*)data_ptr);
    if (data_type == ImGuiDataType_Float)
        return ImFormatString(buf, buf_size, format, *(const float*)data_ptr);
    if (data_type == ImGuiDataType_Double)
        return ImFormatString(buf, buf_size, format, *(const double*)data_ptr);
    if (data_type == ImGuiDataType_S8)
        return ImFormatString(buf, buf_size, format, *(const ImS8*)data_ptr);
    if (data_type == ImGuiDataType_U8)
        return ImFormatString(buf, buf_size, format, *(const ImU8*)data_ptr);
    if (data_type == ImGuiDataType_S16)
        return ImFormatString(buf, buf_size, format, *(const ImS16*)data_ptr);
    if (data_type == ImGuiDataType_U16)
        return ImFormatString(buf, buf_size, format, *(const ImU16*)data_ptr);
    IM_ASSERT(0);
    return 0;
}