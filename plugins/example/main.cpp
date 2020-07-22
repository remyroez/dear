#include "imgui.h"
#include "dear.h"

namespace
{

int step(dear::context *context)
{
    ImGui::ShowDemoWindow();
    return 0;
}

} // namespace

DEAR_PLUGIN_MAIN(nullptr, &::step, nullptr, nullptr);
