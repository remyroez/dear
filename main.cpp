#include "imgui.h"
#include "imapp.h"

#define DEAR_HOST CR_UNSAFE
#include "dear.h"

#include <filesystem>
#include <vector>
#include <string>
#include <memory>

namespace
{

// Make Plug-In file name
inline std::string make_plugin_filename(const std::string &name) {
#if defined(CR_WINDOWS)
    return name + ".dll";
#elif defined(CR_LINUX)
    return "lib" + name + ".so";
#elif defined(CR_OSX)
    return "lib" + name + ".dylib";
#else
    return name;
#endif
}

// Plug-In Entry
struct dear_plugin_entry {
    dear_plugin_entry(std::string _name) : name(_name) {}
    std::string name;
    cr_plugin plugin_context;
};

// Plug-In Manager
class dear_plugin_manager
{
    // Entry type
    using entry_t = std::shared_ptr<dear_plugin_entry>;

public:
    // Open new plugin
    bool open(const std::string &name) {
        // make entry
        auto entry = std::make_shared<dear_plugin_entry>(name);
        entry->plugin_context.userdata = _userdata;

        // make path
        auto filename = make_plugin_filename(name);
        auto path = _base_path / filename;

        // open
        bool success = cr_plugin_open(entry->plugin_context, path.c_str());
        if (success) {
            if (!_temp_path.empty()) cr_set_temporary_path(entry->plugin_context, _temp_path);
            _plugin_entries.emplace_back(entry);
        }
        return success;
    }

    // Close all plugin
    void close() {
        for (auto &entry : _plugin_entries) {
            cr_plugin_close(entry->plugin_context);
        }
        _plugin_entries.clear();
    }

    // Close target plugin
    void close(std::string_view name) {
        for (auto it = _plugin_entries.begin(); it != _plugin_entries.end(); ++it) {
            if ((*it)->name == name) {
                cr_plugin_close((*it)->plugin_context);
                it = _plugin_entries.erase(it);
                break;
            }
        }
    }

    // Update all plugin
    void update() {
        for (auto &entry : _plugin_entries) {
            cr_plugin_update(entry->plugin_context);
        }
    }

    // Set user data
    void set_userdata(void *userdata) {
        _userdata = userdata;
    }

    // Set temporary path
    void set_temporary_path(const std::string &temp_path) {
        auto path = std::filesystem::absolute(temp_path);
        if (std::filesystem::is_directory(path)) {
            _temp_path = path / "";

        } else if (!std::filesystem::create_directory(path)) {
            // can't create == file?
            _temp_path = std::filesystem::temp_directory_path() / "";

        } else {
            _temp_path = path / "";
        }
    }

    // Set base path
    void set_base_path(const std::filesystem::path &path) {
        _base_path = path;
    }

protected:
    // Get entry
    entry_t get(std::string_view name) {
        for (auto &entry : _plugin_entries) {
            if (entry->name == name) {
                return entry;
            }
        }
        return nullptr;
    }

private:
    // plugin entries
    std::vector<entry_t> _plugin_entries;

    // base path
    std::filesystem::path _base_path;

    // temporary path
    std::string _temp_path;

    // user data
    void *_userdata = NULL;
};

dear_plugin_manager plugin_manager;

dear::context context;

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

    // Setup dear
    context.imgui_context = ImGui::GetCurrentContext();

    // Setup plugin manager
    plugin_manager.set_userdata(&context);
    plugin_manager.set_base_path("plugins");
    plugin_manager.set_temporary_path("temp");

    // open plugin
    plugin_manager.open("example");

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
