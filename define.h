//Window config
#define W_WIDTH			W_HEIGHT*16/9
#define W_HEIGHT		900
#define S_SIZE			16
	
//Game config
#define G_WIDTH			(3*W_WIDTH/(4*S_SIZE))
#define G_HEIGHT		(W_HEIGHT/S_SIZE)
#define SEED			time(NULL)
#define NB_PLAYER		50
#define NB_APPLE		1
#define WEIGHT_REF		(G_WIDTH+G_HEIGHT)

//Snake directions
#define NONE			0
#define LEFT			-1
#define RIGHT			1
#define DOWN			-2
#define UP				2

//Keybinds
#define K_PAUSE			Keyboard::Space
#define K_UP_FPS		Keyboard::Add
#define K_DOWN_FPS		Keyboard::Subtract
#define K_FPS_CAP		Keyboard::Multiply
#define K_TOGGLE_BFS	Keyboard::Numpad0
#define K_SHOW_COST		Keyboard::Numpad1
#define K_SHOW_WEIGHT	Keyboard::Numpad2
#define K_SHOW_PATH		Keyboard::Numpad3

//Ressources
#define FONT_TITLE		"fonts/NEON_LIGHTS.ttf"
#define FONT_SCORE		"fonts/arcade.ttf"
#define T_SNAKE			"img/snake.png"
#define T_APPLE			"img/apple.png"
#define T_SIZE			32