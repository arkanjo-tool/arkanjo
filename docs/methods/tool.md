\page method Method

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

We have @f$\mathcal{V}@f$, where it is defined by the set of all possible words (*tokens*).

@f[
    \mathcal{V}=\{w_1​,w_2​,\dots,w_n\}
@f]

For each function @f$f@f$, we have a list of words.

@f[
    f = \left(\text{"int"}, \text{"main"}, \text{"return"}, \text{"0"}, \dots\right)
@f]

Each function body becomes a counting vector:

@f[
    \text{BoW}(f)=(c_1​,c_2​,\dots,c_n​)
@f]

where

@f[
    c_i​=\text{number of times that $w_i$ appears in $f$}
@f]

and then we calculate the relative frequency of the word in the document

@f[
    \text{TF}(w,f)=\frac{\text{freq}(w,f)}{\sum_k\text{freq}(w_k,f)}
@f]

and Inverse Document Frequency for common words to be less impactful than rare words

@f[
    \text{IDF}(w)=\log\left(\frac{N}{\text{df}(w)}\right)
@f]

@f[N=\text{total number of functions}@f]
@f[\text{df}(w)=\text{number of functions that contain $w$}@f]

concluding the result by the weight of each word as

@f[\text{TF-IDF}(w,f)=\text{TF}(w,f)\cdot\text{IDF}(w)@f]

Each function body is represented as a vector @f$\vec{f}@f$ defined by

@f[\vec{f}=(x_1,x_2,\dots,x_n)@f]
@f[x_i=\text{TF-IDF}(w_i,f)@f]

Finally, the calculation of similarity between two functions.

@f[\text{sim}(f_1,f_2)=\frac{\vec{f}_1\cdot\vec{f}_2}{||\vec{f}_1||\cdot||\vec{f}_2||}@f]

### Intuitions

* Each function: a point in a high-dimensional space

* Similarity: angle between vectors

* Similar code: same words/tokens

* Similar structure: similar distribution of terms
