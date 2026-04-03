# Similarity Model

ArKanjo represents code duplication as a graph, implemented in the `Similarity_Table` class.

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