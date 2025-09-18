# Parallel Computing with LU Decomposition and LCS

This repository contains implementations of **LU Decomposition** and **Longest Common Subsequence (LCS)** using **parallel computing techniques** such as **OpenMP** (shared-memory parallelism) and **MPI** (distributed-memory parallelism). The project demonstrates how numerical and string-based problems can be efficiently solved using parallel and distributed computing methodologies.

---

## Introduction

- **LU (Lower-Upper) Decomposition**  
  LU decomposition is a method for solving systems of linear equations by decomposing a matrix into lower and upper triangular matrices. When implemented in parallel computing environments such as OpenMP, LU decomposition accelerates matrix operations and improves computational efficiency, particularly on shared-memory architectures.

- **LCS (Longest Common Subsequence)**  
  The LCS problem is solved using dynamic programming and widely applied in parallel computing environments with MPI. It efficiently finds the longest ordered sequence common to two input sequences. Using MPI enables distribution of computations across multiple processors, reducing execution time and managing larger input sizes.

Both LU decomposition and LCS benefit significantly from **parallelization**, making them exemplary case studies for evaluating **OpenMP vs MPI** performance.

---

## Methodology

- **LU Decomposition**  
  Pseudocode Reference:  
  [LU Decomposition Example and Pseudocode](https://www.researchgate.net/figure/a-LU-decomposition-pseudocode-b-Example-on-LU-decomposition_fig4_220624910)  

- **LCS (Longest Common Subsequence)**  
  Pseudocode Reference:  
  *Introduction to Algorithms, 3rd Edition* by  
  Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, and Clifford Stein  

---

## Performance Comparison (OpenMP vs MPI)

This project compares **OpenMP** and **MPI** implementations of LU and LCS with respect to:  
- **Execution time**  
- **Scalability**  
- **Communication overhead**  
- **Computation efficiency**  

### Visualizations in Report:
- Longest Common Subsequence Performance Graph
- LCS Communication & Computation Graph
- LU Decomposition Performance Graph
- LU Decomposition Communication & Computation Graph


---

## References

- [Parallel Computing the Longest Common Subsequence (LCS) on GPUs: Efficiency and Language Suitability](https://www.researchgate.net/publication/252320985_Parallel_Computing_the_Longest_Common_Subsequence_LCS_on_GPUs_Efficiency_and_Language_Suitabilit)  
- [Introduction to Algorithms, 4th Edition (Cormen, Leiserson, Rivest, Stein)](https://dl.ebooksworld.ir/books/Introduction.to.Algorithms.4th.Leiserson.Stein.Rivest.Cormen.MIT.Press.9780262046305.EBooksWorld.ir.pdf)  

---

## Conclusion

The implementation of **LU decomposition** and **LCS decomposition** using **MPI** and **OpenMP** highlights the advantages of applying **parallel and distributed computing** techniques to both numerical and sequence-based problems.

- **MPI** facilitated efficient task distribution across processors, enabling scalability for larger datasets and matrices.  
- **OpenMP** provided effective shared-memory parallelism, improving processing speed for smaller to medium-scale problems.  

Overall, the project illustrates how decomposition algorithms can be optimized through parallel computing, making them faster and more suitable for real-world computational challenges.  
