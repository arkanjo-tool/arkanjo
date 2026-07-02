# Architecture

ArKanjo follows a two-stage architecture designed to scale for large codebases:

- **Preprocessor**: performs heavy computation once
- **Query Responder**: enables fast, repeated queries

```
                   +---------------------+
                   |   Query Responder   |
                   |---------------------|
                   | - explorer          |
                   | - function          |
                   | - duplication report|
                   +----------+----------+
                              |
                              v
                   +---------------------+
                   |     Query Output    |
                   +---------------------+
                              ^
                              |
                     (reads cached data)
                              |
=============================================================
|                       PREPROCESSOR                        |
|-----------------------------------------------------------|
|                                                           |
|  +---------------------+       +-----------------------+  |
|  |  Temporary Codebase |---+   |  Code Duplication DB  |  |
|  +----------+----------+   |   +-----------+-----------+  |
|             ^              |               ^              |
|             |              |               |              |
|  +----------+----------+   |   +-----------+-----------+  |
|  |   Function Parser   |   |   |Code Duplication Finder|  |
|  |---------------------|   |   |-----------------------|  |
|  | - Tree-sitter       |   +-->| - TF-IDF + Cosine     |  |
|  |                     |       | - Line diff method    |  |
|  +----------+----------+       +-----------+-----------+  |
|             ^                             ^               |
|             |                             |               |
|       +-----+-----------------------------+-----+         |
|       |             Input Codebase              |         |
|       +-----------------------------------------+         |
|                                                           |
=============================================================
```

**Pipeline overview:**

```
[1] Input -> [2] Parse -> [3] Temp -> [4] Detect -> [5] DB -> [6] Query
```