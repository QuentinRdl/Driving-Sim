#!/bin/bash

TEX_FILE="driving_sim_report"
OUT_DIR="../out"

if [ ! -f "${TEX_FILE}.tex" ]; then
    echo "Erreur : Le fichier ${TEX_FILE}.tex n'existe pas !"
    exit 1
fi

mkdir -p "$OUT_DIR"

# Première passe : compilation initiale avec pdflatex
echo "Première compilation avec pdflatex..."
pdflatex -interaction=nonstopmode -output-directory="$OUT_DIR" "${TEX_FILE}.tex"

# Deuxième passe : exécution de bibtex sur le fichier .aux généré
echo "Exécution de bibtex..."
bibtex "${OUT_DIR}/${TEX_FILE}.aux"

# Troisième passe : exécution de makeglossaries sur le fichier .glo généré
echo "Exécution de makeglossaries..."
makeglossaries "${OUT_DIR}/${TEX_FILE}"

# Quatrième passe : recompilation avec pdflatex pour intégrer les références et le glossaire
echo "Deuxième compilation avec pdflatex..."
pdflatex -interaction=nonstopmode -output-directory="$OUT_DIR" "${TEX_FILE}.tex"

# Cinquième passe : dernière compilation pour s'assurer que tout est à jour
echo "Troisième compilation avec pdflatex..."
pdflatex -interaction=nonstopmode -output-directory="$OUT_DIR" "${TEX_FILE}.tex"
