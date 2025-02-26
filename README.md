📌 Thread-Safe Multi-Threaded Search Engine

A C/C++ program that searches for a specific character across multiple files using threading while ensuring thread safety.

🔹 Project Overview

This project implements a thread-safe multi-threaded search engine using C/C++ and pthreads. It distributes search tasks across multiple worker threads while synchronizing access to shared resources using mutex locks to prevent race conditions.

📂 Files Included

ASS.c → The thread-safe version of the search engine.

in.txt → Input file containing file names and search parameters.

out.txt → Output file with sorted results.

README.md → Project documentation.

⚙️ How It Works

The program reads an input file (in.txt) with:

N → Number of files to search.

TH → Threshold value for categorization.

It spawns T worker threads, each responsible for a subset of files.

Each thread:

Reads the file.

Counts occurrences of the target CHAR.

Uses mutex locks to safely update global shared variables.

The program outputs:

Total occurrences.

Number of files above, equal to, or below the threshold.

A sorted list of files based on occurrence count.
