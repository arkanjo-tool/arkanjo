# Similarity Model

ArKanjo represents code duplication as a graph, implemented in the `Similarity_Table` class.

## Overview of similarity calculations

We have $\mathcal{V}$, where it is defined by the set of all possible words (*tokens*).

$$\mathcal{V}=\{w_1​,w_2​,\dots,w_n\}$$

For each file $d$, we have a list of words.

$$d = \left(\text{"int"}, \text{"main"}, \text{"return"}, \text{"0"}, \dots\right)$$

Each document becomes a counting vector:

$$\text{BoW}(d)=(c_1​,c_2​,\dots,c_n​)$$

where

$$c_i​=\text{number of times that $w_i$ appears in $d$}$$

and then we calculate the relative frequency of the word in the document

$$\text{TF}(w,d)=\frac{\text{freq}(w,d)}{\sum_k\text{freq}(w_k,d)}$$

and Inverse Document Frequency for common words to be less impactful than rare words

$$\text{IDF}(w)=\log\left(\frac{N}{\text{df}(w)}\right)$$

$$N=\text{total number of documents}$$
$$\text{df}(w)=\text{number of documents that contain $w$}$$

concluding the result by the weight of each word as

$$\text{TF-IDF}(w,d)=\text{TF}(w,d)\cdot\text{IDF}(w)$$

Each document becomes a vector $\vec{d}$ defined by

$$\vec{d}=(x_1,x_2,\dots,x_n)$$
$$x_i=\text{TF-IDF}(w_i,d)$$

Finally, the calculation of similarity between two documents.

$$\text{sim}(d_1,d_2)=\frac{\vec{d}_1\cdot\vec{d}_2}{\lvert\lvert\vec{d}_1\rvert\rvert\cdot\lvert\lvert\vec{d}_2\rvert\rvert}$$

### Intuitions

* Each document: a point in a high-dimensional space

* Similarity: angle between vectors

* Similar code: same words/tokens
* Similar structure: similar distribution of terms

## Graph Model

- Nodes: functions (identified by their path)
- Edges: similarity between functions
- Weight: similarity score ($0.0$ - $1.0$)

```
A ----0.9---- B
 \           /
  +--0.85---C
```

- The graph is undirected

## Threshold

Edges are considered during analysis only if:

$$\text{similarity} \geq \text{threshold}$$

## Implementation

This model is implemented in the `Similarity_Table` class.