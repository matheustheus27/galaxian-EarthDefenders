#include <SDL/SDL.h>
#include "SDL/SDL_opengl.h"
#include "SDL/SDL_image.h"
#include <SDL/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include "objectsGalaxian.h"
#include <stdlib.h>
#include <string.h>
#include <SOIL/SOIL.h>
#include <time.h>

//Variaveis Globais
int QTDINI = 16;
int QTDMAXDISP = 500;
float VELINI = 2;
int PPONTOS = 0;

//---------------------------------------------------- Carregamento de Texturas ------------------------------------------------------------
GLuint carregaTextura(const char* arquivo) {
  GLuint idTextura = SOIL_load_OGL_texture(
                     arquivo,
                     SOIL_LOAD_AUTO,
                     SOIL_CREATE_NEW_ID,
                     SOIL_FLAG_INVERT_Y
                    );

  if (idTextura == 0) {
    printf("Erro do SOIL: '%s'\n", SOIL_last_result());
  }

  return idTextura;
} 
void defineTextura(){
	//Menus
	pauseMenu = carregaTextura("resources/sprites/others/pause_menu.png");//Sprite Menu de Pausa
	startMenu = carregaTextura("resources/sprites/others/start_screen.png");//Sprite Menu Inicial
	creditosMenu = carregaTextura("resources/sprites/others/credits.png");//Sprite Menu de Creditos
	controlsScreen = carregaTextura("resources/sprites/others/controls_screen.png");//Sprite de Controles
	gameOver = carregaTextura("resources/sprites/others/game_over.png");//Sprite Game Over

	//Cenário
	background = carregaTextura("resources/sprites/others/background_space.png");//Sprite Fundo
	scene_asteroid = carregaTextura("resources/sprites/others/asteroid.png");//Sprite Asteroide
	explosion = carregaTextura("resources/sprites/others/explosao.png");//Sprite Explosão
	
	//Player Status
	playerLife = carregaTextura("resources/sprites/player/player_lifebar.png");//Sprite Barra de Vida do Jogador
	playerPowerup[0] = carregaTextura("resources/sprites/player/player_getlife.png");//Sprite Powerup (Vida)
	playerPowerup[1] = carregaTextura("resources/sprites/player/player_getgun1.png");//-------------- (Tiro Fraco)
	playerPowerup[2] = carregaTextura("resources/sprites/player/player_getgun2.png");//-------------- (Tiro Medio)
	playerPowerup[3] = carregaTextura("resources/sprites/player/player_getgun3.png");//-------------- (Tiro Forte)

	//Contador de Pontuação
	contadorPontuacao[0] = carregaTextura("resources/sprites/others/contador.png");//Sprite Pontuação
	contadorPontuacao[1] = carregaTextura("resources/sprites/others/contador.png");//----------------
	contadorPontuacao[2] = carregaTextura("resources/sprites/others/contador.png");//----------------
	contadorPontuacao[3] = carregaTextura("resources/sprites/others/contador.png");//----------------
	contadorPontuacao[4] = carregaTextura("resources/sprites/others/contador.png");//----------------

	//Textura dos Personagens
	/*Player*/
	player = carregaTextura("resources/sprites/player/player_spacecraft.png");//Nave do Jogador
	playerBullet[0] = carregaTextura("resources/sprites/player/player_bullet01.png");//Tiro do Jogador
	playerBullet[1] = carregaTextura("resources/sprites/player/player_bullet02.png");//--------------
	playerBullet[2] = carregaTextura("resources/sprites/player/player_bullet03.png");//--------------
	/*Enemy*/
	enemy[0] = carregaTextura("resources/sprites/enemy/enemy_spacecraft01.png");//Sprites Inimigos
	enemy[1] = carregaTextura("resources/sprites/enemy/enemy_spacecraft02.png");//-----------------
	enemy[2] = carregaTextura("resources/sprites/enemy/enemy_spacecraft03.png");//-----------------
	enemyBullet = carregaTextura("resources/sprites/enemy/enemy_bullet01.png"); //Tiro dos Inimigos
	/*Boss*/
	tBoss[0] = carregaTextura("resources/sprites/enemy/boss/boss_matheus.png");//Sprite dos Chefões
	tBoss[1] = carregaTextura("resources/sprites/enemy/boss/boss_filipe.png");//-------------------
	tBoss[2] = carregaTextura("resources/sprites/enemy/boss/boss_thiago.png");//-------------------
	tBoss[3] = carregaTextura("resources/sprites/enemy/boss/boss_maria.png");//--------------------

	//Musicas
	opennigMus = Mix_LoadWAV("resources/sounds/opening_soundtrack.wav"); //Musica de fundo
	gameNameMus = Mix_LoadWAV("resources/sounds/earth_defenders.wav");	//Som com o nome do jogo *Executado na tela inicial*
	navigatingMus = Mix_LoadWAV("resources/sounds/navigating_menu.wav"); //Som de movimento entre as opções do Menu principal
	gameStartMus = Mix_LoadWAV("resources/sounds/ready_go.wav");	//Som executado quando uma partida é iniciada
	gameOverMus = Mix_LoadWAV("resources/sounds/game_over.wav");	//Som executado quando o jogador perde
	explosionMus = Mix_LoadWAV("resources/sounds/explosion_noise.wav"); //Som executado quando algo explode
	bossMus = Mix_LoadWAV("resources/sounds/laughter_boss.wav");	//Som reproduzido pelo Chefão
	firingMus = Mix_LoadWAV("resources/sounds/firing_noise.wav");	//Som de tiro

}

