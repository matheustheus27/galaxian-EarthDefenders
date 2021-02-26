//Variaveis de Objestos
typedef struct{
	float MaiX;
	float MaiY;
	float MenX;
	float MenY;
	float base;
}Coord;
typedef struct{
	/*Configurações Disparo*/
	//Estado
	bool rend;

	//Movimento
	float VelY;

	//Tipo
	int Disp;
	int Dano;

	//Dimensões
	float X;
	float Y;
	float Comp;
	float Alt;

	//Textura
	int FrameAtual;
	Coord tamFrame;
	GLuint textura;

} Bullet;

typedef struct{
	/*Configurações Player*/
	//Estado
	bool rend;

	//Vidas
	int Vidas;

	//Quantidade de Disparos
	int Disp;

	//Dimensões
	float X;
	float Y;
	float Comp;
	float Alt;

	//Disparos
	Bullet bul[500];
	GLuint texturaBulletAtual;

	//Textura
	int FrameAtual;
	Coord tamFrame;
} Player;


typedef struct{
	/*Configurações Inimigo*/
	//Estado
	bool rend;

	//Vidas
	int Vidas;

	//Quantidade de Disparos
	int Disp;
	int bulTime;
	int auxbulTime;

	//Movimento
	float VelX;

	//Dimensões
	float X;
	float Y;
	float Comp;
	float Alt;

	//Disparos
	Bullet bul[500];

	//Textura
	int FrameAtual;
	Coord tamFrame;
	GLuint textura;
}Enemy;

typedef struct{
	//Estado
	bool rend;

	//Dimensões
	float X;
	float Y;
	float Comp;
	float Alt;

	//Textura
	int FrameAtual;
	Coord tamFrame;
} Explosion;

typedef struct{
	//Estado
	bool rend;

	//Tipo
	int pup;

	//Dimensões
	float X;
	float Y;
	float Comp;
	float Alt;

	//Textura
	Coord tamFrame;
	GLuint textura;
} Powerup;

typedef struct{
	//Estado
	bool rend;

	//Vidas
	int Vidas;

	//Dimensões
	float X;
	float Y;
	float Comp;
	float Alt;

	//Textura
	int FrameAtual;
	Coord tamFrame;
	GLuint textura;
} Asteroid;

//Variaveis de texturas
GLuint startMenu;
GLuint pauseMenu;
GLuint controlsScreen;
GLuint gameOver;
GLuint creditosMenu;
Coord startCoord;
Coord pauseCoord;
Coord controlsCoord;
Coord gameOverCoord;
Coord backgroundCoord;
Coord creditosCoord;
Coord asteroidCoord;
Coord playerLifeCoord;
Coord contadorPontuacaoCoord[5];

GLuint background;
GLuint scene_asteroid;
GLuint explosion;
GLuint playerLife;
GLuint playerPowerup[4];
GLuint contadorPontuacao[5];


GLuint player;
GLuint playerBullet[3];
GLuint enemy[3];
GLuint enemyBullet;
GLuint tBoss[4];

SDL_Surface* principalDisplay;
SDL_Surface* icon;

//Variaveis de Musica
Mix_Chunk *opennigMus;
Mix_Chunk *gameNameMus;
Mix_Chunk *gameStartMus;
Mix_Chunk *gameOverMus;
Mix_Chunk *navigatingMus;

Mix_Chunk *explosionMus;
Mix_Chunk *bossMus;
Mix_Chunk *firingMus;