all: aaf-ex aaf-cf aaf-adm aaf-stb aaf-cmp aaf-prf

aaf-ex: aaf-ex.o
	g++ -Wall -Wextra -O3 -DNDEBUG -o ../../build/aaf-ex ../../build/aaf-ex.o -L../../build -lcadical

aaf-ex.o:
	g++ -Wall -Wextra -O3 -DNDEBUG -o ../../build/aaf-ex.o -c ./ex.cpp

aaf-cf: aaf-cf.o
	g++ -Wall -Wextra -O3 -DNDEBUG -o ../../build/aaf-cf ../../build/aaf-cf.o -L../../build -lcadical
aaf-cf.o:
	g++ -Wall -Wextra -O3 -DNDEBUG -o ../../build/aaf-cf.o -c ./cf.cpp

aaf-adm: aaf-adm.o
	g++ -Wall -Wextra -O3 -DNDEBUG -o ../../build/aaf-adm ../../build/aaf-adm.o -L../../build -lcadical
aaf-adm.o:
	g++ -Wall -Wextra -O3 -DNDEBUG -o ../../build/aaf-adm.o -c ./adm.cpp

aaf-stb: aaf-stb.o
	g++ -Wall -Wextra -O3 -DNDEBUG -o ../../build/aaf-stb ../../build/aaf-stb.o -L../../build -lcadical
aaf-stb.o:
	g++ -Wall -Wextra -O3 -DNDEBUG -o ../../build/aaf-stb.o -c ./stb.cpp

aaf-cmp: aaf-cmp.o
	g++ -Wall -Wextra -O3 -DNDEBUG -o ../../build/aaf-cmp ../../build/aaf-cmp.o -L../../build -lcadical
aaf-cmp.o:
	g++ -Wall -Wextra -O3 -DNDEBUG -o ../../build/aaf-cmp.o -c ./cmp.cpp

aaf-prf: aaf-prf.o
	g++ -Wall -Wextra -O3 -DNDEBUG -o ../../build/aaf-prf ../../build/aaf-prf.o -L../../build -lcadical
aaf-prf.o:
	g++ -Wall -Wextra -O3 -DNDEBUG -o ../../build/aaf-prf.o -c ./prf.cpp
