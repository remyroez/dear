#include "imgui.h"
#include "imapp.h"

#define CR_HOST CR_UNSAFE
#include "cr.h"

#include <filesystem>

namespace
{
    
cr_plugin ctx;

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

    // Setup cr
    ctx.userdata = ImGui::GetCurrentContext();
    cr_plugin_open(ctx, "plugins/" CR_PLUGIN("example"));

    // start
    ImApp::StartMainLoop(main_loop);

    // cleanup
    cr_plugin_close(ctx);
    ImApp::EndApplication();

    return 0;
}

// Main loop (definition)
void main_loop(void* arg)
{
    IM_UNUSED(arg);

    cr_plugin_update(ctx);
}
