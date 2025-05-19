#!/usr/bin/env python3

import os

def remove_newlines_from_file(filepath: str):
    """
    Reads the file at 'filepath', removes newline characters, and overwrites
    the existing file in place.
    """
    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()
    
    # Remove newline characters
    oneline_content = content.replace('\n', ' ')
    
    # Overwrite the original file with modified text
    with open(filepath, 'w', encoding='utf-8') as f:
        f.write(oneline_content)
    
    print(f"Removed newlines from: {filepath}")

def main(root_dir: str):
    """
    Recursively walks through 'root_dir', finds all .txt files,
    and removes newlines in place.
    """
    for dirpath, dirnames, filenames in os.walk(root_dir):
        for filename in filenames:
            if filename.lower().endswith('.txt'):
                full_path = os.path.join(dirpath, filename)
                remove_newlines_from_file(full_path)

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <root_directory>")
        sys.exit(1)
    
    root_directory = sys.argv[1]
    main(root_directory)
