# Um simples interpretador em C

Esse interpretador simples foi desenvolvido na universidade Unifenas pelos alunos [Luigi Erucci](https://github.com/ezboy4life), [Mateus Battaglia Hirata](https://github.com/WarmMateTea), [Jhônatas Tomaz da Silva](https://github.com/Jhonatas48) e Marcello do 6º período de Ciência da Computação. Caso você seja um destes colaboradores por gentileza veja as [diretrizes de colaboração](CONTRIBUTING.md).

## Capacidades do Interpretador

- 3 Tipos de comandos de programação. ✅
- 26 variáveis. ✅
- Comandos de entrada e saída (input/output). ✅
- Aceita espaços em brancos entre os comandos ✅
- Aceita linhas em branco no código ✅
- Executa o código fonte existente em um arquivo de texto separado ✅ (@'test.txt')
- O interpretador é executado em prompt: `C:/Compiladores> interpreter test.txt` ✅

## Compilação

Para gerar o arquivo executável você pode usar qualquer compilador de C/C++, para gerar o arquivo `.exe` que está disponível foi utilizado o compilador GCC, porém outros compiladores podem (provavelmente) compilar o projeto normalmente.

## Utilização

Para utilizar o interpretador basta executar o arquivo `interpreter.exe` e digitar seu código, caso você deseje executar ele via CMD basta abrir o diretório em que ele se localiza e digitar `interpreter.exe <argumento>`. Atualmente é possível passar apenas um argumento para o interpretador via Shell/CMD que seria um caminho referente a um arquivo de código fonte da linguagem cujo interpretador entende.

Caso você deseje colocar o interpretador para ser executado globalmente em seu sistema Windows você precisa:

- Localize o diretório onde seu arquivo `interpreter.exe` está, por exemplo: `C:/Users/PC/Interpretador`.
- No Menu Iniciar, procure por "Editar Variáveis de Ambiente do Sistema".
- Clique em "Variáveis de Ambiente".
- Na parte inferior "Variáveis do Sistema", procure por `PATH` (ou `Path`), selecione e clique em "Editar".
- Clique em "Novo" e adicione o diretório onde seu executável está, no nosso caso `C:/Users/PC/Interpretador`.

Tudo pronto! Após isso você já poderá utilizar seu interpretador em qualquer diretório de seu sistema, basta abrir um CMD e digitar `interpreter` seguido (ou não) de seu argumento! Vale ressaltar que caso um CMD estiver aberto durante o processo de inclusão do diretório na variável PATH você precisará reiniciá-lo para que ele funcione.