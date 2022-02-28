#include "ColorDecoder.h"
#include "config.h"

Uint32 getpixel(SDL_Surface* surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp)
    {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16 *)p;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
                break;

        case 4:
            return *(Uint32 *)p;
            break;

        default:
            return 0;
    }
}



SDL_Color decodeColor(SDL_Surface* gSurface, int x, int y){
    SDL_Color rgb;
    if(x>=0 && x<WIDTH && y>=0 && y<HEIGHT) {
        Uint32 data = getpixel(gSurface, x, y);
        SDL_GetRGB(data, gSurface->format, &rgb.r, &rgb.g, &rgb.b);
    }
    return rgb;
}

bool compareColor(SDL_Color c1, SDL_Color c2){
    if(c1.r==c2.r && c1.g==c2.g && c1.b==c2.b) return true;
    else return false;

}