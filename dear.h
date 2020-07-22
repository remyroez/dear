#ifndef DEAR_H_
#define DEAR_H_

// ImGuiContext
#include "imgui.h"

// pre define for cr
#ifdef DEAR_HOST
#define CR_HOST DEAR_HOST
#endif

// cr
#define CR_MAIN_FUNC "dear_plugin_main"
#include "cr.h"

// post define for cr
#define DEAR_EXPORT CR_EXPORT

namespace dear {

// global context
struct context {
    ImGuiContext *imgui_context = nullptr;
};

// plugin operation
using operation = int (context *);

} // namespace dear

#define DEAR_PLUGIN_MAIN(LOAD_FN, STEP_FN, UNLOAD_FN, CLOSE_FN) \
DEAR_EXPORT int dear_plugin_main(cr_plugin *ctx, cr_op operation) { \
    assert(ctx); \
    auto *context = static_cast<dear::context *>(ctx->userdata); \
    ImGui::SetCurrentContext(context->imgui_context); \
    switch (operation) { \
        case CR_LOAD:   if (auto fn = static_cast<dear::operation*>(LOAD_FN))   return fn(context); \
        case CR_STEP:   if (auto fn = static_cast<dear::operation*>(STEP_FN))   return fn(context); \
        case CR_UNLOAD: if (auto fn = static_cast<dear::operation*>(UNLOAD_FN)) return fn(context); \
        case CR_CLOSE:  if (auto fn = static_cast<dear::operation*>(CLOSE_FN))  return fn(context); \
    } \
    return 0; \
}

#endif // DEAR_H_
