#!/bin/bash

# Created: Vasco (3relos)

# number abfragen
read -p "Enter exercise sheet no.: " number

# Source directory with the PDF-files
SOURCE_DIR="/Users/<Username>/Downloads/Uebungsblatt$number"
# Destiny directory, where the PDF-files should be copied
DEST_DIR="/Users/<Username>/Tutorium_WS24-25/Abgaben/Ex$number"

# Check if the source dir exists
[ -d "$SOURCE_DIR" ] || { echo "Source directory does not exist: $SOURCE_DIR"; exit 1; }

# Check if the destiny dir exists, if not create it
[ -d "$DEST_DIR" ] || { echo "Target directory does not exist. Create: $DEST_DIR"; mkdir -p "$DEST_DIR"; }

# Searches for all PDF-files in the source dir and copy it to the destiny dir
find "$SOURCE_DIR" -type f -iname "*.pdf" -exec cp {} "$DEST_DIR" \;

echo "All PDF-files were copied to '$DEST_DIR'."
