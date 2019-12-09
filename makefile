P3.out : Project3.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o CodeGenerator.o
	g++ -g -o P3.out Project3.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o CodeGenerator.o

Project3.o : Project3.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ -g -c Project3.cpp

SetLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ -g -c SyntacticalAnalyzer.cpp

CodeGenerator.o : CodeGenerator.cpp CodeGenerator.h
	g++ -g -c CodeGenerator.cpp

clean :
	rm P3.out *.p1 *.p2 *.lst *.dbg

run : P3.out
	@clear
	./P3.out test.ss
	@cat test.cpp



submit : Project3.cpp LexicalAnalyzer.h SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt
	rm -rf TeamNameP3
	mkdir TeamNameP3
	cp Project3.cpp TeamNameP3
	cp SyntacticalAnalyzer.h TeamNameP3
	cp SyntacticalAnalyzer.cpp TeamNameP3
	cp CodeGenerator.h TeamNameP3
	cp CodeGenerator.cpp TeamNameP3
	cp makefile TeamNameP3
	cp README.txt TeamNameP3
	tar cfvz TeamNameP3.tgz TeamNameP3
	cp TeamNameP3.tgz ~tiawatts/cs460drop