//---------------------------------------------------- Verificações ------------------------------------------------------------
/*Verifica a quantidade de inimigos restantes*/
bool verificaInimigos(Enemy *en){
	int qtdEnemyM = 0;
	for(int i = 0; i < QTDINI; i++){
		if(en[i].rend == false){
			qtdEnemyM++;
		}
	}

	if(qtdEnemyM == QTDINI){
		return true;
	}

	return false;
}
/*Realiza o sorteio de um numero*/
bool verificaSorteio(){
	int sorteio = rand()%500;
	if(sorteio == 7){
		return true; //Retorna verdadeiro caso o numero sorteado seja 7
	}

	return false; //Retorna falso caso o numero sorteado seja diferente 7
}
/*Verifica se aconteceu uma colisão entre dois objetos*/
bool colisao(float bulletX, float bulletY, float bulletComp, float bulletAlt, float personX, float personY, float personComp, float personAlt){
	if(bulletY+bulletAlt < personY){
		return false;
	}else if(bulletY > personY+personAlt){
		return false;
	}else if(bulletX+bulletComp < personX){
		return false;
	}else if(bulletX > personX+personComp){
		return false;
	}

	return true;
}
//---------------------------------------------------- Criação de Objetos ------------------------------------------------------------
/*Gera a munição utilizado pelos inimigos ou pelo player*/
void criaBullet(Bullet *b, GLuint textura){
	/*Configuração Bullet*/

	for(int j = 0; j < QTDMAXDISP; j++){
			//Movimento
			b[j].VelY = 2;
			//Dimensões
			b[j].X = 0;
			b[j].Y = 0;
			b[j].Comp = 16;
			b[j].Alt = 16;

			//Textura
			b[j].FrameAtual = 1;
			b[j].textura = textura;

			//Dano
			b[j].Dano = 1;

			//Estado
			b[j].rend = false;
		}
}
/*Gera um vetor de inimigos*/
void criaInimigos(Enemy *en){
	/*Configurações Inimigo*/
	int cont = 1;
	int sup = 2;
	for(int i = 0, x = 1, y = 6; i < QTDINI; i++, x = x + 66){
		if(x > 560){
			x = 1;
			y = y + 64;
			cont++;
		}
		int aux = rand()%3; //Obtem um numero de 0 até 3 para definir a quantidade de vida e a skin do inimigo
		//Quantidade de Vidas
		en[i].Vidas = aux;
		//Movimento
		if(cont != sup){
			en[i].VelX = VELINI;
		}else{
			en[i].VelX = -VELINI;
		}

		//Dimensões
		en[i].X = x;
		en[i].Y = y;
		en[i].Comp = 64;
		en[i].Alt = 64;

		//Textura
		en[i].FrameAtual = 1;
		en[i].textura = enemy[aux];
		//Tempo Disparo
		en[i].auxbulTime = (rand()%45)+50; //Define aleatoriamete o tempo que o inimigo leva para atirar
		en[i].bulTime = 0;

		//Estado
		en[i].rend = true;

		/*Configurações Disparo do Inimigo*/
		//Quantidade Disparada
		en[i].Disp = 0;
		//Munição
		criaBullet(en[i].bul, enemyBullet);
	}
}
/*Cria o Jogador*/
Player criaPlayer(){
	/*Configurações Player*/
	Player pl;
	//Quantidade de Vidas
	pl.Vidas = 3;
	//Dimensões
	pl.X = 300;
	pl.Y = 538;
	pl.Comp = 64;
	pl.Alt = 64;

	//Textura
	pl.FrameAtual = 1;
	pl.texturaBulletAtual = playerBullet[0];
	/*Configurações Disparo Player*/
	//Quantidade Disparada
	pl.Disp = 0;
	//Munição
	criaBullet(pl.bul, pl.texturaBulletAtual);

	return pl;
}
/*Cria o Chefão*/
Enemy criaBoss(){
	/*Configurações Boss*/
	Enemy bo;

	//Quantidade de Vidas
	bo.Vidas = 10;

	//Movimento
	bo.VelX = 5;

	//Dimensões
	bo.X = 300;
	bo.Y = 20;
	bo.Comp = 64;
	bo.Alt = 64;

	//Textura
	bo.FrameAtual = 1;
	bo.textura = tBoss[rand()%4]; //Define aleatoriamente a skin do Chefão


	//Estado
	bo.rend = false;
	
	/*Configurações Disparo Boss*/
	//Tempo Disparo
	bo.auxbulTime = (rand()%20)+10; //Define aleatoriamente o tempo que o Chefão demora para disparar, o tempo é menor do que os inimigos
	bo.bulTime = 0;

	//Quantidade Disparada
	bo.Disp = 0;

	//Munição
	criaBullet(bo.bul, enemyBullet);

	return bo;
}
/*Cria a Explosão*/
Explosion criaExplosion(Explosion *ex){
	/*Configurações Explosões*/
	for(int i = 0; i < QTDINI+2; i++){
		//Estado
		ex[i].rend = false;

		//Dimensões
		ex[i].X = 0;
		ex[i].Y = 0;
		ex[i].Comp = 0;
		ex[i].Alt = 0;

		//Textura
		ex[i].FrameAtual = 1;
	}
}
/*Cria o Powerup*/
Powerup criaPowerup(){
	Powerup poup;
	//Estado
	poup.rend = false;

	//Tipo
	poup.pup = rand()%4; //Faz o sorteio o Powerup / 0 - Vida / 1 - Tiro fraco / 2 - Tiro medio / 3 - Tiro forte 

	//Dimensões
	poup.X = (rand()%545) -4; //Define aleatoriamente onde ele deve aparecer
	poup.Y = 6;
	poup.Comp = 32;
	poup.Alt = 32;

	//Textura
	poup.tamFrame.MaiX = 1;
	poup.tamFrame.MaiY = 0;
	poup.tamFrame.MenX = 0;
	poup.tamFrame.MenY = 1;
	poup.textura = playerPowerup[poup.pup];

	return poup;
}
/*Cria o Asteroide*/
Asteroid criaAsteroid(){
	Asteroid as;
	//Estado
	as.rend = false;

	//Quantidade de Vidas
	as.Vidas = 6;
	//Dimensões
	as.X = (rand()%545) -4; //Define aleatoriamente onde ele deve aparecer
	as.Y = 6;
	as.Comp = 45;
	as.Alt = 45;

	//Textura
	as.FrameAtual = 1;
	as.tamFrame.MaiX = 1;
	as.tamFrame.MaiY = 0.5;
	as.tamFrame.MenX = 0;
	as.tamFrame.MenY = 1;
	as.textura = scene_asteroid;

	return as;
}

//---------------------------------------------------- Posicionamento para Renderização ------------------------------------------------------------
void renderizarObjetos(float personX, float personY, float personComp, float personAlt, Coord frame, GLuint personTex){
	int red = 0, green = 0, blue = 0;
	//Matriz
	glPushMatrix(); //Inicia

		glOrtho(0, 600, 600, 0, -1, 1); //Dimensões da Matriz
		glColor4ub(255, 255, 255, 255); //Cor Branca

		glEnable(GL_TEXTURE_2D); 
			glBindTexture(GL_TEXTURE_2D, personTex);
			glBegin(GL_QUADS); //Inicia Desenho
				
				glTexCoord2d(frame.MenX, frame.MenY);	 glVertex2f(personX, personY);
				glTexCoord2d(frame.MaiX, frame.MenY);	 glVertex2f(personX+personComp, personY);
				glTexCoord2d(frame.MaiX, frame.MaiY);	 glVertex2f(personX+personComp, personY+personAlt);
				glTexCoord2d(frame.MenX, frame.MaiY);	 glVertex2f(personX, personY+personAlt);
			glEnd(); //Encerra Desenho
		glDisable(GL_TEXTURE_2D);

	glPopMatrix(); //Fecha
}

//---------------------------------------------------- Configurações da Aplicação ------------------------------------------------------------
/*Define as configurações do consumo de memoria*/
void controleMemoria(){
	//Controle de Memória
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}
/*Define as configurações da janela*/
void confJanela(){
	//Configurações da Janela
	SDL_WM_SetCaption("Earth Defenders", NULL); //Nome
	icon = SDL_LoadBMP("resources/sprites/others/icon.bmp");
	SDL_WM_SetIcon(icon, NULL); //Adicona icone na janela
	principalDisplay = SDL_SetVideoMode(600, 600, 32, SDL_OPENGL); //Tamanho
	glClearColor(1, 1, 1, 1); //Cor de fundo
	glViewport(0, 0, 600, 600); //Área de Exibição
	glShadeModel(GL_SMOOTH); //Sombreamento
	glMatrixMode(GL_PROJECTION); //2D
	glLoadIdentity(); //Desenho Geometrico
	glDisable(GL_DEPTH_TEST); //3D
	glEnable(GL_BLEND);//Uso da Imagem
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Uso da Imagem
}

//---------------------------------------------------- Funções para carregar renderização dos Menus e HUDs do Jogo ------------------------------------------------------------
void displayPause(){
	glClear(GL_COLOR_BUFFER_BIT);
	//Render Pause

	renderizarObjetos(0, 0, 600, 600, pauseCoord, pauseMenu);
	SDL_GL_SwapBuffers();
}
void displayCredits(){
	glClear(GL_COLOR_BUFFER_BIT);
	//Render Credits

	renderizarObjetos(0, 0, 600, 600, creditosCoord, creditosMenu);
	SDL_GL_SwapBuffers();
}
void displayGameOver(){
	glClear(GL_COLOR_BUFFER_BIT);
	//Render GameOver

	renderizarObjetos(0, 0, 600, 600, gameOverCoord, gameOver);
	SDL_GL_SwapBuffers();
}
void displayStartScreen(){
	glClear(GL_COLOR_BUFFER_BIT);
	//Render StartScreen
	
	renderizarObjetos(0, 0, 600, 600, startCoord, startMenu);
	SDL_GL_SwapBuffers();
}
void displayControls(){
	glClear(GL_COLOR_BUFFER_BIT);
	//Render ControlScreen
	
	renderizarObjetos(0, 0, 600, 600, controlsCoord, controlsScreen);
	SDL_GL_SwapBuffers();
}

