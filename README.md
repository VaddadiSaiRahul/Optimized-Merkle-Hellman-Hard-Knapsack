# Optimized-Merkle-Hellman-Hard-Knapsack
Parallelized Dynamic Programming implementation of Hard Variant of Merkle-Hellman Knapsack for Public key encryption. This optimized variant, ensures increased security from cryptanalysis like Shamir's attack on the Traditional Merkle-Hellman Knapsack. Even though the hard variant involves solving an inefficient subset-sum compared to the traditional knapsack, through optimization it is ensured to work faster than the traditional variant on most system configurations (depends on the system!!!). Therefore, ensuring both speed and reliability of the system.

Constraints :
1. Number of elements must be 8, since messages entered are in alphabetics i.e. from a-z or A-Z => 8-bit ASCII representation
2. Messages must be entered without a space. E.g. - Thisisanimportantmessage

Descriptions :
1. knapsack.txt contains sequences to begin with. Superincreasing sequences to be tried with the Traditional Knapsack codes and Non-Superincreasing sequence with the optimized Hard Knapsack codes.

2. Traditional Knapsack folder contains 2 files - one which is a serialized version of traditional merkle-hellman and the other a parallelized version.

3. Hard Knapsack folder contains 2 files - one which is a serialized version of hard merkle-hellman and the other a dp parallelized version.

For detailed understanding of the system, check my paper https://cit.iict.bas.bg/CIT-2021/v-21-2/10341-Volume21_Issue_2-05_paper.pdf.
