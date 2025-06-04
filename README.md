# Huffman Coding Compression Tool

This project implements a Huffman coding compression algorithm in C++. It 
performs character frequency analysis, constructs a binary Huffman tree, 
and encodes input data into a compressed binary format.

## Features

- Frequency-based character encoding using Huffman trees
- Deterministic output with tie-breaking rules
- Outputs:
  - Huffman code table (sorted by code length and character)
  - First 8 bytes of the compressed binary
  - Compressed size and compression ratio

## Technologies Used

- C++
- Standard Template Library (STL)
- File I/O
- Binary trees
- Bitwise operations

## How to Run

1. Clone this repo:
   ```bash
   git clone https://github.com/Randomdude951/huffman-coding.git
   cd huffman-coding

