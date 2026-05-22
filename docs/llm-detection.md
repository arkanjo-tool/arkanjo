# Método `llm-detection` (embeddings da Hugging Face)

Detector de duplicação de código a **nível de função** que mede similaridade
*semântica* entre corpos de função usando embeddings de um modelo da Hugging Face
(`jinaai/jina-embeddings-v2-base-code`). É o 4º método do Arkanjo, intercambiável
com os existentes (NLP/gensim, diff, AST): mesma entrada, mesma saída e
selecionável no mesmo menu do pré-processador.

## Como funciona

Igual aos demais métodos, ele implementa a interface `IMethod`
(`include/arkanjo/methods/method.hpp`):

1. **`on_function(fd)`** — para cada função extraída, grava o corpo da função em
   `~/.cache/arkanjo/default/features/source/<caminho>/<funcao><ext>`
   (mesmo padrão de `ToolMethod` e `DiffMethod`).
2. **`execute()`** — invoca o script Python via `popen`, lê o stdout, filtra pelo
   limiar de similaridade, ordena decrescente e grava o resultado.
3. **`save_duplications()`** — grava `~/.cache/arkanjo/default/output_parsed.txt`
   no formato legado (idêntico a Diff/AST):
   ```
   <quantidade_de_pares>
   <path1> <path2> <similaridade>
   ...
   ```

O trabalho pesado de embedding fica num script Python, invocado pelo C++:

```
python3 -W ignore <third_party_dir>/llm-detection/llm_detection.py \
    --source-dir <dir_dos_corpos_de_funcao> \
    --min-similarity <limiar>
```

**Contrato C++↔Python:** o script imprime no stdout uma linha por par, separada
por TAB — `path1\tpath2\tsimilaridade`. Os caminhos são **resource paths
relativos** ao `--source-dir` (ex.: `wacom_wac.c/minha_funcao.c`), no mesmo
formato do `ASTMethod` — o lado de consulta resolve esse caminho contra as raízes
`source/`/`info/`/`header/` do cache, então caminhos absolutos **não** resolvem.
Similaridade em 0-100.
Todo log/diagnóstico do script vai para stderr, mantendo o stdout limpo. O lado
C++ ignora linhas malformadas defensivamente.

## Arquivos

| Arquivo | Papel |
| --- | --- |
| `include/arkanjo/methods/llm/llm_method.hpp` | Declaração da classe `LLMMethod : public IMethod`. |
| `src/methods/llm/llm_method.cpp` | `on_function` (grava cache), `execute` (chama o script, parseia), `save_duplications`. |
| `third-party/llm-detection/llm_detection.py` | Script de embeddings (`sentence-transformers` + cosseno par-a-par). |
| `third-party/llm-detection/requirements.txt` | Dependências Python do script. |
| `src/commands/pre/build/preprocessor_build.hpp` | Registro do método no menu (`MethodsType`, 4ª entrada). |

O `src/methods/llm/llm_method.cpp` é compilado automaticamente pelo
`file(GLOB_RECURSE src/methods/*.cpp)` (`cmake/Targets.cmake`). A pasta
`third-party/llm-detection/` é copiada para o diretório de build pelo target
`copy_third_party` e instalada pela regra `install(DIRECTORY third-party/ ...)`.

### Trocar o modelo

O nome do modelo é uma constante no topo de
`third-party/llm-detection/llm_detection.py`:

```python
MODEL_NAME = "jinaai/jina-embeddings-v2-base-code"
```

Como o script usa `sentence-transformers` (API `.encode()` uniforme para qualquer
modelo de embedding do Hub), trocar de modelo é só editar essa única linha.

## Como testar

### 1. Build

Adicionar um arquivo novo ao GLOB do CMake exige **re-rodar o configure** (não
basta `cmake --build`):

```sh
cmake -S . -B build
cmake --build build -j"$(nproc)"
```

Esperado: compila e linka sem erros; `llm_method.cpp` entra em `core_methods`; o
script é copiado para `build/third-party/llm-detection/`.

### 2. Setup das dependências Python (uma vez)

As dependências **não** são instaladas automaticamente. Instale-as no `python3`
que o `popen` vai chamar (atenção a venvs):

```sh
python3 -m pip install -r third-party/llm-detection/requirements.txt
```

> A primeira execução baixa o modelo (~centenas de MB) para `~/.cache/huggingface`.
> Em máquina offline, é preciso pré-popular esse cache.

(Opcional) Testar o script isoladamente, sem o binário:

```sh
mkdir -p /tmp/funcs
printf 'int add(int a, int b){ return a + b; }\n'        > /tmp/funcs/add.c
printf 'int soma(int x, int y){ return x + y; }\n'       > /tmp/funcs/soma.c
printf 'void log_msg(const char* m){ puts(m); }\n'       > /tmp/funcs/log.c
python3 third-party/llm-detection/llm_detection.py \
    --source-dir /tmp/funcs --min-similarity 70
# stdout: linhas "path1<TAB>path2<TAB>similaridade"
```

### 3. Rodar pelo Arkanjo (end-to-end)

```sh
./build/arkanjo-preprocessor build /caminho/do/projeto
```

Nos prompts:

- **Enter your project path:** (já passado como argumento acima)
- **Enter minimum similarity desired:** ex. `70`
- **Enter the number of the duplication finder technique:** `4`
  (`Semantic similarity using Hugging Face embeddings (...)`)

### 4. Resultado esperado

- Corpos de função materializados em
  `~/.cache/arkanjo/default/features/source/...`
- Logs do script (carregamento do modelo, embedding) no stderr.
- `~/.cache/arkanjo/default/output_parsed.txt` gravado: 1ª linha = nº de pares;
  demais = `path1 path2 similaridade` (`%.2f`, resource paths relativos).

Consultar com os comandos existentes (consomem o mesmo `output_parsed.txt`):

```sh
./build/arkanjo explorer -l 10 -s
./build/arkanjo duplication
```

## Observações / limitações

- **O(n²)**: a comparação é par-a-par (script e C++), quadrática no nº de funções.
  O embedding é vetorizado (uma matmul), mas a matriz n×n e o nº de linhas emitidas
  são o teto de custo em bases grandes.
- **`python3` do shell**: as dependências precisam estar no interpretador que o
  `popen` invoca.
- **Download do modelo**: pesado na primeira execução.
