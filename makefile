P3.out : Project3.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o CodeGenerator.o
	g++ -g -o P3.out Project3.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o CodeGenerator.o
										
Project3.o : Project3.cpp SetLimits.h SyntacticalAnalyzer.h CodeGenerator.h
	g++ -g -c Project3.cpp

SetLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp

LexicalAnalyzer.o : LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ -g -c LexicalAnalyzer.cpp

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ -g -c SyntacticalAnalyzer.cpp

CodeGenerator.o: CodeGenerator.cpp CodeGenerator.h
	g++ -g -c CodeGenerator.cpp

clean : 
	rm -f *.o P2.out *.gch *.x *.p1 *.p2 *.ss *.lst *.dbg *.exp

cleanProgram : 
	rm -f *.o P2.out 

cleanTestFiles : 
	rm -f *.p1 *.p2 *.p3 *.lst *.dbg *.exp

test-lst : 
	./cmpttlerrs.sh 

test-p3 : 
	./run-all.sh

submit : Project3.cpp LexicalAnalyzer.h LexicalAnalyzer.cpp SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt
	rm -rf TeamRP2
	mkdir TeamRP2
	cp Project3.cpp TeamRP2
	cp LexicalAnalyzer.h TeamRP2
	cp LexicalAnalyzer.cpp TeamRP2
	cp SyntacticalAnalyzer.h TeamRP2
	cp SyntacticalAnalyzer.cpp TeamRP2
	cp makefile TeamRP2
	cp README.txt TeamRP2
	tar cfvz TeamRP2.tgz TeamRP2
	cp TeamRP2.tgz ~tiawatts/cs460drop
