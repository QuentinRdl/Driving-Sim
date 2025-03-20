#!/bin/bash

TEX_FILE="driving_sim_report"
OUT_DIR="../out"

if [ ! -f "${TEX_FILE}.tex" ]; then
    echo "Erreur : Le fichier ${TEX_FILE}.tex n'existe pas !"
    exit 1
fi

mkdir -p "$OUT_DIR"

pdflatex -interaction=nonstopmode -output-directory="$OUT_DIR" "${TEX_FILE}.tex"

# bibtex "$OUT_DIR/${TEX_FILE}"
# makeglossaries "$OUT_DIR/${TEX_FILE}"

pdflatex -interaction=nonstopmode -output-directory="$OUT_DIR" "${TEX_FILE}.tex"
pdflatex -interaction=nonstopmode -output-directory="$OUT_DIR" "${TEX_FILE}.tex"
if [ $? -ne 0 ]; then
    echo "Erreur : La compilation a échoué !"
    exit 1
fi
echo "✅ Compilation terminée : ${OUT_DIR}/${TEX_FILE}.pdf généré avec succès."
