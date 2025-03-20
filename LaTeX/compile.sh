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
echo "Le processus de compilation s'est terminé. Voici l'exit code de la dernière commande : $?"
sleep 10
exit 0