#Author Dennis Vymer, xvymer00
#This file uses qmake to generate a Makefile and hides it behind custom targets created.


#Compiling
compile: update
	make -f qMakefile
	rm -rf *.o qMakefile moc* *.cpp

update: src/CPP_Projekt.pro
	qmake -o qMakefile src/CPP_Projekt.pro


#Running
run: compile
	./chess

#Other stuff
clean:
	rm -f chess *.o qMakefile moc* *.cpp
	#rm -vrf !("Doxyfile"), use this in THE /DOC FOLTER, to remove all files in a given folder but the Doxyfile

doxygen:
	doxygen doc/Doxyfile

pack:
	zip -r xvymer00-xgospa00.zip src res examples Makefile README.txt doc/Doxyfile
