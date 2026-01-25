/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Filesystem functions
 */
#include <SDL/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include "../include/SDL2/SDL_filesystem.h"

/* Helper to create directories recursively */
static int mkdir_p(const char *path) {
    char tmp[512];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);

    if (tmp[len - 1] == '/') {
        tmp[len - 1] = 0;
    }

    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, 0755);
            *p = '/';
        }
    }

    return mkdir(tmp, 0755);
}

char* SDL_GetBasePath(void) {
    char *path = NULL;
    char buf[512];
    ssize_t len;

#ifdef __linux__
    /* Try /proc/self/exe on Linux */
    len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (len > 0) {
        buf[len] = '\0';
        /* Remove the executable name, keep directory */
        char *last_slash = strrchr(buf, '/');
        if (last_slash) {
            *(last_slash + 1) = '\0';
            path = strdup(buf);
        }
    }
#endif

    /* Fallback to current directory */
    if (!path) {
        if (getcwd(buf, sizeof(buf))) {
            len = strlen(buf);
            if (buf[len - 1] != '/') {
                strcat(buf, "/");
            }
            path = strdup(buf);
        }
    }

    /* Final fallback */
    if (!path) {
        path = strdup("./");
    }

    return path;
}

char* SDL_GetPrefPath(const char *org, const char *app) {
    char *path = NULL;
    char buf[512];
    const char *home;

    (void)org; /* Not used in simple implementation */

#ifdef __WEBOS__
    /* webOS: use /media/internal/appdata */
    snprintf(buf, sizeof(buf), "/media/internal/appdata/%s/", app);
#else
    /* Unix/Linux: use ~/.local/share/app */
    home = getenv("HOME");
    if (!home) {
        home = "/tmp";
    }
    snprintf(buf, sizeof(buf), "%s/.local/share/%s/", home, app);
#endif

    /* Create directory if it doesn't exist */
    mkdir_p(buf);

    path = strdup(buf);
    return path;
}