void displayBackground(){
	//Render Background

	renderizarObjetos(0, 0, 600, 600, backgroundCoord, background);
}
void displayPontuacao(){
	//Render Pontuação

	//Dezena de Milhares
	int mi = PPONTOS/10000;
	if(mi == 0){
		contadorPontuacaoCoord[0].MaiX = 1;
		contadorPontuacaoCoord[0].MaiY = 0.9;
		contadorPontuacaoCoord[0].MenX = 0;
		contadorPontuacaoCoord[0].MenY = 1;
	}
	if(mi == 1){
		contadorPontuacaoCoord[0].MaiX = 1;
		contadorPontuacaoCoord[0].MaiY = 0.8;
		contadorPontuacaoCoord[0].MenX = 0;
		contadorPontuacaoCoord[0].MenY = 0.9;	
	}
	if(mi == 2){
		contadorPontuacaoCoord[0].MaiX = 1;
		contadorPontuacaoCoord[0].MaiY = 0.7;
		contadorPontuacaoCoord[0].MenX = 0;
		contadorPontuacaoCoord[0].MenY = 0.8;	
	}
	if(mi == 3){
		contadorPontuacaoCoord[0].MaiX = 1;
		contadorPontuacaoCoord[0].MaiY = 0.6;
		contadorPontuacaoCoord[0].MenX = 0;
		contadorPontuacaoCoord[0].MenY = 0.7;	
	}
	if(mi == 4){
		contadorPontuacaoCoord[0].MaiX = 1;
		contadorPontuacaoCoord[0].MaiY = 0.5;
		contadorPontuacaoCoord[0].MenX = 0;
		contadorPontuacaoCoord[0].MenY = 0.6;	
	}
	if(mi == 5){
		contadorPontuacaoCoord[0].MaiX = 1;
		contadorPontuacaoCoord[0].MaiY = 0.4;
		contadorPontuacaoCoord[0].MenX = 0;
		contadorPontuacaoCoord[0].MenY = 0.5;	
	}
	if(mi == 6){
		contadorPontuacaoCoord[0].MaiX = 1;
		contadorPontuacaoCoord[0].MaiY = 0.3;
		contadorPontuacaoCoord[0].MenX = 0;
		contadorPontuacaoCoord[0].MenY = 0.4;	
	}
	if(mi == 7){
		contadorPontuacaoCoord[0].MaiX = 1;
		contadorPontuacaoCoord[0].MaiY = 0.2;
		contadorPontuacaoCoord[0].MenX = 0;
		contadorPontuacaoCoord[0].MenY = 0.3;	
	}
	if(mi == 8){
		contadorPontuacaoCoord[0].MaiX = 1;
		contadorPontuacaoCoord[0].MaiY = 0.1;
		contadorPontuacaoCoord[0].MenX = 0;
		contadorPontuacaoCoord[0].MenY = 0.2;
	}
	if(mi == 9){
		contadorPontuacaoCoord[0].MaiX = 1;
		contadorPontuacaoCoord[0].MaiY = 0;
		contadorPontuacaoCoord[0].MenX = 0;
		contadorPontuacaoCoord[0].MenY = 0.1;	
	}

	//Milhares
	int m = (PPONTOS%10000)/1000;
	if(m == 0){
		contadorPontuacaoCoord[1].MaiX = 1;
		contadorPontuacaoCoord[1].MaiY = 0.9;
		contadorPontuacaoCoord[1].MenX = 0;
		contadorPontuacaoCoord[1].MenY = 1;
	}
	if(m == 1){
		contadorPontuacaoCoord[1].MaiX = 1;
		contadorPontuacaoCoord[1].MaiY = 0.8;
		contadorPontuacaoCoord[1].MenX = 0;
		contadorPontuacaoCoord[1].MenY = 0.9;	
	}
	if(m == 2){
		contadorPontuacaoCoord[1].MaiX = 1;
		contadorPontuacaoCoord[1].MaiY = 0.7;
		contadorPontuacaoCoord[1].MenX = 0;
		contadorPontuacaoCoord[1].MenY = 0.8;	
	}
	if(m == 3){
		contadorPontuacaoCoord[1].MaiX = 1;
		contadorPontuacaoCoord[1].MaiY = 0.6;
		contadorPontuacaoCoord[1].MenX = 0;
		contadorPontuacaoCoord[1].MenY = 0.7;	
	}
	if(m == 4){
		contadorPontuacaoCoord[1].MaiX = 1;
		contadorPontuacaoCoord[1].MaiY = 0.5;
		contadorPontuacaoCoord[1].MenX = 0;
		contadorPontuacaoCoord[1].MenY = 0.6;	
	}
	if(m == 5){
		contadorPontuacaoCoord[1].MaiX = 1;
		contadorPontuacaoCoord[1].MaiY = 0.4;
		contadorPontuacaoCoord[1].MenX = 0;
		contadorPontuacaoCoord[1].MenY = 0.5;	
	}
	if(m == 6){
		contadorPontuacaoCoord[1].MaiX = 1;
		contadorPontuacaoCoord[1].MaiY = 0.3;
		contadorPontuacaoCoord[1].MenX = 0;
		contadorPontuacaoCoord[1].MenY = 0.4;	
	}
	if(m == 7){
		contadorPontuacaoCoord[1].MaiX = 1;
		contadorPontuacaoCoord[1].MaiY = 0.2;
		contadorPontuacaoCoord[1].MenX = 0;
		contadorPontuacaoCoord[1].MenY = 0.3;	
	}
	if(m == 8){
		contadorPontuacaoCoord[1].MaiX = 1;
		contadorPontuacaoCoord[1].MaiY = 0.1;
		contadorPontuacaoCoord[1].MenX = 0;
		contadorPontuacaoCoord[1].MenY = 0.2;
	}
	if(m == 9){
		contadorPontuacaoCoord[1].MaiX = 1;
		contadorPontuacaoCoord[1].MaiY = 0;
		contadorPontuacaoCoord[1].MenX = 0;
		contadorPontuacaoCoord[1].MenY = 0.1;	
	}
	//Centenas
	int c = ((PPONTOS%10000)%1000)/100;
	if(c == 0){
		contadorPontuacaoCoord[2].MaiX = 1;
		contadorPontuacaoCoord[2].MaiY = 0.9;
		contadorPontuacaoCoord[2].MenX = 0;
		contadorPontuacaoCoord[2].MenY = 1;
	}
	if(c == 1){
		contadorPontuacaoCoord[2].MaiX = 1;
		contadorPontuacaoCoord[2].MaiY = 0.8;
		contadorPontuacaoCoord[2].MenX = 0;
		contadorPontuacaoCoord[2].MenY = 0.9;	
	}
	if(c == 2){
		contadorPontuacaoCoord[2].MaiX = 1;
		contadorPontuacaoCoord[2].MaiY = 0.7;
		contadorPontuacaoCoord[2].MenX = 0;
		contadorPontuacaoCoord[2].MenY = 0.8;	
	}
	if(c == 3){
		contadorPontuacaoCoord[2].MaiX = 1;
		contadorPontuacaoCoord[2].MaiY = 0.6;
		contadorPontuacaoCoord[2].MenX = 0;
		contadorPontuacaoCoord[2].MenY = 0.7;	
	}
	if(c == 4){
		contadorPontuacaoCoord[2].MaiX = 1;
		contadorPontuacaoCoord[2].MaiY = 0.5;
		contadorPontuacaoCoord[2].MenX = 0;
		contadorPontuacaoCoord[2].MenY = 0.6;	
	}
	if(c == 5){
		contadorPontuacaoCoord[2].MaiX = 1;
		contadorPontuacaoCoord[2].MaiY = 0.4;
		contadorPontuacaoCoord[2].MenX = 0;
		contadorPontuacaoCoord[2].MenY = 0.5;	
	}
	if(c == 6){
		contadorPontuacaoCoord[2].MaiX = 1;
		contadorPontuacaoCoord[2].MaiY = 0.3;
		contadorPontuacaoCoord[2].MenX = 0;
		contadorPontuacaoCoord[2].MenY = 0.4;	
	}
	if(c == 7){
		contadorPontuacaoCoord[2].MaiX = 1;
		contadorPontuacaoCoord[2].MaiY = 0.2;
		contadorPontuacaoCoord[2].MenX = 0;
		contadorPontuacaoCoord[2].MenY = 0.3;	
	}
	if(c == 8){
		contadorPontuacaoCoord[2].MaiX = 1;
		contadorPontuacaoCoord[2].MaiY = 0.1;
		contadorPontuacaoCoord[2].MenX = 0;
		contadorPontuacaoCoord[2].MenY = 0.2;
	}
	if(c == 9){
		contadorPontuacaoCoord[2].MaiX = 1;
		contadorPontuacaoCoord[2].MaiY = 0;
		contadorPontuacaoCoord[2].MenX = 0;
		contadorPontuacaoCoord[2].MenY = 0.1;	
	}
	//Dezenas
	int d = (((PPONTOS%10000)%1000)%100)/10;
	if(d == 0){
		contadorPontuacaoCoord[3].MaiX = 1;
		contadorPontuacaoCoord[3].MaiY = 0.9;
		contadorPontuacaoCoord[3].MenX = 0;
		contadorPontuacaoCoord[3].MenY = 1;
	}
	if(d == 1){
		contadorPontuacaoCoord[3].MaiX = 1;
		contadorPontuacaoCoord[3].MaiY = 0.8;
		contadorPontuacaoCoord[3].MenX = 0;
		contadorPontuacaoCoord[3].MenY = 0.9;	
	}
	if(d == 2){
		contadorPontuacaoCoord[3].MaiX = 1;
		contadorPontuacaoCoord[3].MaiY = 0.7;
		contadorPontuacaoCoord[3].MenX = 0;
		contadorPontuacaoCoord[3].MenY = 0.8;	
	}
	if(d == 3){
		contadorPontuacaoCoord[3].MaiX = 1;
		contadorPontuacaoCoord[3].MaiY = 0.6;
		contadorPontuacaoCoord[3].MenX = 0;
		contadorPontuacaoCoord[3].MenY = 0.7;	
	}
	if(d == 4){
		contadorPontuacaoCoord[3].MaiX = 1;
		contadorPontuacaoCoord[3].MaiY = 0.5;
		contadorPontuacaoCoord[3].MenX = 0;
		contadorPontuacaoCoord[3].MenY = 0.6;	
	}
	if(d == 5){
		contadorPontuacaoCoord[3].MaiX = 1;
		contadorPontuacaoCoord[3].MaiY = 0.4;
		contadorPontuacaoCoord[3].MenX = 0;
		contadorPontuacaoCoord[3].MenY = 0.5;	
	}
	if(d == 6){
		contadorPontuacaoCoord[3].MaiX = 1;
		contadorPontuacaoCoord[3].MaiY = 0.3;
		contadorPontuacaoCoord[3].MenX = 0;
		contadorPontuacaoCoord[3].MenY = 0.4;	
	}
	if(d == 7){
		contadorPontuacaoCoord[3].MaiX = 1;
		contadorPontuacaoCoord[3].MaiY = 0.2;
		contadorPontuacaoCoord[3].MenX = 0;
		contadorPontuacaoCoord[3].MenY = 0.3;	
	}
	if(d == 8){
		contadorPontuacaoCoord[3].MaiX = 1;
		contadorPontuacaoCoord[3].MaiY = 0.1;
		contadorPontuacaoCoord[3].MenX = 0;
		contadorPontuacaoCoord[3].MenY = 0.2;
	}
	if(d == 9){
		contadorPontuacaoCoord[3].MaiX = 1;
		contadorPontuacaoCoord[3].MaiY = 0;
		contadorPontuacaoCoord[3].MenX = 0;
		contadorPontuacaoCoord[3].MenY = 0.1;	
	}
	//Unidades
	int u = (((PPONTOS%10000)%1000)%100)%10;
	if(u == 0){
		contadorPontuacaoCoord[4].MaiX = 1;
		contadorPontuacaoCoord[4].MaiY = 0.9;
		contadorPontuacaoCoord[4].MenX = 0;
		contadorPontuacaoCoord[4].MenY = 1;
	}
	if(u == 1){
		contadorPontuacaoCoord[4].MaiX = 1;
		contadorPontuacaoCoord[4].MaiY = 0.8;
		contadorPontuacaoCoord[4].MenX = 0;
		contadorPontuacaoCoord[4].MenY = 0.9;	
	}
	if(u == 2){
		contadorPontuacaoCoord[4].MaiX = 1;
		contadorPontuacaoCoord[4].MaiY = 0.7;
		contadorPontuacaoCoord[4].MenX = 0;
		contadorPontuacaoCoord[4].MenY = 0.8;	
	}
	if(u == 3){
		contadorPontuacaoCoord[4].MaiX = 1;
		contadorPontuacaoCoord[4].MaiY = 0.6;
		contadorPontuacaoCoord[4].MenX = 0;
		contadorPontuacaoCoord[4].MenY = 0.7;	
	}
	if(u == 4){
		contadorPontuacaoCoord[4].MaiX = 1;
		contadorPontuacaoCoord[4].MaiY = 0.5;
		contadorPontuacaoCoord[4].MenX = 0;
		contadorPontuacaoCoord[4].MenY = 0.6;	
	}
	if(u == 5){
		contadorPontuacaoCoord[4].MaiX = 1;
		contadorPontuacaoCoord[4].MaiY = 0.4;
		contadorPontuacaoCoord[4].MenX = 0;
		contadorPontuacaoCoord[4].MenY = 0.5;	
	}
	if(u == 6){
		contadorPontuacaoCoord[4].MaiX = 1;
		contadorPontuacaoCoord[4].MaiY = 0.3;
		contadorPontuacaoCoord[4].MenX = 0;
		contadorPontuacaoCoord[4].MenY = 0.4;	
	}
	if(u == 7){
		contadorPontuacaoCoord[4].MaiX = 1;
		contadorPontuacaoCoord[4].MaiY = 0.2;
		contadorPontuacaoCoord[4].MenX = 0;
		contadorPontuacaoCoord[4].MenY = 0.3;	
	}
	if(u == 8){
		contadorPontuacaoCoord[4].MaiX = 1;
		contadorPontuacaoCoord[4].MaiY = 0.1;
		contadorPontuacaoCoord[4].MenX = 0;
		contadorPontuacaoCoord[4].MenY = 0.2;
	}
	if(u == 9){
		contadorPontuacaoCoord[4].MaiX = 1;
		contadorPontuacaoCoord[4].MaiY = 0;
		contadorPontuacaoCoord[4].MenX = 0;
		contadorPontuacaoCoord[4].MenY = 0.1;	
	}

	renderizarObjetos(0, 3, 20, 20, contadorPontuacaoCoord[0], contadorPontuacao[0]);
	renderizarObjetos(20, 3, 20, 20, contadorPontuacaoCoord[1], contadorPontuacao[1]);
	renderizarObjetos(40, 3, 20, 20, contadorPontuacaoCoord[2], contadorPontuacao[2]);
	renderizarObjetos(60, 3, 20, 20, contadorPontuacaoCoord[3], contadorPontuacao[3]);
	renderizarObjetos(80, 3, 20, 20, contadorPontuacaoCoord[4], contadorPontuacao[4]);
}

