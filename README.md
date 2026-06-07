# AVL Tree Management System

## Overview

This project is a C++ implementation of a self-balancing AVL tree.

The application allows users to create, modify, analyze, and visualize a binary search tree through a hierarchical menu system. Tree balancing is performed automatically to maintain efficient search, insertion, and deletion operations.

The project was developed to study advanced tree data structures and balancing algorithms.

## Features

### Tree Structure

Each node contains:

* Unsigned integer key
* Information field
* References to child nodes
* Height information for balancing

### Core Operations

* Create a binary search tree
* Insert new nodes
* Delete nodes
* Search for elements
* Clear the entire tree
* Display tree contents

### Tree Traversal

Support for standard traversal methods:

* Inorder traversal
* Preorder traversal
* Postorder traversal

Tree contents are displayed in a tabular format including:

* Node number
* Key value
* Information field

### AVL Balancing

Automatic balancing using:

* Left rotations
* Right rotations
* Double rotations
* Balance factor calculations
* Height updates

### Tree Analysis

Depending on the selected variant, additional operations may include:

* Counting leaves on a specified level
* Counting leaves on all levels
* Removing leaves
* Counting internal nodes
* Comparing subtrees
* Finding paths between nodes
* Counting nodes in subtrees

## Technologies

* C++
* AVL Trees
* Binary Search Trees
* Recursive Algorithms
* Dynamic Memory Management

## Learning Objectives

This project demonstrates:

* Binary search tree construction
* Self-balancing tree algorithms
* Recursive traversal techniques
* Tree rotations
* Hierarchical menu systems
* Dynamic memory management
* Efficient search structures

## Performance

AVL trees guarantee:

* O(log n) insertion
* O(log n) deletion
* O(log n) search

through automatic balancing after structural modifications.

## Compilation

Compile using a standard C++ compiler:

```bash
g++ main.cpp -o AVLTree
./AVLTree
```

## Author

Educational project developed for studying self-balancing binary search trees and advanced data structures in C++.
