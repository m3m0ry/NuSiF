
PROJECT   := summary
PRJ_FILE  := $(PROJECT).tex
TEX_FILES := $(wildcard ../*.tex)
CFG_FILES := $(wildcard ../settings/*.tex)
#BIB_FILES := bibliography.bib

TEX_FLAGS := -output-directory
OUTPUT_DIR := build/

all: pdf
# all: dvi pdf ps


clean:
	rm -rf $(OUTPUT_DIR)
# 	rm -f *.log *.aux *.toc *.lof *.bbl 
# 	rm -f *.blg *.idx *.ind *.ilg *.glo *.gls
# 	rm -f *.lot *.loa *.equ *.lol *.nlo *.out
# 
# distclean: clean
# 	rm -f *.dvi *.pdf *.ps
# 	rm -f *~

# mrproper: distclean

pdf: $(PRJ_FILE) $(CFG_FILES) $(TEX_FILES) $(BIB_FILES)
	mkdir -p $(OUTPUT_DIR)
	mkdir -p $(OUTPUT_DIR)/contents/
	mkdir -p $(OUTPUT_DIR)/settings/
	pdflatex $(TEX_FLAGS) $(OUTPUT_DIR) $(PROJECT) 
	#cp $(BIB_FILES) build/; cd build/; \
	#bibtex $(PROJECT)
#	makeindex $(OUTPUT_DIR)$(PROJECT).idx -o $(OUTPUT_DIR)$(PROJECT).ind
#	makeindex $(OUTPUT_DIR)$(PROJECT).glo -o $(OUTPUT_DIR)$(PROJECT).gls
	pdflatex $(TEX_FLAGS) $(OUTPUT_DIR) $(PROJECT) 
	pdflatex $(TEX_FLAGS) $(OUTPUT_DIR) $(PROJECT) 

# ps: $(PROJECT).dvi
# 	dvips -o $(PROJECT).ps $(PROJECT).dvi
# 
# fastdvi: $(PRJ_FILE) task.tex abstract.tex chapter*.tex appendix*.tex references.bib
# 	latex $(PRJ_FILE)
# 
# dvi: $(PRJ_FILE) abstract.tex chapter*.tex appendix*.tex references.bib
# 	latex $(PROJECT)
# 	bibtex $(PROJECT)
# 	makeindex $(PROJECT).idx -s $(PROJECT).ist -o $(PROJECT).ind
# 	makeindex $(PROJECT).glo -s nomencl.ist -o $(PROJECT).gls
# 	latex $(PROJECT)
# 	latex $(PROJECT)
