# Matrix Calculator

## Installation

Just clone the repo into any directory you'd like, and in the terminal type 'make run' to run the calculator.

## How to use

Your equations and matrices go into the file input.txt in the following format:

- The first line contains your equation
- Everything following the first line should be a matrix, with each matrix separated by a double line break
- Example:
> (your equation here)
> 
> 1 2 3  
> 4 5 6  
> 7 8 9  
> 
> 2 4 6  
> 8 10 12  
> 14 16 18  

The output to your equation is written to output.txt.

Your equation should be written in postfix, and you should refer to your matrices with single-character uppercase letters, starting with 'A' and continuing alphabetically based on the order you wrote them in the input file.

Example:

> A B *
>
> 1 2 3  
> 4 5 6  
> 7 8 9  
>
> 2 4 6  
> 8 10 12  
> 14 16 18  

The above means matrix multiply the first matrix (1 2 3...) by the second matrix (2 4 6...). If there were a third matrix, it would be referred to as 'C', and so forth.

### What is postfix?

Postfix is a way to write equations such that the arguments of an operation preceed the operator. As shown above, A B * is postfix for A * B; the arguments of the multiplication (A and B) preceed the multiplication symbol.

More examples:

> A B + C -  
> means (A + B) - C

> A B - A B + *  
> means (A - B) * (A + B)

> A 2 ^ B 2 ^ + 1 2 / ^  
> means ((A^2) + (B^2))^(1/2)

> A rref  
> means rref(A)

### Why postfix?

Because order of operations is hard and too ambiguous for me to account for all of the edge cases while still retaining my sanity.

## Supported Operations

### Matrix Multiplication

Operator: '*'  
Number of arguments: 2  
Return type: Matrix  
Returns the matrix product of two matrices.

### Matrix Addition

Operator: '+'  
Number of arguments: 2    
Return type: Matrix  
Returns the matrix sum of two matrices.

### Matrix Subtraction

Operator: '-'  
Number of arguments: 2    
Return type: Matrix  
Returns the matrix difference of two matrices.

### Determinant

Operator: 'det'  
Number of arguments: 1  
Return type: Number  
Returns the determinant of a matrix.

### Matrix Inverse

Operator: '^'  
Number of arguments: 2    
Return type: Matrix  
Returns the matrix inverse of the first argument if '-1' is the second argument.

### Reduced Row Eschelon Form

Operator: 'rref'  
Number of arguments: 1    
Return type: Matrix  
Returns the reduced row eschelon form of the argument.

### Basis

Operator: 'basis'  
Number of arguments: 1  
Return type: Matrix  
Returns the basis of the space spanned by the columns of a matrix.

### Null Space

Operator: 'null'  
Number of arguments: 1  
Return type: Matrix  
Returns the basis of the null space of a matrix.

### Transpose

Operator: '^'  
Number of arguments: 2    
Return type: Matrix  
Returns the transpose of the first argument if 't' is the second argument.

### Find Eigenvalues

Operator: 'eval'  
Number of arguments: 1  
Return type: Vector    
Returns a vector containing all of the eigenvalues for the given matrix.

### Vector

Operator: 'vec'  
Number of arguments: 2    
Return type: Vector  
Returns the arg2'th vector in argument 1.

### Dot Product

Operator: '.'  
Number of arguments: 2    
Return type: Number  
Only works if arguments are vectors. Returns the dot product of two vectors.

### Vector Magnitude

Operator: '||'  
Number of arguments: 1  
Return type: Number  
Only works if the argument is a vector. Returns the magnitude of the vector.

### Orthonormalize Matrix

Operator: 'orth'  
Number of arguments: 1  
Return type: Matrix  
Makes a matrix orthogonal; that is, makes all columns of matrix orthonormal to each other.

### Change of Coordinates Matrix

Operator: '<-'  
Number of arguments: 2    
Return type: Matrix  
Returns the change-of-coordinates matrix from argument 2 to argument 1.

### Lower-Upper Decomposition

Operator: 'l'  
Number of arguments: 1    
Return type: Matrix  
Returns lower triangular matrix in LU Decomposition.

Operator: 'u'  
Number of arguments: 1    
Return type: Matrix  
Returns upper triangular matrix in LU Decomposition.

### QR Decomposition

Operator: 'q'  
Number of arguments: 1    
Return type: Matrix  
Returns orthogonal matrix used in QR Decomposition.

Operator: 'r'  
Number of arguments: 1    
Return type: Matrix  
Returns upper triangular matrix R in QR Decomposition.

## FAQ

### How do you denote a vector?

A vector is just a matrix object with one column. This means you can do matrix operations on a vector, but you can't necessarily do the opposite.

### Why isn't \<operation\> supported?

Probably because I either forgot to add it or I didn't learn it in my linear algebra class.

### What is the runtime of \<operation\>?

I haven't calculated runtimes only because so many operations are contingent upon so many other operations, but I can assure you that the runtime never exceeds polynomial time. Just don't try to use my calculator on 1000x1000 matrices or anything.

### Why postfix and not prefix? 

Because I made a mistake and wrote it to support postfix and now I'm in too deep.

### Why can't I name my own matrices?

Like I said, this is how I wrote it and I'm in too deep, man.

### Are these really frequently asked questions?

No, I'm just guessing what they would be, okay
