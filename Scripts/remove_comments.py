#!/usr/bin/env python3

import os
import re

# Regex patterns for single-line and multi-line comments.
single_line_comment_pattern = re.compile(r'//.*')
multi_line_comment_pattern = re.compile(r'/\*.*?\*/', re.DOTALL)

def remove_comments_from_code(code: str) -> str:
    """
    Removes C-style single and multi-line comments from the given code string.
    Returns cleaned code.
    """
    # Remove multi-line comments
    without_multiline = re.sub(multi_line_comment_pattern, '', code)
    # Remove single-line comments
    without_single_line = re.sub(single_line_comment_pattern, '', without_multiline)
    return without_single_line

def process_file(filepath: str):
    """
    Reads the file at 'filepath', strips comments, and writes a
    'no_comments_<filename>' copy in the same directory.
    """
    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
        code = f.read()
    
    cleaned_code = remove_comments_from_code(code)
    
    directory, filename = os.path.split(filepath)
    new_filename = f"no_comments_{filename}"
    new_filepath = os.path.join(directory, new_filename)
    
    with open(new_filepath, 'w', encoding='utf-8') as f:
        f.write(cleaned_code)
    
    print(f"Created: {new_filepath}")

def main(root_dir: str):
    """
    Recursively walks through 'root_dir' and processes all .c and .h files found.
    """
    for dirpath, dirnames, filenames in os.walk(root_dir):
        for filename in filenames:
            if filename.lower().endswith(('.c', '.h')):
                full_path = os.path.join(dirpath, filename)
                process_file(full_path)

if __name__ == "__main__":
    # Example usage:
    # python remove_comments.py /path/to/project/folder
    import sys
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <root_directory>")
        sys.exit(1)
    
    root_directory = sys.argv[1]
    main(root_directory)
