
# Apple is stupid so I have to this.

MAC-OPTS = -I /opt/homebrew/Cellar/sfml/2.6.0/include -L /opt/homebrew/Cellar/sfml/2.6.0/lib
WIN-OPTS = -I C:\SFML-2.6.0-windows-vc17-64-bit\SFML-2.6.0\include -L C:\SFML-2.6.0-windows-vc17-64-bit\SFML-2.6.0\lib

LIBS = -l sfml-system -l sfml-window -l sfml-graphics -l sfml-audio -l sfml-network

main:
	g++ main.cpp $(MAC-OPTS) $(LIBS)

main-win:
	g++ main.cpp $(WIN-OPTS) $(LIBS)
