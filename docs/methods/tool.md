# NLP text similarity using gensim

A function-level duplication detector that uses classical NLP techniques to compute semantic similarity between code functions.

Instead of structural comparison, it represents functions using token-based vectorization (gensim) and measures similarity in embedding space.

It is one of the interchangeable detection methods in ArKanjo.

## Preparing the environment

The NLP dependencies are **not** installed by default.

Install them using:

```sh
python3 -m pip install -r third-party/duplicate-code-detection-tool/requirements.txt

python3 -m nltk.downloader punkt
```

## Notes

We have $\mathcal{V}$, where it is defined by the set of all possible words (*tokens*).

$$\mathcal{V}=\{w_1​,w_2​,\dots,w_n\}$$

For each function $f$, we have a list of words.

$$f = \left(\text{"int"}, \text{"main"}, \text{"return"}, \text{"0"}, \dots\right)$$

Each function body becomes a counting vector:

$$\text{BoW}(f)=(c_1​,c_2​,\dots,c_n​)$$

where

$$c_i​=\text{number of times that $w_i$ appears in $f$}$$

and then we calculate the relative frequency of the word in the document

$$\text{TF}(w,f)=\frac{\text{freq}(w,f)}{\sum_k\text{freq}(w_k,f)}$$

and Inverse Document Frequency for common words to be less impactful than rare words

$$\text{IDF}(w)=\log\left(\frac{N}{\text{df}(w)}\right)$$

$$N=\text{total number of functions}$$
$$\text{df}(w)=\text{number of functions that contain $w$}$$

concluding the result by the weight of each word as

$$\text{TF-IDF}(w,f)=\text{TF}(w,f)\cdot\text{IDF}(w)$$

Each function body is represented as a vector $\vec{f}$ defined by

$$\vec{f}=(x_1,x_2,\dots,x_n)$$
$$x_i=\text{TF-IDF}(w_i,f)$$

Finally, the calculation of similarity between two functions.

$$\text{sim}(f_1,f_2)=\frac{\vec{f}_1\cdot\vec{f}_2}{\lvert\lvert\vec{f}_1\rvert\rvert\cdot\lvert\lvert\vec{f}_2\rvert\rvert}$$

### Intuitions

* Each function: a point in a high-dimensional space

* Similarity: angle between vectors

* Similar code: same words/tokens

* Similar structure: similar distribution of terms
