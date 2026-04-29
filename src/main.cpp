#include "JSBridge.hpp"
#include "quickjs.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <fstream>
#include <iostream>
#include <string>

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

void check_exception(JSContext *ctx, JSValue val) {
  if (JS_IsException(val)) {
    JSValue exception = JS_GetException(ctx);
    const char *msg = JS_ToCString(ctx, exception);
    std::cerr << "JS ERROR: " << msg << std::endl;
    JS_FreeCString(ctx, msg);
    JS_FreeValue(ctx, exception);
  }
}

int main(int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO))
    return -1;
  if (!TTF_Init())
    return -1;

  SDL_Window *window =
      SDL_CreateWindow("Physics Playground", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  JSBridge::renderer = SDL_CreateRenderer(window, NULL);
  JSBridge::font = TTF_OpenFont("assets/font.ttf", 24);

  JSRuntime *rt = JS_NewRuntime();
  JSContext *ctx = JS_NewContext(rt);
  JSBridge::Register(ctx);

  // Inject Window Dimensions BEFORE loading script
  JSValue global = JS_GetGlobalObject(ctx);
  JS_SetPropertyStr(ctx, global, "windowWidth", JS_NewInt32(ctx, WINDOW_WIDTH));
  JS_SetPropertyStr(ctx, global, "windowHeight",
                    JS_NewInt32(ctx, WINDOW_HEIGHT));
  JS_FreeValue(ctx, global);

  // Load Bundle
  std::ifstream t("dist/bundle.js");
  std::string script((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
  JSValue eval = JS_Eval(ctx, script.c_str(), script.length(), "bundle.js",
                         JS_EVAL_TYPE_GLOBAL);
  check_exception(ctx, eval);
  JS_FreeValue(ctx, eval);

  bool quit = false;
  SDL_Event e;
  Uint64 lastTime = SDL_GetTicks();

  while (!quit) {
    Uint64 currentTime = SDL_GetTicks();
    double dt = (double)(currentTime - lastTime) / 1000.0;
    lastTime = currentTime;

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_EVENT_QUIT)
        quit = true;
      else if (e.type == SDL_EVENT_KEY_DOWN) {
        JSValue g = JS_GetGlobalObject(ctx);
        JSValue f = JS_GetPropertyStr(ctx, g, "onKeyPress");
        if (JS_IsFunction(ctx, f)) {
          JSValue k = JS_NewString(ctx, SDL_GetKeyName(e.key.key));
          JS_FreeValue(ctx, JS_Call(ctx, f, g, 1, &k));
          JS_FreeValue(ctx, k);
        }
        JS_FreeValue(ctx, f);
        JS_FreeValue(ctx, g);
      }
    }

    SDL_SetRenderDrawColor(JSBridge::renderer, 15, 15, 20, 255);
    SDL_RenderClear(JSBridge::renderer);

    JSValue g = JS_GetGlobalObject(ctx);
    JSValue r = JS_GetPropertyStr(ctx, g, "render");
    if (JS_IsFunction(ctx, r)) {
      JSValue jdt = JS_NewFloat64(ctx, dt);
      check_exception(ctx, JS_Call(ctx, r, g, 1, &jdt));
      JS_FreeValue(ctx, jdt);
    }
    JS_FreeValue(ctx, r);
    JS_FreeValue(ctx, g);

    SDL_RenderPresent(JSBridge::renderer);
  }

  TTF_CloseFont(JSBridge::font);
  JS_FreeContext(ctx);
  JS_FreeRuntime(rt);
  SDL_Quit();
  return 0;
}
