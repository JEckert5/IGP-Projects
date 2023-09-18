# Apple is stupid so I have to this.

OPTS = -I /opt/homebrew/Cellar/sfml/2.6.0/include -I ./headers -L /opt/homebrew/Cellar/sfml/2.6.0/lib -std=c++17

LIBS = -l sfml-system -l sfml-window -l sfml-graphics -l sfml-audio -l sfml-network

CPPS = ./src/Animation.cpp ./src/Entity.cpp ./src/Player.cpp ./src/Bullet.cpp ./src/Dio.cpp

main:
	g++ ./src/main.cpp $(CPPS) $(OPTS) $(LIBS)

clean:
	rm a.out