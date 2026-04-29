#include "JSBridge.hpp"

namespace JSBridge {
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;

    JSValue js_drawRect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        double x, y, w, h;
        int r, g, b;
        JS_ToFloat64(ctx, &x, argv[0]);
        JS_ToFloat64(ctx, &y, argv[1]);
        JS_ToFloat64(ctx, &w, argv[2]);
        JS_ToFloat64(ctx, &h, argv[3]);
        JS_ToInt32(ctx, &r, argv[4]);
        JS_ToInt32(ctx, &g, argv[5]);
        JS_ToInt32(ctx, &b, argv[6]);

        SDL_FRect rect = { (float)x, (float)y, (float)w, (float)h };
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderFillRect(renderer, &rect);
        return JS_UNDEFINED;
    }

    JSValue js_drawText(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        const char* text = JS_ToCString(ctx, argv[0]);
        double x, y;
        JS_ToFloat64(ctx, &x, argv[1]);
        JS_ToFloat64(ctx, &y, argv[2]);

        SDL_Color white = { 255, 255, 255, 255 };
        SDL_Surface* surface = TTF_RenderText_Blended(font, text, 0, white);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FRect dst = { (float)x, (float)y, (float)surface->w, (float)surface->h };
            SDL_RenderTexture(renderer, texture, NULL, &dst);
            SDL_DestroySurface(surface);
            SDL_DestroyTexture(texture);
        }
        JS_FreeCString(ctx, text);
        return JS_UNDEFINED;
    }

    void Register(JSContext* ctx) {
        JSValue global = JS_GetGlobalObject(ctx);
        JS_SetPropertyStr(ctx, global, "nativeDrawRect", JS_NewCFunction(ctx, js_drawRect, "nativeDrawRect", 7));
        JS_SetPropertyStr(ctx, global, "nativeDrawText", JS_NewCFunction(ctx, js_drawText, "nativeDrawText", 3));
        JS_FreeValue(ctx, global);
    }
}
