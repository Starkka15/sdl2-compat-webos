# SDL2-to-SDL1.2 Compatibility Shim

A compatibility layer that provides SDL2 API on top of SDL 1.2, designed primarily for porting modern SDL2 games to legacy platforms like webOS.

## Features

### Video/Window
- `SDL_CreateWindow` / `SDL_DestroyWindow`
- `SDL_SetWindowSize`, `SDL_SetWindowFullscreen`, `SDL_SetWindowTitle`, `SDL_SetWindowIcon`
- `SDL_GetWindowFlags`, `SDL_GetDesktopDisplayMode`

### Renderer/Textures
- `SDL_CreateRenderer` / `SDL_DestroyRenderer`
- `SDL_CreateTexture` / `SDL_CreateTextureFromSurface` / `SDL_DestroyTexture`
- `SDL_RenderCopy` / `SDL_RenderCopyEx` (with horizontal flip support)
- `SDL_RenderPresent`, `SDL_RenderClear`
- `SDL_RenderFillRect`, `SDL_RenderDrawLine`, `SDL_RenderDrawPoint`
- `SDL_SetTextureColorMod`, `SDL_SetTextureAlphaMod`
- `SDL_SetRenderDrawColor`, `SDL_SetRenderDrawBlendMode`

### Surfaces
- SDL2-style `SDL_GetColorKey`
- `SDL_SetSurfaceAlphaMod`, `SDL_SetSurfaceColorMod`, `SDL_SetSurfaceBlendMode`

### Filesystem
- `SDL_GetBasePath` / `SDL_GetPrefPath`

### Joystick
- `SDL_JoystickNameForIndex`, `SDL_JoystickGetAttached`
- `SDL_JoystickRumble` (stub - not supported in SDL1.2)

### Other
- `SDL_ShowSimpleMessageBox` (prints to console)
- Memory functions mapped to stdlib

## Limitations

- No rotation support in `SDL_RenderCopyEx` (flip only)
- No hardware acceleration (all software rendering)
- Single window only (SDL1.2 limitation)
- No text input events
- No rumble support

## Building

```bash
# Native build (requires SDL 1.2 dev headers)
make

# webOS cross-compilation
make webos
```

## Usage

1. Add `-I/path/to/sdl2-compat/include` to your include path
2. The `SDL2/SDL.h` include will pull in our compatibility headers
3. Link against `libSDL2-compat.a` and SDL 1.2

## Target: NXEngine-evo (Cave Story)

This shim was designed specifically to port NXEngine-evo to webOS. The game's clean abstraction layer (Renderer/Surface classes) makes it a good candidate for the shim approach.
