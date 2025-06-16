#!/bin/bash

# Created: Vasco (3relos)

# retrieve number
read -p "Enter exercise sheet no.: " number

# Source directory with the PDF-files
if [ $number -gt 9 ]
then
    SOURCE_DIR="/home/<user>/Downloads/Hausaufgabe $number" # Update path acoordingly to your needs!
else
    SOURCE_DIR="/home/<user>/Downloads/Hausaufgabe 0$number" # Update path acoordingly to your needs!
fi
# Destiny directory, where the PDF-files should be copied
DEST_DIR="/home/<user>/<path>/Blatt$number" # Update path acoordingly to your needs!

# Check if the source dir exists
[ -d "$SOURCE_DIR" ] || { echo "Source directory does not exist: $SOURCE_DIR"; exit 1; }

# Check if the destiny dir exists, if not create it
[ -d "$DEST_DIR" ] || { echo "Target directory does not exist. Create: $DEST_DIR"; mkdir -p "$DEST_DIR"; }

# Searches for all PDF-files in the source dir and copy it to the destiny dir
find "$SOURCE_DIR" -type f -iname "*.pdf" -exec cp {} "$DEST_DIR" \;

echo "All PDF-files were copied to '$DEST_DIR'."

# Searches and prints the number of pdf.files in destiny dir
echo "Number copied files: $(find $DEST_DIR -maxdepth 1 -type f | wc -l)"

# retrieve if source dir should be deleted
read -p "Delete original directory? [y/n] " del

# delete source dir if wanted
if [[ "$del" == "y" ]]
then
    rm -R "$SOURCE_DIR"
    echo "Deleted source directory '$SOURCE_DIR'"
else
    echo "As you wish!"
    echo "Not deleted source directory."
fi

echo "Finished!"
