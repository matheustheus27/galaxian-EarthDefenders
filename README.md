Projeto de TP1 para Computação Gráfica - Galaxian

	Docente: Glender Brás
	Discentes: Filipe Bicalho, Maria Eduarda, Matheus Thiago e Thiago Freitas


Bibliotecas Utilizadas:

	SDL, SDL_image, SDL_mixer: 
		sudo apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev 
	SOIL: 
		sudo apt-get install libsoil-dev


linhas de compilação e Execução:

	---- Manual ----
		gcc galaxian.c -o galaxian -lGL -lSOIL -lSDL -lSDLmain -lSDL_image -lSDL_mixer -lm
		./galaxian

	---- MakeFile ----
		make install-libraries  		- Instalar as bibliotecas utilizadas
		make all				- Compilar o projeto
		make run				- Executar o projeto já compilado
		make clean				- Remove o arquivo compilado


Recursos Implementados:

	Pausar - Pressione "P" para pausar o jogo
	Reiniciar - Pressione "R" para reiniciar o jogo
	Voltar/Sair - Pressione "ESC" para voltar/sair do Jogo
	Confirmar/Atirar - Pressione "SPACE" para confirmar no menu ou atirar durante o jogo
	Movimento Personagem - Pressione as setas "LEFT" ou "RIGHT" para movimentar o personagem
	Movimento no Menu - Pressione as setas "UP" ou "DOWN" para navegar entre as opções do menu
	Movimento dos Inimigo - Os inimigos se movimentam no Eixo X e Eixo Y
	Disparo Randomico dos Inimigos - Cada Inimigo tem o seu proprio tempo de disparo
	Jogo Infinito - O jogo é continuo e a velocidade dos inimigos aumenta a cada nova onda, até que ele não seja mais capaz de continuar o jogo
	Powerups - O jogo concede ao jogador um novo recurso, sendo mais uma vida ou um novo tipo de tiro
	Pontuação - O jogo conta 1 ponto a cada vez que o usuario acerta um inimigo
	Asteroid - O asteroide causa instakill no jogador, ele é renderizado depois que o jogador atinge a onda 50 de inimigos

	StartScreen - Foi adicionado Menu Inicial
	PauseScreen - Foi adicionado Menu de Pausa
	ControlsScreen - Foi adicionado uma tela exibindo os controles
	GameOverScreen - Foi adicionado Tela de Fim de Jogo
	CreditScreen - Foi adicionado Tela de Créditos constando o nome dos Integrantes
	LifeBar	- Foi adicionado uma barra que conta as vidas restantes do jogador
	Gunbar - Foi adicionado uma barra que indica o tipo de disparo do jogador
	Scorebar - Foi adicionado uma barra que mostra a pontuação do jogador

	PlayerSprite - Foi adicionado uma sprite animada da nave do jogador
	EnemySprite - Foi adicionado três sprites animadas das naves inimigas
	BossSprite - Foi adicionado quatro sprites animadas dos chefes, sendo cada uma delas uma caricatura de um dos integrantes
	BulletSprite - Foi adicionado uma sprite animada do tiro inimigo e três sprites animadas de tiro do jogador
	BackgroundSprite - Foi adicionado uma sprite animada de fundo 
	ExplosionSprite - Foi adicionado uma sprite animada para quando os inimigo ou o jogador é explodido
	PowerupSprites - Foi adicionados sprites de powerups
	ScoreSprite - Foi adicionado uma sprite para contador de pontos
	AsteroidSprite - Foi adicionado uma sprite animada de um asteroide

	OpennigMusic - Foi adicionado uma musica de tema ao jogo, sendo tocada durante todo o jogo
	GameNameMusic - Foi adicionado um som com o nome do jogo, executado quando o jogador está no StartScreen
	GameStartMusic - Foi adicionado um som que é reproduzido quando o jogador inicia a partida
	GameOverMusic - Foi adicionado um som que é reproduzido quando o jogador perde o jogo
	NavigatingMusic - Foi adicionado um som que é reproduzido quando o jogador está navegando pelas opções do StartScreen
	ExplosionMusic - Foi adicionado um som que é reproduzido quando o jogador ou uma nave inimiga é explodida
	BossMusic - Foi adicionado um som que é reproduzido pelos chefões enquanto o jogador os enfrenta
	FiringMusic - Foi adicionado um som que é reproduido quando o jogador atira

	- A vida e a skin dos Inimigos comuns são geradas aleatoriamente
	- A skin dos Chefões são gerados aleatoriamente
	- O tempo para atirar de todos os inimigos são gerados aletoriamente
	- A posição e o tipo de Powerup é gerado aleatoriamente
	- Os asteroides são gerados aleatoriamente (após a onda 50), a posição onde aparece também é aleatório
	- Dificuldade progressiva a cada nova onda gerada
	
	*Notas: Todas as Sprites e Sons utilizados neste projeto foram feitos pelo grupo de discentes
			A aparência do jogo foi toda pensada e trabalhada para se assemelhar a um jogo retrô

Screenshots:

![alt Asteroid](https://github.com/matheustheus27/galaxian-EarthDefenders/blob/master/Screenshots/StartScreen.pngraw=true=80x20 ) ![alt Asteroid](https://github.com/matheustheus27/galaxian-EarthDefenders/blob/master/Screenshots/Asteroid.png?raw=true=100x20) 

	
