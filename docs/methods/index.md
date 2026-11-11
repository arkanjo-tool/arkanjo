# Methods

The duplication detection process supports the following methods, implemented
through the \ref IMethod interface:

| Class | More Details |
|-|-|
| \ref TfidfMethod | \subpage tfidf |
| \ref DiffMethod | -- |
| \ref ASTMethod | -- |
| \ref EmbeddingMethod | \subpage embedding |

The available methods are defined by the \ref PreprocessorBuild::MethodsType enumeration:

\snippet src/commands/pre/build/preprocessor_build.hpp MethodsType
