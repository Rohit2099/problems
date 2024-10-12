# Maximum Leaf Spanning Tree

### Problem Statement:

This assignment focuses on solving the **Maximum Leaf Spanning Tree** problem using a parameterized algorithm. The algorithm takes a graph (with specified number of nodes and edges) as input and finds a spanning tree with at least **k leaves**, if one exists.

---

## Input:

-   **Number of nodes** and **edges**: The graph will be created randomly based on the input number of nodes and edges.
-   **k value**: This value is used as a parameter for the algorithm.

You can modify these inputs in the following lines of the Python script:

-   **Number of nodes (n):** Line 107
-   **Number of edges (e):** Line 108
-   **K value:** Line 109

File: `maximum-leaf-spanning-tree.py`

---

## Output:

-   The algorithm produces both **textual output** and **visual output**.
-   Visual Output:
    -   Nodes colored in **green** represent the leaves of a spanning tree that has at least `k` leaves (if one exists).
    -   Nodes colored in **blue** represent normal vertices.

---

## How to Execute:

### Dependencies:

Install the following Python libraries:

-   **Networkx**
-   **Matplotlib**

### Steps to Run:

1. Set the number of nodes, edges, and the `k` value in the Python script as described above.
2. Execute the program using the following command:
    ```bash
    $ python3 maximum-leaf-spanning-tree.py
    ```
