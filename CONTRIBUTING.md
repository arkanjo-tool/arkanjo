# Contributing

## Pull Requests

* Keep pull requests focused on a single change whenever possible.
* Ensure the project builds successfully.
* Update documentation when necessary.
* Follow the commit convention described below.
* Ensure a clean commit history before merging (use rebase or squash when appropriate).

## Commit Convention

This project follows a convention based on Conventional Commits.

### Format

```text
<type>: <description>
<type>(<scope>): <description>
<type>(<scope>/<subscope>): <description>
<type>(<scope>/<subscope>): <description> (#<issue>)
```

The commit subject should be a brief, one-line description and should not exceed 72 characters.

The commit body can be used to provide additional context, rationale, implementation details, or other relevant information. Keep each line of the body within 72 characters when possible.

Issue references are optional and should be appended at the end of the subject.

### Developer Certificate of Origin (DCO)

All commits must include a `Signed-off-by` line certifying that the contributor agrees to the Developer Certificate of Origin.

Create commits with the sign-off option:

```bash
git commit -s
```

### Examples

```text
docs: update README

Signed-off-by: John Doe <john@example.com>
```

```text
fix: correct broken link (#123)

Signed-off-by: John Doe <john@example.com>
```

```text
feat(benchmark): add benchmark command

Signed-off-by: John Doe <john@example.com>
```

```text
fix(parser): handle invalid input (#456)

The parser was failing when receiving an empty input file.
This adds validation before processing the input.

Signed-off-by: John Doe <john@example.com>
```

### Common Types

* `feat` - a new feature
* `fix` - a bug fix
* `docs` - documentation changes
* `chore` - maintenance tasks, dependencies, or infrastructure changes
* `refactor` - code changes that neither fix a bug nor add a feature
* `test` - adding or updating tests
