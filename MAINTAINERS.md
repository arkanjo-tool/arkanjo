# Maintainers

## Integration Policy

Preferred integration method:
* Rebase and Merge

Alternative methods:
* Merge Commit for integration branches.
* Squash Merge only when preserving the original commit history is impractical.

When integrating:
* Preserve commit authorship.
* Reference the original PR.

## Merge Commit Format

```text
<subsystem>: merge pull request

Short summary of the changes.

Changes <or "Sumary">:
 - commit subject
 - commit subject
 - commit subject

Pull Request: #<id>
```

The "Changes" section may be generated from:

```sh
git log --format=' - %s' <base>..<branch>
```

## Release Policy

This project uses Git tags as the single source of truth for versioning.

Version format follows Semantic Versioning:
vMAJOR.MINOR.PATCH

Examples:
- v0.2.0
- v0.2.1
- v1.0.0
