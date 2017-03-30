#include <SDL2/SDL.h>
#include "SDL2_image/SDL_image.h"
#include "SDL2_ttf/SDL_ttf.h"
#include "graphics.h"
#include "conf.h"
#include <iostream>

//#include "sprig/sprig.h"
//#include "sprig/sprig_common.h"

//constructor
Graphics::Graphics()
{
  TTF_Init();
  std::cout<<"Graphics created\n";
  //int SDL_CreateWindowAndRenderer(int            width,
  //                        int            height,
  //                        Uint32         window_flags,
  //pointer to window pointer          SDL_Window**   window,
  //pointer to renderer pointer        SDL_Renderer** renderer)
  //void SDL_SetWindowTitle(SDL_Window* window, const char* title)
  //create window
  _window = SDL_CreateWindow( "Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 720, SDL_WINDOW_SHOWN );
  //std::cerr<<SDL_GetWindowSurface(_window);
  
  //_surface = SDL_GetWindowSurface(_window);

  //Create renderer for window
  _renderer = SDL_CreateRenderer( _window, -1, SDL_RENDERER_ACCELERATED);
  if(_renderer == NULL )
  {
    printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
  }
  else
  {
    //Initialize renderer color
    SDL_SetRenderDrawColor( _renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
      printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }
  }
  SDL_SetWindowTitle(this->_window, "Game");
  //_surface = SDL_GetWindowSurface(_window);

  SDL_Surface* surface = IMG_Load("sprites/dither.png");
  dith = surface;
  
  //dith = SDL_CreateTextureFromSurface(_renderer, surface);
  //SDL_FreeSurface(surface);
  
  surface = IMG_Load("sprites/ghostcolor.png");
  ghostColor = surface;
  
  surface = IMG_Load("sprites/ghostcolor1.png");
  ghostColorInit = surface;
  //invDith = SDL_CreateTextureFromSurface(_renderer, surface);
  //SDL_FreeSurface(surface);
  
  blackSurface = SDL_CreateRGBSurface(0, 500, 500, 32, 0, 0, 0, 0);
  SDL_FillRect(blackSurface, NULL, SDL_MapRGB(blackSurface->format, 0, 0, 0));
}

//destructor
Graphics::~Graphics()
{
  std::cout<<"Graphics is destroyed";
  //Destroy window    
  SDL_DestroyRenderer( _renderer );
  SDL_DestroyWindow( _window );
  //SDL_FreeSurface(_surface);
  _window = NULL;
  _renderer = NULL;
  _surface = NULL;
  
  SDL_FreeSurface(dith);
  SDL_FreeSurface(ghostColor);
  SDL_FreeSurface(blackSurface);
  SDL_FreeSurface(ghostColorInit);
  
  SDL_FreeSurface(messageSurface);
  //SDL_DestroyTexture(dith);
  //SDL_DestroyTexture(invDith);

  //Quit SDL subsystems
  IMG_Quit();
  SDL_Quit();
}

SDL_Surface* Graphics::loadImage(const std::string &spriteSheetFilePath){
  //this is going to require downloading SDL_Image
  if (_spriteSheets.count(spriteSheetFilePath) == 0){
    this->_spriteSheets[spriteSheetFilePath] = IMG_Load(spriteSheetFilePath.c_str());
  }

  if(!_spriteSheets[spriteSheetFilePath]) {
    printf("IMG_Load: %s\n", IMG_GetError());
  }
  return this-> _spriteSheets[spriteSheetFilePath];
}

void Graphics::blendPixelOnSurface(SDL_Surface *surface, Sint16 x, Sint16 y, Uint32 color, Uint8 alpha, BLEND_TYPE btype){
  if(true){
    Uint32 Rmask = surface->format->Rmask, Gmask = surface->format->Gmask, Bmask = surface->format->Bmask, Amask = surface->format->Amask;
    Uint32 R,G,B,A=0;//SDL_ALPHA_OPAQUE;
    Uint32* pixel;
    switch (surface->format->BytesPerPixel) {
      case 1: { /* Assuming 8-bpp */
        
        Uint8 *pixel = (Uint8 *)surface->pixels + y*surface->pitch + x;
        
        Uint8 dR = surface->format->palette->colors[*pixel].r;
        Uint8 dG = surface->format->palette->colors[*pixel].g;
        Uint8 dB = surface->format->palette->colors[*pixel].b;
        Uint8 sR = surface->format->palette->colors[color].r;
        Uint8 sG = surface->format->palette->colors[color].g;
        Uint8 sB = surface->format->palette->colors[color].b;
        
        dR = dR + ((sR-dR)*alpha >> 8);
        dG = dG + ((sG-dG)*alpha >> 8);
        dB = dB + ((sB-dB)*alpha >> 8);
        
        *pixel = SDL_MapRGB(surface->format, dR, dG, dB);
        
      }
        break;
        
      case 2: { /* Probably 15-bpp or 16-bpp */
        
        Uint16 *pixel = (Uint16 *)surface->pixels + y*surface->pitch/2 + x;
        Uint32 dc = *pixel;
        
        R = ((dc & Rmask) + (( (color & Rmask) - (dc & Rmask) ) * alpha >> 8)) & Rmask;
        G = ((dc & Gmask) + (( (color & Gmask) - (dc & Gmask) ) * alpha >> 8)) & Gmask;
        B = ((dc & Bmask) + (( (color & Bmask) - (dc & Bmask) ) * alpha >> 8)) & Bmask;
        if( Amask )
          A = ((dc & Amask) + (( (color & Amask) - (dc & Amask) ) * alpha >> 8)) & Amask;
        
        *pixel= R | G | B | A;
        
      }
        break;
        
      case 3: { /* Slow 24-bpp mode, usually not used */
        Uint8 *pix = (Uint8 *)surface->pixels + y * surface->pitch + x*3;
        Uint8 rshift8=surface->format->Rshift/8;
        Uint8 gshift8=surface->format->Gshift/8;
        Uint8 bshift8=surface->format->Bshift/8;
        Uint8 ashift8=surface->format->Ashift/8;
        
        
        
        Uint8 dR, dG, dB, dA=0;
        Uint8 sR, sG, sB, sA=0;
        
        pix = (Uint8 *)surface->pixels + y * surface->pitch + x*3;
        
        dR = *((pix)+rshift8);
        dG = *((pix)+gshift8);
        dB = *((pix)+bshift8);
        dA = *((pix)+ashift8);
        
        sR = (color>>surface->format->Rshift)&0xff;
        sG = (color>>surface->format->Gshift)&0xff;
        sB = (color>>surface->format->Bshift)&0xff;
        sA = (color>>surface->format->Ashift)&0xff;
        
        dR = dR + ((sR-dR)*alpha >> 8);
        dG = dG + ((sG-dG)*alpha >> 8);
        dB = dB + ((sB-dB)*alpha >> 8);
        dA = dA + ((sA-dA)*alpha >> 8);
        
        *((pix)+rshift8) = dR;
        *((pix)+gshift8) = dG;
        *((pix)+bshift8) = dB;
        *((pix)+ashift8) = dA;
        
      }
        break;
        
      case 4: /* Probably 32-bpp */
        pixel = (Uint32*)surface->pixels + y*surface->pitch/4 + x;
        Uint32 dc = *pixel;
        R = color & Rmask;
        G = color & Gmask;
        B = color & Bmask;
        A = 0;  // keep this as 0 to avoid corruption of non-alpha surfaces
        
        
        switch(btype)
        {
          case COMBINE_ALPHA:  // Blend and combine src and dest alpha
            if( alpha != SDL_ALPHA_OPAQUE ){
              R = ((dc & Rmask) + (( R - (dc & Rmask) ) * alpha >> 8)) & Rmask;
              G = ((dc & Gmask) + (( G - (dc & Gmask) ) * alpha >> 8)) & Gmask;
              B = ((dc & Bmask) + (( B - (dc & Bmask) ) * alpha >> 8)) & Bmask;
            }
            if(Amask)
              A = ((((dc & Amask) >> surface->format->Ashift) + alpha) >> 1) << surface->format->Ashift;
            break;
          case DEST_ALPHA:  // Blend and keep dest alpha
            if( alpha != SDL_ALPHA_OPAQUE ){
              R = ((dc & Rmask) + (( R - (dc & Rmask) ) * alpha >> 8)) & Rmask;
              G = ((dc & Gmask) + (( G - (dc & Gmask) ) * alpha >> 8)) & Gmask;
              B = ((dc & Bmask) + (( B - (dc & Bmask) ) * alpha >> 8)) & Bmask;
            }
            if(Amask)
              A = (dc & Amask);
            break;
          case SRC_ALPHA:  // Blend and keep src alpha
            if( alpha != SDL_ALPHA_OPAQUE ){
              R = ((dc & Rmask) + (( R - (dc & Rmask) ) * alpha >> 8)) & Rmask;
              G = ((dc & Gmask) + (( G - (dc & Gmask) ) * alpha >> 8)) & Gmask;
              B = ((dc & Bmask) + (( B - (dc & Bmask) ) * alpha >> 8)) & Bmask;
            }
            if(Amask)
              A = (alpha << surface->format->Ashift);
            break;
          case COPY_SRC_ALPHA: // Direct copy with src alpha
            if(Amask)
              A = (alpha << surface->format->Ashift);
            break;
          case COPY_DEST_ALPHA: // Direct copy with dest alpha
            if(Amask)
              A = (dc & Amask);
            break;
          case COPY_COMBINE_ALPHA: // Direct copy with combined alpha
            if(Amask)
              A = ((((dc & Amask) >> surface->format->Ashift) + alpha) >> 1) << surface->format->Ashift;
            break;
          case COPY_NO_ALPHA:  // Direct copy, alpha opaque
            if(Amask)
              A = (SDL_ALPHA_OPAQUE << surface->format->Ashift);
            break;
          case COPY_ALPHA_ONLY:  // Direct copy of just the alpha
            R = dc & Rmask;
            G = dc & Gmask;
            B = dc & Bmask;
            if(Amask)
              A = (alpha << surface->format->Ashift);
            break;
          case COMBINE_ALPHA_ONLY:  // Blend of just the alpha
            R = dc & Rmask;
            G = dc & Gmask;
            B = dc & Bmask;
            if(Amask)
              A = ((((dc & Amask) >> surface->format->Ashift) + alpha) >> 1) << surface->format->Ashift;
            break;
          case COMBINE_ALPHA_DEST:  // Blend of just the alpha with dest being the fallback
            R = dc & Rmask;
            G = dc & Gmask;
            B = dc & Bmask;
            if(Amask){
              A = ((((dc & Amask) >> surface->format->Ashift) + alpha) >> 1) << surface->format->Ashift;
              if((dc & Amask) == 0){
                A = 0;
              }
              if((alpha) == 0){
                A = 0;
              }
            }
            break;
        }
        
        *pixel = R | G | B | A;
        break;
    }
  }
}

Uint32 Graphics::getPixelInfo(SDL_Surface *s, Sint16 x, Sint16 y){
  switch(s->format->BytesPerPixel){
    case 1:{
      return *((Uint8 *)s->pixels + y*s->pitch +x);
    }
    case 2:{
      return *((Uint16 *)s->pixels + y*s->pitch/2 +x);
    }
    case 3:{
      throw; //not implemented yet
      return 0;
    }
    case 4:{
      return *((Uint32 *)s->pixels + y*s->pitch/4 +x);
    }
    break;
  }
  return 0;
}

void Graphics::createRectInts(SDL_Rect* rect, SDL_Rect* drect, int *lowX, int *lowY, int *highX, int *highY){
  if(rect){
    *lowX = rect->x;
    *highX = rect->x + rect->w;
    *lowY = rect->y;
    *highY = rect->y + rect->h;
  }
  else{
    *lowX = 0;
    *highX = drect->w;
    *lowY = 0;
    *highY = drect->h;
  }
  
}

void Graphics::blendTextures(SDL_Surface *overlay, SDL_Rect *overlayRect, SDL_Surface *dest, SDL_Rect *destRect, BLEND_TYPE blend){
  int lowSX, highSX, lowSY, highSY, lowDX, highDX, lowDY, highDY;
  
  createRectInts(overlayRect, destRect, &lowSX, &lowSY, &highSX, &highSY);
  createRectInts(destRect, destRect, &lowDX, &lowDY, &highDX, &highDY);
  
  //get overlay alpha
  Uint8 overlayAlpha;
  SDL_GetSurfaceAlphaMod(overlay, &overlayAlpha);
  
  //recycling loop variables
  int sx = 0, sy = 0, dx = 0, dy = 0;
  Uint32 color;
  Uint8 r, g, b, a;
  
  //loop through overlay image and gather pixel information
  for(sx=lowSX, sy=lowSY, dx = lowDX, dy = lowDY; dy < highDY;){
    color = getPixelInfo(overlay, sx, sy);
    SDL_GetRGBA(color,overlay->format, &r,&g,&b,&a);
    
    //combine entire surface alpha with the per pixel alpha
    a = (Uint8)(a*(overlayAlpha)/255.0);
    
    //convert color to dest color
    color = SDL_MapRGB(dest->format,r,g,b);
    
    //now blend the pixels
    SDL_LockSurface(dest);
    blendPixelOnSurface(dest, dx, dy, color, a, blend);
    SDL_UnlockSurface(dest);
    
    
    sx++;
    dx++;
    
    if(dx >= highDX){
      sx = lowSX;
      sy++;
      dx = lowDX;
      dy++;
    }
  }

}



void Graphics::blitSurface(SDL_Rect* srcRect, SDL_Rect* destRect, SDL_Texture* srcTexture, double angle, const SDL_Point* point, const SDL_RendererFlip flip){
  int destRight = (destRect->x + destRect->w);
  int destLeft = destRect->x;
  int destTop = destRect->y;
  int destBottom = destRect->y + destRect->h;
  
  destRect->x -= _camera.x;
  destRect->y -= _camera.y;
  
  bool isInCameraRange =
  destRight >= _camera.x &&
  destLeft<= (_camera.x + _camera.w) &&
  destBottom >= _camera.y &&
  destTop <= (_camera.y + _camera.h);
  
  //SDL_SetTextureBlendMode(srcTexture, SDL_BLENDMODE_MOD);
  //SDL_SetTextureAlphaMod(srcTexture, 255);
  int w, h;
  
  //if(destRect->x >= _camera.x && destRect->x <=)
  try{
    if(isInCameraRange && SDL_QueryTexture(srcTexture, NULL, NULL, &w, &h) == 0)
      SDL_RenderCopyEx(_renderer, srcTexture, srcRect, destRect, angle, point, flip);
  }
  catch(std::exception &e){
    std::cout<<"I guess this texture isn't valid??"<<std::endl;
  }
}

/*void Graphics::blitSurfaceAlphaMask(SDL_Rect* srcRect, SDL_Rect* destRect, SDL_Texture* srcTexture, double angle, const SDL_Point* point, const SDL_RendererFlip flip){
  int destRight = (destRect->x + destRect->w);
  int destLeft = destRect->x;
  int destTop = destRect->y;
  int destBottom = destRect->y + destRect->h;
  
  destRect->x -= _camera.x;
  destRect->y -= _camera.y;
  
  bool isInCameraRange =
  destRight >= _camera.x &&
  destLeft<= (_camera.x + _camera.w) &&
  destBottom >= _camera.y &&
  destTop <= (_camera.y + _camera.h);
  
  //SDL_SetTextureAlphaMod(srcTexture, 255);
  
  //if(destRect->x >= _camera.x && destRect->x <=)
  if(isInCameraRange){
    //SDL_SetTextureBlendMode(invDith, SDL_BLENDMODE_ADD);
    //SDL_SetTextureAlphaMod(dith, 255);
    SDL_Rect sr = {0,0,2*37,4*25};
    //SDL_RenderCopy(_renderer,invDith,&sr,destRect);
    //SDL_SetTextureBlendMode(srcTexture, SDL_BLENDMODE_BLEND);
    
    SDL_Texture* renderTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, srcRect->w, srcRect->h);
    SDL_SetRenderTarget(_renderer, renderTexture);
    //SDL_RenderClear(_renderer);
    SDL_Rect copyRect = {0,0,srcRect->w,srcRect->h};
    if(flip == SDL_FLIP_HORIZONTAL){
      SDL_SetTextureBlendMode(srcTexture, SDL_BLENDMODE_NONE);
      SDL_RenderCopy(_renderer, srcTexture, srcRect, &copyRect);
      //SDL_RenderClear(_renderer);
      
      SDL_SetTextureBlendMode(dith, SDL_BLENDMODE_MOD);
      //SDL_SetTextureAlphaMod(dith, 255);
      sr = {0,0,2*37,4*25};
      SDL_RenderCopy(_renderer,dith,&sr,&copyRect);
    }
    else{
      //SDL_RenderClear(_renderer);
      SDL_SetTextureAlphaMod(srcTexture, 255);
      SDL_SetTextureBlendMode(srcTexture, SDL_BLENDMODE_NONE);
      SDL_Rect copyRect = {0,0,srcRect->w,srcRect->h};
      SDL_RenderCopy(_renderer, srcTexture, srcRect, &copyRect);
      //SDL_RenderClear(_renderer);
      
      SDL_SetTextureBlendMode(dith, SDL_BLENDMODE_MOD);
      SDL_SetTextureAlphaMod(dith, 0);
      SDL_SetTextureColorMod(dith, 255, 0, 0);
      sr = {0,0,2*37,4*25};
      SDL_RenderCopy(_renderer,dith,&sr,&copyRect);
    }
    SDL_LockTexture(renderTexture, &copyRect, nullptr, nullptr);
    
    SDL_SetRenderTarget(_renderer, NULL);
    
    //SDL_SetTextureBlendMode(mainTexture, SDL_BLENDMODE_MOD);
    
    SDL_SetRenderTarget(_renderer, NULL);
    SDL_RenderCopyEx(_renderer, renderTexture, &copyRect, destRect, angle, point, flip);
    
    SDL_DestroyTexture(renderTexture);
  }
}*/

void Graphics::blitSurfaceIgnoreCamera(SDL_Rect* srcRect, SDL_Rect* destRect, SDL_Texture* srcTexture, double angle, const SDL_Point* point, const SDL_RendererFlip flip){
  SDL_RenderCopyEx(_renderer, srcTexture, srcRect, destRect, angle, point, flip);
}

void Graphics::blitSurfaceRenderText(std::string text, const char* fontLoc, int size, SDL_Color color, Vector2 position, SDL_Surface* preloadedSurface){
  if (font == NULL){
    printf("Error: %s", TTF_GetError());
    font = TTF_OpenFont(fontLoc, size*SPRITE_SCALE);
    messageSurface = NULL;
  }
  
  if(preloadedSurface == NULL){
    messageSurface = TTF_RenderText_Solid(font, text.c_str(), color);
  }
  else
  {
    messageSurface = preloadedSurface;
  }
  SDL_Texture* message = SDL_CreateTextureFromSurface(_renderer, messageSurface);
  
  
  //get the text size
  int width;
  int height;
  
  if(TTF_SizeText(font, text.c_str(), &width, &height) != 0){
    //failure
    printf("TTF_Error: %s\n",TTF_GetError());
    width = 100;
    height = 100;
  }
  
  SDL_Rect rect = {position.x, position.y, width, height};
  SDL_RenderCopy(_renderer, message, NULL, &rect);
  //SDL_FreeSurface(messageSurface);
  SDL_DestroyTexture(message);
  //SDL_free(message);
  
}

void Graphics::getTextSize(int *width, int *height, std::string message){
  TTF_SizeText(font, message.c_str(), width, height);
}

void Graphics::fillScreen(SDL_Color color){
  SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
  SDL_RenderClear(_renderer);
  SDL_RenderPresent(_renderer);
}

void Graphics::setCamera(SDL_Rect *camera){
  _camera.x = camera->x;
  _camera.y = camera->y;
  _camera.w = camera->w;
  _camera.h = camera->h;
}

SDL_Rect Graphics::getCamera(){
  return _camera;
}

void Graphics::render(){
  SDL_RenderPresent(_renderer);
}

void Graphics::clear(){
  SDL_RenderClear(_renderer);
}

SDL_Renderer* Graphics::getRenderer() const{
  return this->_renderer;
}

SDL_Window* Graphics::getWindow() const{
  return this->_window;
}
