//Window config
#define W_WIDTH			W_HEIGHT*16/9
#define W_HEIGHT		900
#define S_SIZE			16
	
//Game config
#define G_WIDTH			(3*W_WIDTH/(4*S_SIZE))
#define G_HEIGHT		(W_HEIGHT/S_SIZE)
#define SEED			time(NULL)
#define NB_PLAYER		1
#define NB_APPLE		1
#define WEIGHT_REF		(G_WIDTH+G_HEIGHT)

//Snake directions
#define NONE			0
#define LEFT			-1
#define RIGHT			1
#define DOWN			-2
#define UP				2

//Ressources
#define FONT			"fonts/arcade.ttf"
#define T_SNAKE			"img/snake.png"
#define T_APPLE			"img/apple.png"
#define T_SIZE			32