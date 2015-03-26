#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
using namespace std;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* currentSurface = NULL;


enum KeyPressSurfaces{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

//Starts up SDL & Creates window
bool init();

//Loads Media
bool loadMedia();

//Frees Media & shuts down SDL
void close();

//Load Individual Image
SDL_Surface* loadSurface( std::string path );

//Load Individual Image as Texture
SDL_Texture* loadTexture( std:: string path );

//The images that correspond to a keypress
SDL_Surface* keypressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

int main( int argc, char* args[] ){

	bool quit = false;
	SDL_Event e;
	currentSurface = keypressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];	


	if(!init()){
		printf("failed to initialize\n");
	} else {
		if(!loadMedia()){
			printf("failed to load media!\n");
		} else {

			SDL_BlitSurface(currentSurface, NULL, screenSurface, NULL);
			SDL_UpdateWindowSurface( window );

			while( !quit ){
				while( SDL_PollEvent( &e ) != 0){
					if(e.type == SDL_QUIT){
						quit = true;
					} else if(e.type == SDL_KEYDOWN){
						switch(e.key.keysym.sym){
							case SDLK_UP:
								currentSurface = keypressSurfaces[KEY_PRESS_SURFACE_UP];
								break;
							case SDLK_DOWN:
								currentSurface = keypressSurfaces[KEY_PRESS_SURFACE_DOWN];
								break;
							case SDLK_LEFT:
								currentSurface = keypressSurfaces[KEY_PRESS_SURFACE_LEFT];
								break;
							case SDLK_RIGHT:
								currentSurface = keypressSurfaces[KEY_PRESS_SURFACE_RIGHT];
								break;
							default:
								currentSurface = keypressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
								break;
						}
					}
				}
				SDL_Rect stretchRect;
				stretchRect.x = 0;
				stretchRect.y = 0;
				stretchRect.w = SCREEN_WIDTH;
				stretchRect.h = SCREEN_HEIGHT;	
				
				SDL_BlitScaled(currentSurface, NULL, screenSurface, &stretchRect);
				SDL_UpdateWindowSurface( window );
			}
			SDL_Rect stretchRect;
			stretchRect.x = 0;
			stretchRect.y = 0;
			stretchRect.w = SCREEN_WIDTH;
			stretchRect.h = SCREEN_HEIGHT;	
			SDL_BlitScaled(currentSurface, NULL, screenSurface, &stretchRect);
			SDL_UpdateWindowSurface( window );
			SDL_Delay ( 2000 );
		}
	}

	close();
	return 0;

}
SDL_Surface* loadSurface( std::string path ){

	SDL_Surface* optimizedSurface = NULL;

	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	if( loadedSurface == NULL ){
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	} else {
		optimizedSurface = SDL_ConvertSurface( loadedSurface, screenSurface->format, NULL);
		if(optimizedSurface == NULL){
			printf("unable to optimize image %s! SDL ERROR: %s\n", path.c_str(), SDL_GetError() );
		}
		SDL_FreeSurface(loadedSurface);
	}
	return optimizedSurface;
}

bool loadMedia(){
	bool success = true;
	keypressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface( "dog.bmp" );
	if(keypressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL){
		printf("failed to load default image!\n");
		success = false;
	}

	keypressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("human.bmp");
	if(keypressSurfaces[KEY_PRESS_SURFACE_UP] == NULL){
		printf("failed to load up image!\n");
		success = false;
	}

	keypressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("cats.bmp");
	if(keypressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL){
		printf("failed to load down image!\n");
		success = false;
	}

	keypressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("house.bmp");
	if(keypressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL){
		printf("failed to load left image!\n");
		success = false;
	}

	keypressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("books.bmp");
	if(keypressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL){
		printf("failed to load rightt image!\n");
		success = false;
	}

	return success;
}


bool init(){
	//initialization flag
	bool success = true;

	if(SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	} else {
		window = SDL_CreateWindow( "SDL TUTORIAL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if(window == NULL){
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		} else {
			screenSurface = SDL_GetWindowSurface( window );
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF) );
			SDL_UpdateWindowSurface( window );
		}
	}
	
	return success;
}


void close(){
	SDL_FreeSurface(currentSurface);
	currentSurface = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
}

