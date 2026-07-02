# Testing

ArKanjo currently provides an end-to-end (E2E) test suite that verifies the generated output against the expected results.

To run the tests, execute:

```bash
build/arkanjo-test
```

Most output files are compared byte-for-byte. The only exception is output_parsed.txt, whose last column contains a floating-point similarity value. Since floating-point computations may differ slightly across platforms, the tests compare all other fields exactly and allow a small epsilon for the similarity metric.

The E2E tests are also executed in the CI pipeline to help detect regressions automatically.
