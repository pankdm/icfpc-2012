
compile:
	ghc --make change_code_blocks.hs

tex:
	pandoc write-up.md -H use-russian.tex --highlight-style zenburn -o write-up.tex

tex-minted:
	pandoc write-up.md -t json |./change_code_blocks |pandoc -f json -H use-russian.tex -o write-up.tex

pdf:
	xelatex -shell-escape write-up.tex

view:
	evince write-up.pdf
