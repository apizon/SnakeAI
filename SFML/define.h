// Window config
// I recommand only changing the height and let the width adapt to it
// otherwise the score display is all messed up
#define W_WIDTH			1280
#define W_HEIGHT		720
// Sprite display size, change if you want bigger/smaller sprites
// Don't touch to T_SIZE though
#define S_SIZE			16


// Game config
#define G_WIDTH			(3*W_WIDTH/(4*S_SIZE))
#define G_HEIGHT		(W_HEIGHT/S_SIZE)
#define SEED			time(NULL)				// For the RNG
#define NB_PLAYER		1
#define NB_APPLE		1
// Maximum weight of a node (= weight of snake body parts)
// I set it to G_WIDTH+G_HEIGHT to be sure there will be no negative values
#define WEIGHT_REF		(G_WIDTH+G_HEIGHT)


// Snake directions
// Used to facilitate some calculations, don't touch
#define NONE			0
#define LEFT			-1
#define RIGHT			1
#define DOWN			-2
#define UP				2


// Controlls 
// Change as you will
#define M_ADD_OBS		Mouse::Left
#define M_DEL_OBS		Mouse::Right
#define K_PAUSE			Keyboard::Space
#define K_UP_FPS		Keyboard::Add
#define K_DOWN_FPS		Keyboard::Subtract
#define K_FPS_CAP		Keyboard::Multiply
#define K_TOGGLE_BFS	Keyboard::Numpad0
#define K_SHOW_COST		Keyboard::Numpad1
#define K_SHOW_WEIGHT	Keyboard::Numpad2
#define K_SHOW_PATH		Keyboard::Numpad3


// Ressources
// Score display is set almost pixel perfect with ugly calculations
// so i wouldn't recommand changing the fonts
#define FONT_TITLE		"fonts/NEON_LIGHTS.ttf"
#define FONT_SCORE		"fonts/arcade.ttf"
#define T_SNAKE			"img/snake.png"
#define T_APPLE			"img/apple.png"
// Tile size on the image.png, if you want to change the display size 
// change S_SIZE, don't touch this one
#define T_SIZE			32