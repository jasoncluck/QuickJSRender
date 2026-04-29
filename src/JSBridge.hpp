#pragma once
#include "quickjs.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace JSBridge {
    // We store these globally in the bridge for easy access by JS functions
    extern SDL_Renderer* renderer;
    extern TTF_Font* font;

    // The actual C functions
    JSValue js_drawRect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
    JSValue js_drawText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);

    // Helper to register everything at once
    void Register(JSContext* ctx);
}
