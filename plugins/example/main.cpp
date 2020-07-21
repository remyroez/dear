#include "imgui.h"
#include "cr.h"

namespace
{

int load()
{
    return 0;
}

int unload()
{
    // if needed, save stuff to pass over to next instance
    return 0;
}

int close()
{
    return 0;
}

int step()
{
    ImGui::ShowDemoWindow();
    return 0;
}

} // namespace

CR_EXPORT int cr_main(cr_plugin *ctx, cr_op operation) {
    assert(ctx);
    ImGui::SetCurrentContext((ImGuiContext*)ctx->userdata);

    switch (operation) {
        case CR_LOAD:
            return load();
        case CR_UNLOAD:
            return unload();
        case CR_CLOSE:
            return close();
        case CR_STEP:
            return step();
    }

    return 0;
}