//---------------------------------------------------- Execução do Loop que faz o jogo Funcionar ------------------------------------------------------------
void estadoExecucao(){
	//-------------------------- DECLARAÇÃO DE OBJETOS --------------------------//
	/*Configurações Player*/
	Player p = criaPlayer();
	//Teclas
	bool keyEsquerda = false;
	bool keyDireita = false;
	bool keyDisparo = false;
	//Explosão
	Explosion pexp;
	pexp.rend = true;
	pexp.FrameAtual = 1;

	/*Configurações Inimigo*/
	Enemy *e;
	e = malloc (QTDINI * sizeof (Enemy));
	criaInimigos(e);

	/*Configurações Boss*/
	Enemy boss = criaBoss();

	/*Configurações Boss*/
	Explosion explosions[QTDINI];
	criaExplosion(explosions);
	/*Configurações Powerup*/
	Powerup ajudinha;
	ajudinha = criaPowerup();
	/*Configurações Asteroid*/
	Asteroid asteroide;
	asteroide = criaAsteroid();
	//Explosão
	Explosion asExplosion;
	asExplosion.rend = false;
	asExplosion.FrameAtual = 1;
	//Atribui as texturas
	defineTextura();

	/*Configurações Telas*/
	//StartScreen
	int startScreenFrameAtual = 1;
	//ControlsScreen
	int controlsFrameAtual = 1;
	//Background
	int backgroundFrameAtual = 1;
	//GameOver
	int gameOverFrameAtual = 1;
	//Explosion
	int explosionFrameAtual = 1;
	//Pause
	pauseCoord.MaiX = 1;
	pauseCoord.MaiY = 0;
	pauseCoord.MenX = 0;
	pauseCoord.MenY = 1;
	//Creditos
	creditosCoord.MaiX = 1;
	creditosCoord.MaiY = 0;
	creditosCoord.MenX = 0;
	creditosCoord.MenY = 1;

	//-------------------------- ESTADO DE EXECUÇÃO EM LOOPING --------------------------//
	/*Estado de Execução*/
	//Variaveis de Comando
	bool executando = true; //Matem o loop em execução
	bool keyPause = false; //Aciona a Pausa
	bool KeyReset = false; //Aciona o Reset
	int qtdOndas = 1; //Conta a quantidade de ondas de inimigos
	int qtdBossSpawn = 5; //Determina qual onda um novo chefão vai ser spawnado
	int qtdMusBoss = 100; // Conta quando o som do chegão vai ser executado novamente
	bool spawnBoss = false; //Determina se o chefão pode ser spawnado
	bool oJogo = false; //Determina o inicio da partida
	bool creditos = false; //Determina se a tela de Crédito deve ser exibida
	bool repName = true; //Determina se o nome do jogo deve ser dito
	bool repGameOver = true; //Determina se o som de Game Over dever ser executado
	bool controls = true; //Determina se a tela de controles deve ser exibida
	bool notFire = true; //Impede que o jogador comece com um disparo quando pressiona 'SPACE' para iniciar a partida
	SDL_Event eventos; //Captura os eventos do sistema <Pressionar uma tecla ou fechar a janela>

	Mix_PlayChannel(-1, opennigMus, -1);

	while(executando){
		while(SDL_PollEvent(&eventos)){
			/*COMANDOS GERAL*/
			//Encerramento pelo X da Janela
			if(eventos.type == SDL_QUIT){
				executando = false;
			}
			//Encerramento pelo ESC
			if(eventos.type == SDL_KEYUP && eventos.key.keysym.sym == SDLK_ESCAPE){
				if(oJogo == true){
					oJogo = false;
				}else{
					executando = false;
				}	
			}
			//Pausar o Jogo
			if(eventos.type == SDL_KEYUP && eventos.key.keysym.sym == SDLK_p){
				if(keyPause == false){
					keyPause = true;
				}else{
					keyPause = false;
				}
			}	
			//Resetar Jogo
			if(eventos.type == SDL_KEYDOWN){
				if(eventos.key.keysym.sym == SDLK_r){
					KeyReset = true;
				}
			}
			//Movimento no Menu Inicial
			if(eventos.type == SDL_KEYUP && eventos.key.keysym.sym == SDLK_UP){
				if(startScreenFrameAtual == 1){
					Mix_PlayChannel(-1, navigatingMus, 0);
					startScreenFrameAtual = 3;
				}else if(startScreenFrameAtual == 2){
					Mix_PlayChannel(-1, navigatingMus, 0);
					startScreenFrameAtual = 1;
				}else if(startScreenFrameAtual == 3){
					Mix_PlayChannel(-1, navigatingMus, 0);
					startScreenFrameAtual = 2;
				}
			}
			if(eventos.type == SDL_KEYUP && eventos.key.keysym.sym == SDLK_DOWN){
				if(startScreenFrameAtual == 1){
					Mix_PlayChannel(-1, navigatingMus, 0);
					startScreenFrameAtual = 2;
				}else if(startScreenFrameAtual == 2){
					Mix_PlayChannel(-1, navigatingMus, 0);
					startScreenFrameAtual = 3;
				}else if(startScreenFrameAtual == 3){
					Mix_PlayChannel(-1, navigatingMus, 0);
					startScreenFrameAtual = 1;
				}
			}	
			if(eventos.type == SDL_KEYUP && eventos.key.keysym.sym == SDLK_SPACE){
				if(startScreenFrameAtual == 1 && oJogo == false){
					oJogo = true;
					KeyReset = true;
					p = criaPlayer();
					repName = true;
					repGameOver = true;
					controls = true;
					notFire = true;
					keyDisparo = false;
					Mix_PlayChannel(-1, gameStartMus, 0);

				}
				if(startScreenFrameAtual == 2 && creditos == false){
					creditos = true;
				}

				if(startScreenFrameAtual == 3){
					executando = false;
				}
			}

			//Movimeto do Player
			if(eventos.type == SDL_KEYDOWN){ //Tecla Pressionada
				/*Esquerda*/
				if(eventos.key.keysym.sym == SDLK_LEFT){
					keyEsquerda = true;
				}
				/*Direita*/
				if(eventos.key.keysym.sym == SDLK_RIGHT){
					keyDireita= true;
				}
			}
			if(eventos.type == SDL_KEYUP){ //Tecla Solta
				/*Esquerda*/
				if(eventos.key.keysym.sym == SDLK_LEFT){
					keyEsquerda = false;
				}
				/*Direita*/
				if(eventos.key.keysym.sym == SDLK_RIGHT){
					keyDireita= false;
				}
			}
			//Disparo de projetil
			if(eventos.type == SDL_KEYDOWN){
				if(eventos.key.keysym.sym == SDLK_SPACE){
					if(notFire == true){ //Impede que o jogador já comece com um disparo
						notFire = false;
					}else{
						keyDisparo = true;
					}
					
				}
			}
		}
		if(creditos == true){
			//Exibe os Créditos
			displayCredits();
			SDL_Delay(5000);
			creditos = false;
		}else{
			if(oJogo == true){
				if(p.Vidas > 0){
					//Mostra os Controles
					if(controls == true){
						controls = false;
						for(int i = 0; i < 50; i++){
							if(controlsFrameAtual == 1){
								controlsCoord.MaiX = 1;
								controlsCoord.MaiY = 0.5;
								controlsCoord.MenX = 0;
								controlsCoord.MenY = 1;	
								controlsFrameAtual = 2;
							}else if(controlsFrameAtual == 2){
								controlsCoord.MaiX = 1;
								controlsCoord.MaiY = 0;
								controlsCoord.MenX = 0;
								controlsCoord.MenY = 0.5;	
								controlsFrameAtual = 1;
							}
							displayControls();
							SDL_Delay(50);
						}		
					}
					//Pausar o Jogo
					if(keyPause == true){
						displayPause();
					}else{
						//Resetar o Jogo
						if(KeyReset == true){
							p = criaPlayer();
							PPONTOS = 0;
							VELINI = 2;
							criaInimigos(e);
							qtdOndas = 1;
							ajudinha.rend = false;
							boss.rend = false;
							qtdBossSpawn = 5;
							KeyReset = false;
						}

						/*Verificações do Jogo*/
						//Verifica se o Jogo deve gerar uma nova Onda de Inimigos ou um novo Boss
						if(verificaInimigos(e) == true){
							if(qtdOndas < qtdBossSpawn && boss.rend == false){
								VELINI = VELINI + 0.2;
								criaInimigos(e);
								criaBullet(p.bul, p.texturaBulletAtual);
								criaExplosion(explosions);
								p.Disp = 0;
								qtdOndas++;
							}else{
								if(qtdOndas == qtdBossSpawn && boss.rend == false){
									qtdBossSpawn = qtdBossSpawn + 5;
									boss = criaBoss();
									criaBullet(p.bul, p.texturaBulletAtual);
									criaExplosion(explosions);
									p.Disp = 0;
									boss.rend = true;
									qtdOndas++;
								}
							}
						}
						//Verifica se deve gerar um Powerup
						if(verificaSorteio() == true && ajudinha.rend == false){
							ajudinha = criaPowerup();
							ajudinha.rend = true;
						}

						//Verifica se deve gerar um Asteroide
						if(qtdOndas > 50){
							if(verificaSorteio() == true && asteroide.rend == false){
								asteroide = criaAsteroid();
								asteroide.rend = true;
							}
						}

						/*Movimento dos Objetos*/
						//Movimento do Powerup
						if(ajudinha.rend == true){
							ajudinha.Y = ajudinha.Y + 2;
							if(colisao(ajudinha.X, ajudinha.Y, ajudinha.Comp, ajudinha.Alt, p.X, p.Y, p.Comp, p.Alt) == true){
								ajudinha.Y = -9000;
								ajudinha.rend = false;

								if(ajudinha.pup == 0 && p.Vidas < 3){
									p.Vidas = p.Vidas + 1;
								}

								if(ajudinha.pup == 1){
									p.texturaBulletAtual = playerBullet[0];

									for(int i = p.Disp; i < QTDMAXDISP; i++){
										p.bul[i].textura = p.texturaBulletAtual;
										p.bul[i].Dano = 1;
									}
								}

								if(ajudinha.pup == 2){
									p.texturaBulletAtual = playerBullet[1];

									for(int i = p.Disp; i < QTDMAXDISP; i++){
										p.bul[i].textura = p.texturaBulletAtual;
										p.bul[i].Dano = 2;
									}
								}

								if(ajudinha.pup == 3){
									p.texturaBulletAtual = playerBullet[2];

									for(int i = p.Disp; i < QTDMAXDISP; i++){
										p.bul[i].textura = p.texturaBulletAtual;
										p.bul[i].Dano = 3;
									}
								}
							}

							if(ajudinha.Y > 600){
								ajudinha.rend = false;
								ajudinha.Y = -9000;
							}
						}

						//Movimento do Asteroide
						if(asteroide.rend == true){
							asteroide.Y = asteroide.Y + 5;
							if(colisao(asteroide.X, asteroide.Y, asteroide.Comp, asteroide.Alt, p.X, p.Y, p.Comp, p.Alt) == true){
								asteroide.Y = -9000;
								asteroide.rend = false;
								p.Vidas = 0;
							}

							if(asteroide.Y > 600){
								asteroide.rend = false;
								asteroide.Y = -9000;
							}
						}


						//Movimento do Player
						if(keyEsquerda == true){
							if(p.X >= 0){
								p.X = p.X - 8;
							}
						}
						if(keyDireita == true){
							if(p.X <= 550){
								p.X = p.X + 8;
							}
						}
						//Movimento do Tiro Player
						if(keyDisparo == true){
							if(p.Disp < QTDMAXDISP){
								p.bul[p.Disp].X = p.X + (p.Comp / 2);
								p.bul[p.Disp].Y = p.Y;
								p.bul[p.Disp].VelY = 5;
								p.bul[p.Disp].rend = true;
								keyDisparo = false;
								p.Disp++;
							}

							Mix_PlayChannel(-1, firingMus, 0);
						}
						for(int i = 0; i< p.Disp;i++){
							for(int j = 0; j < QTDINI; j++){
								if(p.bul[i].rend == true){
									if(colisao(p.bul[i].X, p.bul[i].Y-e[j].Alt, p.bul[i].Comp, p.bul[i].Alt, e[j].X, e[j].Y, e[j].Comp, e[j].Alt == true)){
										e[j].Vidas = e[j].Vidas - p.bul[i].Dano;
										p.bul[i].rend = false;
										if(e[j].Vidas <= 0){
											explosions[j].X = e[j].X;
											explosions[j].Y = e[j].Y;
											explosions[j].Comp = e[j].Comp;
											explosions[j].Alt = e[j].Alt;
											explosions[j].rend = true;
											Mix_PlayChannel(-1, explosionMus, 0);

											e[j].Y = -9000;
											e[j].rend = false;
										}
										PPONTOS++;	
									}
									if(p.bul[i].Y < 40){
										p.bul[i].Y = -9000;
										p.bul[i].rend == false;
									}
									if(boss.rend == true){
										if(colisao(p.bul[i].X, p.bul[i].Y-e[j].Alt, p.bul[i].Comp, p.bul[i].Alt, boss.X, boss.Y, boss.Comp, boss.Alt == true)){
											boss.Vidas = boss.Vidas - p.bul[i].Dano;
											p.bul[i].rend = false;
											if(boss.Vidas <= 0){
												explosions[0].X = boss.X;
												explosions[0].Y = boss.Y;
												explosions[0].Comp = boss.Comp;
												explosions[0].Alt = boss.Alt;
												explosions[0].rend = true;
												Mix_PlayChannel(-1, explosionMus, 0);

												boss.Y = -9000;
												boss.rend = false;
											}
											PPONTOS++;
										}
										if(p.bul[i].Y < 40){
											p.bul[i].Y = -9000;
											p.bul[i].rend == false;
										}
									}

									if(asteroide.rend == true){
										if(colisao(p.bul[i].X, p.bul[i].Y-e[j].Alt, p.bul[i].Comp, p.bul[i].Alt, asteroide.X, asteroide.Y, asteroide.Comp, asteroide.Alt == true)){
											asteroide.Vidas = asteroide.Vidas - p.bul[i].Dano;
											p.bul[i].rend = false;
											if(asteroide.Vidas <= 0){
												asExplosion.X = asteroide.X;
												asExplosion.Y = asteroide.Y;
												asExplosion.Comp = asteroide.Comp;
												asExplosion.Alt = asteroide.Alt;
												asExplosion.rend = true;
												Mix_PlayChannel(-1, explosionMus, 0);

												asteroide.Y = -9000;
												asteroide.rend = false;
											}
											PPONTOS++;
										}
										if(p.bul[i].Y < 40){
											p.bul[i].Y = -9000;
											p.bul[i].rend == false;
										}
									}
								}
							}
						}

						//Movimento do Boss
						if(boss.rend == true){
							if(boss.X <= 0){
								boss.VelX= - boss.VelX;
							}
							if(boss.X >= 550){
								boss.VelX = - boss.VelX;
							}
							boss.X = boss.X + boss.VelX;
							if(boss.Y < -900){
								boss.rend = false;
							}

							//Movimento do Tiro do Boss
							if(boss.bulTime == boss.auxbulTime){
								boss.bul[boss.Disp].X = boss.X + (boss.Comp / 2);
								boss.bul[boss.Disp].Y = boss.Y + boss.Alt;
								boss.bul[boss.Disp].VelY = 5;
								boss.bul[boss.Disp].rend = true;
								boss.Disp++;
								boss.bulTime = 0;
							}else{
								boss.bulTime++;
							}
							for(int j = 0; j < boss.Disp;j++){
								if(boss.bul[j].rend == true){
									if(colisao(boss.bul[j].X, boss.bul[j].Y, boss.bul[j].Comp, boss.bul[j].Alt, p.X, p.Y, p.Comp, p.Alt == true)){
										p.Vidas = p.Vidas - boss.bul[j].Dano;
										boss.bul[j].Y = -9000;
										boss.bul[j].rend = false;
									}
								}
							}
						}

						for(int i = 0; i < QTDINI;i++){
							//Movimento do Inimigo
							if(e[i].rend == true){
								if(e[i].X <= 0){
									e[i].VelX= - e[i].VelX;
									e[i].Y = e[i].Y + e[i].Alt + 3;
								}
								if(e[i].X >= 550){
									e[i].VelX = - e[i].VelX;
									e[i].Y = e[i].Y + e[i].Alt + 3;
								}
								e[i].X = e[i].X + e[i].VelX;
								if(e[i].Y < -900){
									e[i].rend = false;
								}
							}

							if(colisao(e[i].X, e[i].Y, e[i].Comp, e[i].Alt, p.X, p.Y, p.Comp, p.Alt) == true){
								e[i].Y = -9000;
								e[i].rend = false;
								p.Vidas = 0;
							}
						}
						//Movimento do Tiro do Inimigo
						for(int i = 0; i < QTDINI; i++){
							if(e[i].bulTime == e[i].auxbulTime && e[i].rend == true){
								if(e[i].Y > -2){
									e[i].bul[e[i].Disp].X = e[i].X + (e[i].Comp / 2);
									e[i].bul[e[i].Disp].Y = e[i].Y+e[i].Alt;
									e[i].bul[e[i].Disp].VelY = 5;
									e[i].bul[e[i].Disp].rend = true;
									e[i].Disp++;
								}
								e[i].bulTime = 0;
							}else{
								e[i].bulTime++;
							}
						}
						for(int i = 0; i < QTDINI; i++){
							for(int j = 0; j < e[i].Disp;j++){
								if(e[i].bul[j].rend == true){
									if(colisao(e[i].bul[j].X, e[i].bul[j].Y, e[i].bul[j].Comp, e[i].bul[j].Alt, p.X, p.Y, p.Comp, p.Alt == true)){
										p.Vidas = p.Vidas - e[i].bul[j].Dano;
										e[i].bul[j].Y = -9000;
										e[i].bul[j].rend = false;
									}
								}
							}
						}
							
						/*Renderização*/
						glClear(GL_COLOR_BUFFER_BIT); //Limpa o Buffer

						//Calcula Frame Background
						if(backgroundFrameAtual == 1){
							backgroundCoord.MaiX = 1;
							backgroundCoord.MaiY = 0.6666;
							backgroundCoord.MenX = 0;
							backgroundCoord.MenY = 1;
							backgroundFrameAtual = 2;
						}else if(backgroundFrameAtual == 2){
							backgroundCoord.MaiX = 1;
							backgroundCoord.MaiY = 0.3333;
							backgroundCoord.MenX = 0;
							backgroundCoord.MenY = 0.6666;
							backgroundFrameAtual = 3;
						}else if(backgroundFrameAtual == 3){
							backgroundCoord.MaiX = 1;
							backgroundCoord.MaiY = 0;
							backgroundCoord.MenX = 0;
							backgroundCoord.MenY = 0.3333;
							backgroundFrameAtual = 1;
						}
						//Render Background
						displayBackground();

						//Calcula Frame Player
						if(p.FrameAtual == 1){
							p.tamFrame.MaiX = 1;
							p.tamFrame.MaiY = 0;
							p.tamFrame.MenX = 0;
							p.tamFrame.MenY = 0.5;
							p.FrameAtual = 2;
						}else if(p.FrameAtual == 2){
							p.tamFrame.MaiX = 1;
							p.tamFrame.MaiY = 0.5;
							p.tamFrame.MenX = 0;
							p.tamFrame.MenY = 1;
							p.FrameAtual = 1;
						}
						//Render Player
						renderizarObjetos(p.X, p.Y, p.Comp, p.Alt, p.tamFrame, player);
						for(int i = 0; i < p.Disp;i++){
							if(p.bul[i].rend == true){
								p.bul[i].Y = p.bul[i].Y - p.bul[i].VelY;

								//Calcula Frame Bullet Player
								if(p.bul[i].FrameAtual == 1){
										p.bul[i].tamFrame.MaiX = 1;
										p.bul[i].tamFrame.MaiY = 0;
										p.bul[i].tamFrame.MenX = 0;
										p.bul[i].tamFrame.MenY = 0.5;
										p.bul[i].FrameAtual = 2;
									}else if(p.bul[i].FrameAtual == 2){
										p.bul[i].tamFrame.MaiX = 1;
										p.bul[i].tamFrame.MaiY = 0.5;
										p.bul[i].tamFrame.MenX = 0;
										p.bul[i].tamFrame.MenY = 1;
										p.bul[i].FrameAtual = 1;
									}
								renderizarObjetos(p.bul[i].X, p.bul[i].Y, p.bul[i].Comp, p.bul[i].Alt, p.bul[i].tamFrame, p.bul[i].textura);
							}
						}
						if(boss.rend == false){
							//Render Inimigos
							for(int i = 0; i < QTDINI;i++){
								if(e[i].rend == true){

									//Calcula Frame Enemy							
									if(e[i].FrameAtual == 1){
										e[i].tamFrame.MaiX = 1;
										e[i].tamFrame.MaiY = 0.8;
										e[i].tamFrame.MenX = 0;
										e[i].tamFrame.MenY = 1;
										e[i].FrameAtual = 2;
									}else if(e[i].FrameAtual == 2){
										e[i].tamFrame.MaiX = 1;
										e[i].tamFrame.MaiY = 0.6;
										e[i].tamFrame.MenX = 0;
										e[i].tamFrame.MenY = 0.8;
										e[i].FrameAtual = 3;
									}else if(e[i].FrameAtual == 3){
										e[i].tamFrame.MaiX = 1;
										e[i].tamFrame.MaiY = 0.4;
										e[i].tamFrame.MenX = 0;
										e[i].tamFrame.MenY = 0.6;
										e[i].FrameAtual = 4;
									}else if(e[i].FrameAtual == 4){
										e[i].tamFrame.MaiX = 1;
										e[i].tamFrame.MaiY = 0.2;
										e[i].tamFrame.MenX = 0;
										e[i].tamFrame.MenY = 0.4;
										e[i].FrameAtual = 5;
									}else if(e[i].FrameAtual == 5){
										e[i].tamFrame.MaiX = 1;
										e[i].tamFrame.MaiY = 0.2;
										e[i].tamFrame.MenX = 0;
										e[i].tamFrame.MenY = 0;
										e[i].FrameAtual = 1;
									}
									renderizarObjetos(e[i].X, e[i].Y, e[i].Comp, e[i].Alt, e[i].tamFrame, e[i].textura);	
								}
								for(int j = 0; j < e[i].Disp;j++){
									if(e[i].bul[j].rend == true){
										e[i].bul[j].Y = e[i].bul[j].Y + e[i].bul[j].VelY;

										//Calcula Frame Bullet Enemy
										if(e[i].bul[j].FrameAtual == 1){
											e[i].bul[j].tamFrame.MaiX = 1;
											e[i].bul[j].tamFrame.MaiY = 0;
											e[i].bul[j].tamFrame.MenX = 0;
											e[i].bul[j].tamFrame.MenY = 0.5;
											e[i].bul[j].FrameAtual = 2;
										}else if(e[i].bul[j].FrameAtual == 2){
											e[i].bul[j].tamFrame.MaiX = 1;
											e[i].bul[j].tamFrame.MaiY = 0.5;
											e[i].bul[j].tamFrame.MenX = 0;
											e[i].bul[j].tamFrame.MenY = 1;
											e[i].bul[j].FrameAtual = 1;
										}

										renderizarObjetos(e[i].bul[j].X, e[i].bul[j].Y, e[i].bul[j].Comp, e[i].bul[j].Alt, e[i].bul[j].tamFrame, enemyBullet);
									}
								}
							}
						}else{
							//Calcula Frame Boss
							if(boss.FrameAtual == 1){
								boss.tamFrame.MaiX = 1;
								boss.tamFrame.MaiY = 0;
								boss.tamFrame.MenX = 0;
								boss.tamFrame.MenY = 0.5;
								boss.FrameAtual = 2;
							}else if(boss.FrameAtual == 2){
								boss.tamFrame.MaiX = 1;
								boss.tamFrame.MaiY = 0.5;
								boss.tamFrame.MenX = 0;
								boss.tamFrame.MenY = 1;
								boss.FrameAtual = 1;
							}

							//Render Boss
							renderizarObjetos(boss.X, boss.Y, boss.Comp, boss.Alt, boss.tamFrame, boss.textura);
							if(qtdMusBoss == 100){
								Mix_PlayChannel(-1, bossMus, 0);
								qtdMusBoss = 0;
							}else{
								qtdMusBoss++;
							}
							for(int j = 0; j < boss.Disp;j++){
								if(boss.bul[j].rend == true){
									boss.bul[j].Y = boss.bul[j].Y + boss.bul[j].VelY;

									//Calcula Frame Bullet Boss
									if(boss.bul[j].FrameAtual == 1){
										boss.bul[j].tamFrame.MaiX = 1;
										boss.bul[j].tamFrame.MaiY = 0;
										boss.bul[j].tamFrame.MenX = 0;
										boss.bul[j].tamFrame.MenY = 0.5;
										boss.bul[j].FrameAtual = 2;
									}else if(boss.bul[j].FrameAtual == 2){
										boss.bul[j].tamFrame.MaiX = 1;
										boss.bul[j].tamFrame.MaiY = 0.5;
										boss.bul[j].tamFrame.MenX = 0;
										boss.bul[j].tamFrame.MenY = 1;
										boss.bul[j].FrameAtual = 1;
									}
									renderizarObjetos(boss.bul[j].X, boss.bul[j].Y, boss.bul[j].Comp, boss.bul[j].Alt, boss.bul[j].tamFrame, enemyBullet);
								}
							}
						}

						//Render Explosão
						for(int i = 0; i < QTDINI;i++){
							if(explosions[i].rend == true){
								//Calcula Frame Explosão							
								if(explosions[i].FrameAtual == 1){
									explosions[i].tamFrame.MaiX = 1;
									explosions[i].tamFrame.MaiY = 0.8;
									explosions[i].tamFrame.MenX = 0;
									explosions[i].tamFrame.MenY = 1;
									explosions[i].FrameAtual = 2;
								}else if(explosions[i].FrameAtual == 2){
									explosions[i].tamFrame.MaiX = 1;
									explosions[i].tamFrame.MaiY = 0.6;
									explosions[i].tamFrame.MenX = 0;
									explosions[i].tamFrame.MenY = 0.8;
									explosions[i].FrameAtual = 3;
								}else if(explosions[i].FrameAtual == 3){
									explosions[i].tamFrame.MaiX = 1;
									explosions[i].tamFrame.MaiY = 0.4;
									explosions[i].tamFrame.MenX = 0;
									explosions[i].tamFrame.MenY = 0.6;
									explosions[i].FrameAtual = 4;
								}else if(explosions[i].FrameAtual == 4){
									explosions[i].tamFrame.MaiX = 1;
									explosions[i].tamFrame.MaiY = 0.2;
									explosions[i].tamFrame.MenX = 0;
									explosions[i].tamFrame.MenY = 0.4;
									explosions[i].FrameAtual = 5;
								}else if(explosions[i].FrameAtual == 5){
									explosions[i].tamFrame.MaiX = 1;
									explosions[i].tamFrame.MaiY = 0.2;
									explosions[i].tamFrame.MenX = 0;
									explosions[i].tamFrame.MenY = 0;
									explosions[i].FrameAtual = 1;
									explosions[i].rend = false;
								}
								renderizarObjetos(explosions[i].X, explosions[i].Y, explosions[i].Comp, explosions[i].Alt, explosions[i].tamFrame, explosion);
							}
						}
						//Render PlayerGunbar
						if(p.texturaBulletAtual == playerBullet[0]){
							renderizarObjetos(0, 70, 32, 32, ajudinha.tamFrame, playerPowerup[1]);
						}
						if(p.texturaBulletAtual == playerBullet[1]){
							renderizarObjetos(0, 70, 32, 32, ajudinha.tamFrame, playerPowerup[2]);
						}
						if(p.texturaBulletAtual == playerBullet[2]){
							renderizarObjetos(0, 70, 32, 32, ajudinha.tamFrame, playerPowerup[3]);
						}
						
						//Render Playerlifebar
						if(p.Vidas == 3){
							
							playerLifeCoord.MaiX = 1;
							playerLifeCoord.MaiY = 0.75;
							playerLifeCoord.MenX = 0;
							playerLifeCoord.MenY = 1;
						}
						if(p.Vidas == 2){
							playerLifeCoord.MaiX = 1;
							playerLifeCoord.MaiY = 0.5;
							playerLifeCoord.MenX = 0;
							playerLifeCoord.MenY = 0.75;
						}
						if(p.Vidas == 1){
							playerLifeCoord.MaiX = 1;
							playerLifeCoord.MaiY = 0.25;
							playerLifeCoord.MenX = 0;
							playerLifeCoord.MenY = 0.5;
						}
						if(p.Vidas == 0){
							playerLifeCoord.MaiX = 1;
							playerLifeCoord.MaiY = 0;
							playerLifeCoord.MenX = 0;
							playerLifeCoord.MenY = 0.25;
						}
						renderizarObjetos(0, -2, 128, 128, playerLifeCoord, playerLife);

						//Render Powerup
						if(ajudinha.rend == true){
							renderizarObjetos(ajudinha.X, ajudinha.Y, ajudinha.Comp, ajudinha.Alt, ajudinha.tamFrame, ajudinha.textura);
						}

						//Render Pontuação
						displayPontuacao();

						//Render Asteroid
						if(asteroide.rend == true){
							if(asteroide.FrameAtual == 1){
								asteroide.tamFrame.MaiX = 1;
								asteroide.tamFrame.MaiY = 0;
								asteroide.tamFrame.MenX = 0;
								asteroide.tamFrame.MenY = 0.5;
								asteroide.FrameAtual = 2;
							}else if(asteroide.FrameAtual == 2){
								asteroide.tamFrame.MaiX = 1;
								asteroide.tamFrame.MaiY = 0.5;
								asteroide.tamFrame.MenX = 0;
								asteroide.tamFrame.MenY = 1;
								asteroide.FrameAtual = 1;
							}

							renderizarObjetos(asteroide.X, asteroide.Y, asteroide.Comp, asteroide.Alt, asteroide.tamFrame, scene_asteroid);
						}else if(asExplosion.rend == true){
							if(asExplosion.FrameAtual == 1){
									asExplosion.tamFrame.MaiX = 1;
									asExplosion.tamFrame.MaiY = 0.8;
									asExplosion.tamFrame.MenX = 0;
									asExplosion.tamFrame.MenY = 1;
									asExplosion.FrameAtual = 2;
								}else if(asExplosion.FrameAtual == 2){
									asExplosion.tamFrame.MaiX = 1;
									asExplosion.tamFrame.MaiY = 0.6;
									asExplosion.tamFrame.MenX = 0;
									asExplosion.tamFrame.MenY = 0.8;
									asExplosion.FrameAtual = 3;
								}else if(asExplosion.FrameAtual == 3){
									asExplosion.tamFrame.MaiX = 1;
									asExplosion.tamFrame.MaiY = 0.4;
									asExplosion.tamFrame.MenX = 0;
									asExplosion.tamFrame.MenY = 0.6;
									asExplosion.FrameAtual = 4;
								}else if(asExplosion.FrameAtual == 4){
									asExplosion.tamFrame.MaiX = 1;
									asExplosion.tamFrame.MaiY = 0.2;
									asExplosion.tamFrame.MenX = 0;
									asExplosion.tamFrame.MenY = 0.4;
									asExplosion.FrameAtual = 5;
								}else if(asExplosion.FrameAtual == 5){
									asExplosion.tamFrame.MaiX = 1;
									asExplosion.tamFrame.MaiY = 0.2;
									asExplosion.tamFrame.MenX = 0;
									asExplosion.tamFrame.MenY = 0;
									asExplosion.FrameAtual = 1;
									asExplosion.rend = false;
								}
							renderizarObjetos(asExplosion.X, asExplosion.Y, asExplosion.Comp, asExplosion.Alt, asExplosion.tamFrame,explosion);
						}
						
						SDL_GL_SwapBuffers(); //Animação
						SDL_Delay(30); //Acrescenta um delay até a proxima renderização (Deixe isso quieto se quiser passar pelo menos da primeira fase)

						if(p.Vidas <= 0){ //Verifica se já pode explodir a nave do jogador /Ele precisa ter perdido todas as vidas/
							pexp.rend = true;
						}
					}
				}else{
					if(pexp.rend == true){ //Renderiza a explosão do jogador
						pexp.X = p.X;
						pexp.Y = p.Y;
						pexp.Comp = 64;
						pexp.Alt = 64;

						Mix_PlayChannel(-1, explosionMus, 0);
						
						//Calcula Frame Explosão							
						if(pexp.FrameAtual == 1){
							pexp.tamFrame.MaiX = 1;
							pexp.tamFrame.MaiY = 0.8;
							pexp.tamFrame.MenX = 0;
							pexp.tamFrame.MenY = 1;
							pexp.FrameAtual = 2;
							renderizarObjetos(pexp.X, pexp.Y, pexp.Comp, pexp.Alt, pexp.tamFrame, explosion);
							SDL_GL_SwapBuffers();
							SDL_Delay(20);
						}
						if(pexp.FrameAtual == 2){
							pexp.tamFrame.MaiX = 1;
							pexp.tamFrame.MaiY = 0.6;
							pexp.tamFrame.MenX = 0;
							pexp.tamFrame.MenY = 0.8;
							pexp.FrameAtual = 3;
							renderizarObjetos(pexp.X, pexp.Y, pexp.Comp, pexp.Alt, pexp.tamFrame, explosion);
							SDL_GL_SwapBuffers();
							SDL_Delay(20);
						}
						if(pexp.FrameAtual == 3){
							pexp.tamFrame.MaiX = 1;
							pexp.tamFrame.MaiY = 0.4;
							pexp.tamFrame.MenX = 0;
							pexp.tamFrame.MenY = 0.6;
							pexp.FrameAtual = 4;
							renderizarObjetos(pexp.X, pexp.Y, pexp.Comp, pexp.Alt, pexp.tamFrame, explosion);
							SDL_GL_SwapBuffers();
							SDL_Delay(20);
						}
						if(pexp.FrameAtual == 4){
							pexp.tamFrame.MaiX = 1;
							pexp.tamFrame.MaiY = 0.2;
							pexp.tamFrame.MenX = 0;
							pexp.tamFrame.MenY = 0.4;
							pexp.FrameAtual = 5;
							renderizarObjetos(pexp.X, pexp.Y, pexp.Comp, pexp.Alt, pexp.tamFrame, explosion);
							SDL_GL_SwapBuffers();
							SDL_Delay(20);
						}
						if(pexp.FrameAtual == 5){
							pexp.tamFrame.MaiX = 1;
							pexp.tamFrame.MaiY = 0.2;
							pexp.tamFrame.MenX = 0;
							pexp.tamFrame.MenY = 0;
							pexp.FrameAtual = 1;
							renderizarObjetos(pexp.X, pexp.Y, pexp.Comp, pexp.Alt, pexp.tamFrame, explosion);
							SDL_GL_SwapBuffers();
							SDL_Delay(20);
							pexp.rend = false;
						}
					}

					//Exibe tela de Game Over
					if(repGameOver == true){ //Impede que o jogo te faça chorar ainda mais (Evita que o som de Game Over seja repetido)
						Mix_PlayChannel(-1, gameOverMus, 0);
						repGameOver = false;
					}

					if(gameOverFrameAtual == 1){
						gameOverCoord.MaiX = 1;
						gameOverCoord.MaiY = 0.6666;
						gameOverCoord.MenX = 0;
						gameOverCoord.MenY = 1;
						gameOverFrameAtual = 2;
					}else if(gameOverFrameAtual == 2){
						gameOverCoord.MaiX = 1;
						gameOverCoord.MaiY = 0.3333;
						gameOverCoord.MenX = 0;
						gameOverCoord.MenY = 0.6666;
						gameOverFrameAtual++;
					}else if(gameOverFrameAtual == 3){
						gameOverCoord.MaiX = 1;
						gameOverCoord.MaiY = 0;
						gameOverCoord.MenX = 0;
						gameOverCoord.MenY = 0.3333;
						gameOverFrameAtual = 1;
					}
						displayGameOver();
						SDL_Delay(45);
				}
			}else{
				//Exibe tela do Menu Inicial
				if(repName == true){ //Earth Defenders - Last Edition (Impede que o som seja repetido)
					Mix_PlayChannel(-1, gameNameMus, 0);
					repName = false;
				}
				if(startScreenFrameAtual == 1 && creditos == false){
					startCoord.MaiX = 1;
					startCoord.MaiY = 0.6666;
					startCoord.MenX = 0;
					startCoord.MenY = 1;
				}
				if(startScreenFrameAtual == 2){
					startCoord.MaiX = 1;
					startCoord.MaiY = 0.3333;
					startCoord.MenX = 0;
					startCoord.MenY = 0.6666;
				}
				if(startScreenFrameAtual == 3 && creditos == false){
					startCoord.MaiX = 1;
					startCoord.MaiY = 0;
					startCoord.MenX = 0;
					startCoord.MenY = 0.3333;
				}
				displayStartScreen();
			}
		}
	}
}

int main(int argc, char* args[]){ //Nossa querida e tão aguardada (Pela Linguagem C), Main
	srand(time(NULL)); //Usa a hora do sistema para gerar numeros aleatórios unicos
	SDL_Init(SDL_INIT_EVERYTHING); //Inicia o SDL

	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096); //Define os parâmetros do canal de audio (Frequência, Formato, Canal - Stereo, Buffer)

	//Chama nossas funções principais
	controleMemoria();
	confJanela();

	estadoExecucao();
	
	Mix_CloseAudio(); //Encerra o canal de audio

	printf(" Say Good Bye, My Little Friend \n"); //Não é exatamente igual, mas dá para entender a referência *Capitão is Dead*
	SDL_Quit();	//Encerra o SDL

	return 0;
}