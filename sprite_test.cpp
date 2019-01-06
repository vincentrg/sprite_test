#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <stdio.h>
#include <string>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_SCALE 1
#define SCREEN_NAME "Sprite Test"

SDL_Window* gameWindow = NULL;
SDL_Renderer* gameRenderer = NULL;

int movementSpeed = 4;


class SpriteTexture {
	public:
		// Constructor
		SpriteTexture();

		// Destructor
		~SpriteTexture();

		void free();

		// Load an image
		void loadImage(std::string imgPath, int newNumFrames, int newNumCol, int newNumRow, int newFrameRate);

		// Render a sprite
		void renderSprite(int x, int y);

		// Increment the currentFrame count, resetting if reaching the numFrames
		// Returns true when reset current fram back to 0
		bool updateCurrentFrame();

		// Update current frame with an idle reset upon completion
		// STUPID IDEA BUT KEEP IN CASE *!!!*
		void updateFrameWithReset(bool &phase);

		// Return the center x position of a sprite frame
		int centerX();

		// Return the center y position of a sprite frame
		int centerY();


	private:
		int currentFrame;
		int numFrames;
		SDL_Texture* spriteSheet;
		int numRow;
		int numCol;
		int width;
		int height;
		SDL_Rect* spriteFrames;
		int frameRate;
};

SpriteTexture::SpriteTexture() {
	currentFrame = 0;
	numFrames = 0;
	spriteSheet = NULL;
	numCol = 0;
	numRow = 0;
	width = 0;
	height = 0;
	spriteFrames = NULL;
	frameRate = 0;
}

SpriteTexture::~SpriteTexture() {
	free();
}

void SpriteTexture::free() {
	if (spriteSheet != NULL) {
		SDL_DestroyTexture(spriteSheet);
		currentFrame = 0;
		numFrames = 0;
		spriteSheet = NULL;
		numCol = 0;
		numRow = 0;
		width = 0;
		height = 0;
		frameRate = 0;
	}
	if (spriteFrames != NULL) {
		delete[] spriteFrames;
	}
}

void SpriteTexture::loadImage(std::string imgPath, int newNumFrames, int newNumCol, int newNumRow, int newFrameRate) {
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* newSurface = IMG_Load(imgPath.c_str());
	if (newSurface == NULL) {
		printf("SDL_IMAGE ERROR: %s\n", IMG_GetError());
		exit(1);
	}

	// Set image colour key if needed
	//SDL_SetColorKey(newSurface, SDL_TRUE, SDL_MapRGB(newSurface->format, 0x00, 0x00, 0x00));

	newTexture = SDL_CreateTextureFromSurface(gameRenderer, newSurface);
	if (newTexture == NULL) {
		printf("SDL ERROR: %s\n", SDL_GetError());
		exit(1);
	}

	width = newSurface->w;
	height = newSurface->h;
	numFrames = newNumFrames;
	numCol = newNumCol;
	numRow = newNumRow;
	currentFrame = 0;
	spriteFrames = new SDL_Rect[numFrames];
	frameRate = newFrameRate;

	SDL_FreeSurface(newSurface);

	spriteSheet = newTexture;

	//Set sprite clips
	for (int i = 0; i < numFrames; i++) {
		spriteFrames[i].x = (i%numCol)*(width/numCol);
		spriteFrames[i].y = (i/numCol)*(height/numRow);
		spriteFrames[i].w = width/numCol;
		spriteFrames[i].h = height/numRow;
	}
}

void SpriteTexture::renderSprite(int x, int y) {

	// Get current sprite frame
	SDL_Rect* spriteFrame = &spriteFrames[currentFrame/frameRate];

	// Set rendering space
	SDL_Rect newRect = {x, y, width, height};

	// Choose frame to render
	newRect.w = spriteFrame->w;
	newRect.h = spriteFrame->h;

	//Render to screen
	SDL_RenderCopy( gameRenderer, spriteSheet, spriteFrame, &newRect );
}

bool SpriteTexture::updateCurrentFrame() {
	currentFrame++;
	if ((currentFrame/frameRate) >= numFrames) {
		currentFrame = 0;
		return true;
	}
	return false;
}

void SpriteTexture::updateFrameWithReset(bool &phase) {
	currentFrame++;
	if ((currentFrame/frameRate) >= numFrames) {
		phase = true;
		currentFrame = 0;
	}
}

int SpriteTexture::centerX() {
	return ((SCREEN_WIDTH - (width/numCol)) / 2);
}

int SpriteTexture::centerY() {
	return ((SCREEN_HEIGHT - (height/numRow)) / 2);
}

