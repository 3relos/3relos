#!/bin/bash

# Zahl abfragen
read -p "Übungsblatt Nr. eingeben: " zahl

# Source directory with the PDF-files
SOURCE_DIR="/Users/3relos/Downloads/Uebungsblatt$zahl"
# Destiny directory, where the PDF-files should be copied
DEST_DIR="/Users/3relos/sciebo/Uni/Bachelor/Theo/Tutorium_WS24-25/Abgaben/ÜB$zahl"

# Check if the source dir exists
[ -d "$SOURCE_DIR" ] || { echo "Quellverzeichnis existiert nicht: $SOURCE_DIR"; exit 1; }

# Check if the destiny dir exists, if not create it
[ -d "$DEST_DIR" ] || { echo "Zielverzeichnis existiert nicht. Erstelle: $DEST_DIR"; mkdir -p "$DEST_DIR"; }

# Searches for all PDF-files in the source dir and copy it to the destiny dir
find "$SOURCE_DIR" -type f -iname "*.pdf" -exec cp {} "$DEST_DIR" \;

echo "All PDF-files were copied to 'ÜB$zahl'."
