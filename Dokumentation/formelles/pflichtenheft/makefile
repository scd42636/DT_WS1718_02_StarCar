

default: Pflichtenheft.pdf

Pflichtenheft.pdf:
	pdflatex Pflichtenheft.tex
	pdflatex Pflichtenheft.tex

#previous toc sometimes is needed to create tableofcontents
clean_notoc:
	rm *.log *.aux *.pdf *.out

clean:
	rm *.log *.aux *.pdf *.toc *.out

.PHONY: default clean

