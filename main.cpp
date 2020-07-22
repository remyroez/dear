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

// Plug-In Manager type
class dear_plugin_manager
{
    // Plug-In Entry type
    struct entry_t {
        entry_t(std::string _name) : name(_name) {}
        std::string name;
        cr_plugin plugin_context;
    };

    // Entry handle type
    using entry_handle_t = std::shared_ptr<entry_t>;

public:
    // Open new plugin
    bool open(const std::string &name) {
        // make entry
        auto entry = std::make_shared<entry_t>(name);
        entry->plugin_context.userdata = _userdata;

        // make path
        auto filename = make_plugin_filename(name);
        auto path = _base_path / filename;

        // open
        bool success = cr_plugin_open(entry->plugin_context, path.c_str());
        if (success) {
            if (!_temp_path.empty()) {
                cr_set_temporary_path(entry->plugin_context, _temp_path.c_str());
            }
            _entries.emplace_back(entry);
        }
        return success;
    }

    // Close all plugin
    void close() {
        for (auto &entry : _entries) {
            cr_plugin_close(entry->plugin_context);
        }
        _entries.clear();
    }

    // Close target plugin
    void close(std::string_view name) {
        for (auto it = _entries.begin(); it != _entries.end(); ++it) {
            if ((*it)->name == name) {
                cr_plugin_close((*it)->plugin_context);
                it = _entries.erase(it);
                break;
            }
        }
    }

    // Update all plugin
    void update() {
        for (auto &entry : _entries) {
            cr_plugin_update(entry->plugin_context);
        }
    }

    // Set user data
    void set_userdata(void *userdata) {
        _userdata = userdata;
    }

    // Set base path
    void set_base_path(const std::filesystem::path &path) {
        _base_path = path;
    }

    // Set temporary path
    void set_temporary_path(const std::string &temp_path) {
        auto path = std::filesystem::absolute(temp_path);
        if (std::filesystem::is_directory(path)) {
            _temp_path = path / "";

        } else if (!std::filesystem::create_directory(path)) {
            // can't create == file?
            _temp_path.clear();

        } else {
            _temp_path = path / "";
        }
    }

protected:
    // Get entry
    entry_handle_t get(std::string_view name) {
        for (auto &entry : _entries) {
            if (entry->name == name) {
                return entry;
            }
        }
        return nullptr;
    }

private:
    // entries
    std::vector<entry_handle_t> _entries;

    // base path
    std::filesystem::path _base_path;

    // temporary path
    std::filesystem::path _temp_path;

    // user data
    void *_userdata = NULL;
};

// Plug-In Manager
dear_plugin_manager plugin_manager;

// Global Context
dear::context context;

} // namespace

// Main loop (forward declaration)
void main_loop(void*);

// Main function
int main(int, char**) {
    // setup app
    if (!ImApp::BeginApplication("dear")) {
        return -1;
    }

    // setup Dear ImGui
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

    // setup dear
    context.imgui_context = ImGui::GetCurrentContext();

    // setup plugin manager
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
void main_loop(void* arg) {
    IM_UNUSED(arg);

    plugin_manager.update();
}
