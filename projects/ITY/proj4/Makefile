# MAKEFILE ITY proj4
# 
# Autor: Dominik Křivohlávek
# 1BIT

all:
	latex proj4.tex
	bibtex proj4.aux
	latex proj4.tex
	latex proj4.tex
	dvips -t a4 proj4.dvi
	ps2pdf proj4.ps

clean:
	rm *.aux *.dvi *.pdf *.log *.ps *.blg *.bbl