void initGame() {

	// Initialize SDL
	// SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) example to specify what to init
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL ERROR: %s\n", SDL_GetError());
		exit(1);
	}

	// Enable linear filtering
	if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") == SDL_FALSE) {
		printf("Linear filtering could not be enabled\n");
	}

	// Intialize game window
	gameWindow = SDL_CreateWindow(SCREEN_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH*SCREEN_SCALE, SCREEN_HEIGHT*SCREEN_SCALE, SDL_WINDOW_SHOWN);
	if (gameWindow == NULL) {
		printf("SDL ERROR: %s\n", SDL_GetError());
		exit(1);
	}

	// Initialize renderer
	gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	if (gameRenderer == NULL) {
		printf("SDL ERROR: %s\n", SDL_GetError());
		exit(1);
	}

	// Intialize render colour
	if (SDL_SetRenderDrawColor(gameRenderer, 0x00, 0x00, 0x00, 0xFF) < 0) {
		printf("SDL ERROR: %s\n", SDL_GetError());
		exit(1);
	}

	// Enable specific image loading
	int imgFlags = IMG_INIT_PNG; // IMG_INIT_PNG|IMG_INIT_JPG
	int initBits = IMG_Init(imgFlags);
	if ((initBits&imgFlags) != imgFlags) {
		printf("SDL ERROR: %s\n", SDL_GetError());
		exit(1);
	}

	printf("Game initialized\n");
}

void quitGame() {
	SDL_DestroyRenderer(gameRenderer);
	SDL_DestroyWindow(gameWindow);
	gameRenderer = NULL;
	gameWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}



int main(int argc, char* args[]) {
	// Initialize game
	initGame();

	// Load media

	// Load sprites
	//SpriteTexture fryAttack;
	//fryAttack.loadImage("302001406_atk.png", 22, 3, 8, 4);

	//SpriteTexture fryIdle;
	//fryIdle.loadImage("302001406_idle.png", 4, 3, 2, 8);

	SpriteTexture fryChange;
	fryChange.loadImage("302001406_idle.png", 4, 3, 2, 8);
	bool idlePhase = true;
	bool doneAttacking = true;

	int posx = fryChange.centerX();
	int posy = fryChange.centerY();
	
	bool upOn = false;
	bool downOn = false;
	bool leftOn = false;
	bool rightOn = false;
	

	// Initialize event handler
	SDL_Event event;

	// Run game loop
	SDL_bool running = SDL_TRUE;
	while(running) {

		// Handle events
		while(SDL_PollEvent(&event)) {

			// Quit event
			if(event.type == SDL_QUIT) {
				running = SDL_FALSE;
				break;
			}

			// Keydown event
			// 	- movement
			// 	- sprite change
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym ==  SDLK_ESCAPE) {
					running = SDL_FALSE;
					break;
				}
				else if (event.key.keysym.sym ==  SDLK_a) {
					if (idlePhase) {
						idlePhase = false;
						doneAttacking = false;
						fryChange.loadImage("302001406_atk.png", 22, 3, 8, 4);
						posx = posx - 154;
						posy = posy - 17;
						break;
					}
				}
				else if (event.key.keysym.sym ==  SDLK_i) {
					if (!idlePhase) {
						idlePhase = true;
						fryChange.loadImage("302001406_idle.png", 4, 3, 2, 8);
						posx = posx + 154;
						posy = posy + 17;
						break;
					}
				}

				
				if (event.key.keysym.sym ==  SDLK_UP) {
					upOn = true;
				}
				if (event.key.keysym.sym ==  SDLK_DOWN) {
					downOn = true;
				}
				if (event.key.keysym.sym ==  SDLK_LEFT) {
					leftOn = true;
				}
				if (event.key.keysym.sym ==  SDLK_RIGHT) {
					rightOn = true;
				}
			}

			// Keyup event
			// 	- movement
			if (event.type == SDL_KEYUP) {
				if (event.key.keysym.sym ==  SDLK_UP) {
					upOn = false;
				}
				if (event.key.keysym.sym ==  SDLK_DOWN) {
					downOn = false;
				}
				if (event.key.keysym.sym ==  SDLK_LEFT) {
					leftOn = false;
				}
				if (event.key.keysym.sym ==  SDLK_RIGHT) {
					rightOn = false;
				}
			}

		}

		// Handle non-SDL events

		// Movement
		if (upOn) {
			posy = posy - movementSpeed;
		}
		if (downOn) {
			posy = posy + movementSpeed;
		}
		if (leftOn) {
			posx = posx - movementSpeed;
		}
		if (rightOn) {
			posx = posx + movementSpeed;
		}

		// Changing back to idle once done attacking
		if (doneAttacking && !idlePhase) {
			fryChange.loadImage("302001406_idle.png", 4, 3, 2, 8);
			posx = posx + 154;
			posy = posy + 17;
			idlePhase = true;
		}


		// Reset screen
		SDL_SetRenderDrawColor(gameRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gameRenderer);


		// Frames to render
		//fryAttack.renderSprite(100, 100);
		//fryIdle.renderSprite(100, 200);
		//fryChange.renderSprite(fryChange.centerX(), fryChange.centerY());
		fryChange.renderSprite(posx, posy);

		// Update screen
		SDL_RenderPresent(gameRenderer);

		// Update frames
		//fryAttack.updateCurrentFrame();
		//fryIdle.updateCurrentFrame();
		doneAttacking = fryChange.updateCurrentFrame();
		//fryChange.updateFrameWithReset(doneAttacking);

	}

	// Free any loaded image memory
	// ERRORS WHEN USING, DOUBLE FREE OR CORRUPTION *!*
	//fryAttack.free();
	//fryIdle.free();
	//fryChange.free();

	// Quit game
	quitGame();

	return 0;
}


