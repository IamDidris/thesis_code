#!/usr/bin/env python3

import os

def count_words(filepath: str) -> int:
    """
    Counts the number of words in the given file.
    """
    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
        text = f.read()
    words = text.split()
    return len(words)

def main(root_dir: str, threshold: int = 256):
    for dirpath, _, filenames in os.walk(root_dir):
        for filename in filenames:
            if filename.lower().endswith('.txt'):
                full_path = os.path.join(dirpath, filename)
                word_count = count_words(full_path)
                if word_count > threshold:
                    print(f"[WARNING] File '{full_path}' has {word_count} words, exceeding {threshold}.")

if __name__ == "__main__":
    import sys
    
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <root_directory> [threshold=256]")
        sys.exit(1)
    
    root_directory = sys.argv[1]
    # Optional second argument to override threshold
    word_limit = int(sys.argv[2]) if len(sys.argv) > 2 else 256
    
    main(root_directory, threshold=word_limit)
