#!/bin/bash

# Nom du fichier LaTeX principal (sans extension)
TEX_FILE="driving_sim_report"
OUT_DIR="../out"

# Vérification si le fichier existe
if [ ! -f "${TEX_FILE}.tex" ]; then
    echo "Erreur : Le fichier ${TEX_FILE}.tex n'existe pas !"
    exit 1
fi

# Création du dossier de sortie s'il n'existe pas
mkdir -p "$OUT_DIR"

# Première compilation (génère le PDF + références partielles)
pdflatex -interaction=nonstopmode -output-directory="$OUT_DIR" "${TEX_FILE}.tex"

# Vérification des références pour bibtex/glossaries (décommente si nécessaire)
# bibtex "$OUT_DIR/${TEX_FILE}"
# makeglossaries "$OUT_DIR/${TEX_FILE}"

# Deuxième compilation (résout les références)
pdflatex -interaction=nonstopmode -output-directory="$OUT_DIR" "${TEX_FILE}.tex"

# Troisième compilation (résout \pageref et table des matières à jour)
pdflatex -interaction=nonstopmode -output-directory="$OUT_DIR" "${TEX_FILE}.tex"

# Nettoyage des fichiers auxiliaires (mais garde le PDF)
find "$OUT_DIR" -type f ! -name "${TEX_FILE}.pdf" -delete

echo "✅ Compilation terminée : ${OUT_DIR}/${TEX_FILE}.pdf généré avec succès."
