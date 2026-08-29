# Similarity Methods

ArKanjo supports multiple methods for detecting function-level code duplication.

Each method computes similarity between functions using a different approach.
Some methods require additional dependencies, while others are fully implemented
in C++.

## Configuration

Methods are enabled through `config.json`:

```json
{
    "methods": [
        "tfidf",
        "diff",
        "ast",
        "embedding"
    ]
}
