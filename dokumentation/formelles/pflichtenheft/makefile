

default: Pflichtenheft.pdf

# must be compiled twice , so table of contents actually shows up, so dont be confused ;D
Pflichtenheft.pdf:
	pdflatex Pflichtenheft.tex
	pdflatex Pflichtenheft.tex

clean:
	rm *.log *.aux *.pdf *.toc *.out

.PHONY: default clean

