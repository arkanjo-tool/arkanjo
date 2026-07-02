\page developer-guide Developer Guide

# Similarity Model

ArKanjo represents code duplication as a graph, implemented in the `Similarity_Table` class.

## Graph Model

- Nodes: functions (identified by their path)
- Edges: similarity between functions
- Weight: similarity score (@f$0.0@f$ - @f$1.0@f$)

```
A ------0.9------ B
 \               /
  \            0.15
   \           /
    +--0.85---C
```

- The graph is undirected

## Threshold

Edges are considered during analysis only if:

@f[
    \text{similarity} \geq \text{threshold}
@f]

## Implementation

This model is implemented in the `Similarity_Table` class.