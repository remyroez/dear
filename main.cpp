#include "imgui.h"
#include "imapp.h"

#define CR_HOST CR_UNSAFE
#include "cr.h"

#include <filesystem>
#include <vector>

namespace
{

// Plug-In Manager
class dear_plugin_manager
{
public:
    // Open new plugin
    bool open(const char *path) {
        cr_plugin plugin;
        plugin.userdata = _userdata;
        bool success = cr_plugin_open(plugin, path);
        if (success) {
            _plugins.push_back(plugin);
        }
        return success;
    }

    // Close all plugin
    void close() {
        for (auto &plugin : _plugins) {
            cr_plugin_close(plugin);
        }
        _plugins.clear();
    }

    // Update all plugin
    void update() {
        for (auto &plugin : _plugins) {
            cr_plugin_update(plugin);
        }
    }

    // Set user data
    void set_userdata(void *userdata) {
        _userdata = userdata;
    }

private:
    // plugins
    std::vector<cr_plugin> _plugins;

    // user data
    void *_userdata = NULL;
};

dear_plugin_manager plugin_manager;

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
    plugin_manager.set_userdata(ImGui::GetCurrentContext());
    plugin_manager.open("plugins/" CR_PLUGIN("example"));

    // start
    ImApp::StartMainLoop(main_loop);

    // cleanup
    plugin_manager.close();
    ImApp::EndApplication();

    return 0;
}

// Main loop (definition)
void main_loop(void* arg)
{
    IM_UNUSED(arg);

    plugin_manager.update();
}
