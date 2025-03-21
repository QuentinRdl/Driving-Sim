#!/bin/bash

TEX_FILE="driving_sim_report"
OUT_DIR="../out"

if [ ! -f "${TEX_FILE}.tex" ]; then
    echo "Erreur : Le fichier ${TEX_FILE}.tex n'existe pas !"
    exit 1
fi

#rm -rf "$OUT_DIR/*.pdf"

mkdir -p "$OUT_DIR"

# Utilisation de latexmk pour compiler en PDF avec les passes nécessaires (bibtex, glossaires, etc.)
latexmk -pdf -interaction=nonstopmode -output-directory="$OUT_DIR" "${TEX_FILE}.tex"
EXIT_CODE=$?

# On supprime tout les fichiers excepter le PDF
#find "$OUT_DIR" -type f ! -name "${TEX_FILE}.pdf" -delete

echo "Le processus de compilation s'est terminé. Voici l'exit code de la dernière commande : ${EXIT_CODE}"
exit $EXIT_CODE
