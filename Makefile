all:
	@echo "Compilando..."
	gcc galaxian.c -o galaxian -lGL -lSOIL -lSDL -lSDLmain -lSDL_image -lSDL_mixer -lm




install-libraries:
	@echo "Instalando Bibliotecas..."
	sudo apt-get update
	sudo apt-get install libsoil-dev 
	sudo apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev

run:
	@echo "Executando..."
	./galaxian

clean:
	@echo "Removendo..."
	rm galaxian
