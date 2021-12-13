#include "glad/glad.h"
#define MENTAL_GL_GETINTEGERI_V
#define MENTAL_GL_GETINTEGER64I_V
#define MENTAL_GL_IMPLEMENTATION
#include "mental_gl.h"
#include "SDL.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DEFAULT_GL_MAJOR_VERSION 4
#define DEFAULT_GL_MINOR_VERSION 6
#define DEFAULT_GL_PROFILE SDL_GL_CONTEXT_PROFILE_COMPATIBILITY

static int StrCmpNoCase(const char* a, const char* b)
{
    int cmp;
    while ((cmp = toupper(*a) - toupper(*b)) == 0 && *a++ && *b++);
    return cmp;
}

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Failed to init SDL with video support: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    MGLFormattingOptions formatting;
    MGLFormattingOptions* formattingPtr = NULL;
    char* filter;
    if (argc > 1)
    {
        // MentalGL formatting for GLenum name.
        const size_t len = strlen(argv[1]);
        filter = malloc(len + 1);
        for (size_t i = 0u; i < len; i++) {
            filter[i] = toupper(argv[1][i]);
        }
        filter[len] = '\0';
        formatting.separator = ' ';
        formatting.distance = 1;
        formatting.array_limit = 200;
        formatting.order = MGLFormattingOrderDefault;
        formatting.enable_hex = 1;
        formatting.filter = filter;
        formattingPtr = &formatting;
    }
    else {
        fprintf(stderr, "Usage:\nglGet (enum-name) [(version-number) | (version-number) (profile)]\n\n");

        fprintf(stderr, "(profile) can be Core, Compatibility, or ES\n\n");

        fprintf(stderr, "Examples:\n");
        fprintf(stderr, "glGet GL_MAX_TEXTURE_SIZE\n");
        fprintf(stderr, "glGet GL_MAX_TEXTURE_SIZE 4.6\n");
        fprintf(stderr, "glGet GL_MAX_TEXTURE_SIZE 4.6 Compatibility\n\n");

        fprintf(stderr, "Strings are case insensitive:\n");
        fprintf(stderr, "glGet gl_max_texture_size 4.6 compatibility\n");
        SDL_Quit();
        return EXIT_FAILURE;
    }

    int majorVersion = DEFAULT_GL_MAJOR_VERSION;
    int minorVersion = DEFAULT_GL_MINOR_VERSION;
    int profileMask = DEFAULT_GL_PROFILE;
    if (argc > 2) {
        // GL version and profile.
        if (argc > 3) {
            if (StrCmpNoCase("CORE", argv[3]) == 0) {
                profileMask = SDL_GL_CONTEXT_PROFILE_CORE;
            }
            else if (StrCmpNoCase("COMPATIBILITY", argv[3]) == 0) {
                profileMask = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;
            }
            else if (StrCmpNoCase("ES", argv[3]) == 0) {
                profileMask = SDL_GL_CONTEXT_PROFILE_ES;
            }
        }
        sscanf(argv[2], "%d.%d", &majorVersion, &minorVersion);
        if (majorVersion > 0 && majorVersion <= 4)
        {
            switch (majorVersion)
            {
                case 1:
                    if (minorVersion < 0 || minorVersion > 5 || profileMask == SDL_GL_CONTEXT_PROFILE_CORE || (profileMask == SDL_GL_CONTEXT_PROFILE_ES && minorVersion > 1))
                    {
                        majorVersion = DEFAULT_GL_MAJOR_VERSION;
                        minorVersion = DEFAULT_GL_MINOR_VERSION;
                        profileMask = DEFAULT_GL_PROFILE;
                    }

                    break;

                case 2:
                    if (minorVersion < 0 || minorVersion > 1 || profileMask == SDL_GL_CONTEXT_PROFILE_CORE || (profileMask == SDL_GL_CONTEXT_PROFILE_ES && minorVersion > 0))
                    {
                        majorVersion = DEFAULT_GL_MAJOR_VERSION;
                        minorVersion = DEFAULT_GL_MINOR_VERSION;
                        profileMask = DEFAULT_GL_PROFILE;
                    }
                    break;

                case 3:
                    if (minorVersion < 0 || minorVersion > 3 || (profileMask == SDL_GL_CONTEXT_PROFILE_ES && minorVersion > 2))
                    {
                        majorVersion = DEFAULT_GL_MAJOR_VERSION;
                        minorVersion = DEFAULT_GL_MINOR_VERSION;
                        profileMask = DEFAULT_GL_PROFILE;
                    }
                    break;

                case 4:
                    if (minorVersion < 0 || minorVersion > 6 || profileMask == SDL_GL_CONTEXT_PROFILE_ES)
                    {
                        majorVersion = DEFAULT_GL_MAJOR_VERSION;
                        minorVersion = DEFAULT_GL_MINOR_VERSION;
                        profileMask = DEFAULT_GL_PROFILE;
                    }
                    break;

                default:
                    majorVersion = DEFAULT_GL_MAJOR_VERSION;
                    minorVersion = DEFAULT_GL_MINOR_VERSION;
                    profileMask = DEFAULT_GL_PROFILE;
                    break;
            }
        }
    }

    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, majorVersion) < 0 || SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minorVersion) < 0 || SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, profileMask) < 0) {
        fprintf(stderr, "Failed setting OpenGL version and/or profile: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow("", 0, 0, 640, 480, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
    if (!window)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    if (SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &majorVersion) < 0 || SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minorVersion) < 0 || SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &profileMask) < 0) {
        fprintf(stderr, "%s\n", SDL_GetError());
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    printf("Using OpenGL %d.%d ", majorVersion, minorVersion);
    switch (profileMask)
    {
        case SDL_GL_CONTEXT_PROFILE_CORE:
            printf("Core\n");
            if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
            {
                fprintf(stderr, "Failed to load OpenGL functions\n");
                SDL_GL_DeleteContext(context);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return EXIT_FAILURE;
            }
            break;

        case SDL_GL_CONTEXT_PROFILE_COMPATIBILITY:
            printf("Compatibility\n");
            if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
            {
                fprintf(stderr, "Failed to load OpenGL functions\n");
                SDL_GL_DeleteContext(context);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return EXIT_FAILURE;
            }
            break;

        case SDL_GL_CONTEXT_PROFILE_ES:
            printf("ES\n");
            if (majorVersion == 1)
            {
                if (!gladLoadGLES1Loader(SDL_GL_GetProcAddress))
                {
                    fprintf(stderr, "Failed to load OpenGL functions\n");
                    SDL_GL_DeleteContext(context);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    return EXIT_FAILURE;
                }
            }
            else if (!gladLoadGLES2Loader(SDL_GL_GetProcAddress))
            {
                fprintf(stderr, "Failed to load OpenGL functions\n");
                SDL_GL_DeleteContext(context);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return EXIT_FAILURE;
            }
            break;

        default:
            printf("[INVALID CONTEXT PROFILE]\n");
            break;
    }

    MGLRenderState rs;
    mglQueryRenderState(&rs);
    MGLString s = mglPrintRenderState(&rs, formattingPtr);
    if (strcmp(mglGetUTF8String(s), "") == 0)
    {
        printf("Filter did not match any names\n");
    }
    else {
        printf("%s", mglGetUTF8String(s));
    }
    mglFreeString(s);
    free(filter);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
