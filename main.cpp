#include "imgui.h"
#include "imapp.h"

namespace
{

} // namespace

// Main loop (declaration)
void main_loop(void*);

// Main function
int main(int, char**)
{
    // Setup app
    if (!ImApp::BeginApplication("dear"))
    {
        return -1;
    }

    // Setup Dear ImGui
    {
        // io
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        // font
        //io.Fonts->AddFontDefault();
#ifndef IMGUI_DISABLE_FILE_FUNCTIONS
        //ImFont* font = io.Fonts->AddFontFromFileTTF("fonts/ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
#endif

        ImGui::StyleColorsDark();
    }

    // start
    ImApp::StartMainLoop(main_loop);
    ImApp::EndApplication();

    return 0;
}

// Main loop (definition)
void main_loop(void* arg)
{
    IM_UNUSED(arg);

}
