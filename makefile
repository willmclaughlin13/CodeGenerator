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
	rm -rf Team10P3
	mkdir Team10P3
	cp Project3.cpp Team10P3
	cp SyntacticalAnalyzer.h Team10P3
	cp SyntacticalAnalyzer.cpp Team10P3
	cp CodeGenerator.h Team10P3
	cp CodeGenerator.cpp Team10P3
	cp makefile Team10P3
	cp README.txt Team10P3
	tar cfvz Team10P3.tgz Team10P3
	cp Team10P3.tgz ~tiawatts/cs460drop
