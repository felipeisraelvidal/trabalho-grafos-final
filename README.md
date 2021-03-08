# Trabalho Grafos



### Árvore Geradora Mínima Generalizada

Para executar esse repositório, abra seu terminal na pasta raíz do projeto e execute os seguintes comandos:

```
g++ *.c* -o NOME_DO_EXECUTAVEL
```

Esse comando irá compilar o código e criar um arquivo executável do programa.

A seguir execute o comando a seguir para iniciar a execução:

```
./NOME_DO_EXECUTAVEL CAMINHO_ARQUIVO_ENTRADA CAMINHO_ARQUIVO_SAIDA E_DIRECIONADO E_PODERADO_ARESTAS E_PONDERADO VERTICES
```

Os argumentos que precisam ser passados são:

* Nome do arquivo executável do programa
* O caminho do arquivo contendo a instância do problema
* O caminho do arquivo onde será impresso o resultado das funções
* Se o grafo é direcionado
  * 0 para não direcionado
  * 1 para grafo direcionado
* Se o grafo é ponderado nas arestas
  * 0 para não ponderado
  * 1 para ponderado
* Se o grafo é ponderado nos vértices
  - 0 para não ponderado
  - 1 para ponderado



## Executando o programa...

Quando o programa for executado, aparecerá um menu para a seleção das funções implementadas, como mostra abaixo:

> MENU
> -------------------------------------------------
> 
> [1] Subgrafo induzido por conjunto de vértices<br/>
> [2] Caminho Mínimo entre dois vértices - Dijkstra<br/>
> [3] Caminho Mínimo entre dois vértices - Floyd<br/>
> [4] Árvore Geradora Mínima de Kruskal<br/>
> [5] Árvore Geradora Mínima de Prim<br/>
> [6] Imprimir caminhamento em largura<br/>
> [7] Imprimir ordenacao topológica<br/>
> [8] Algoritmo Guloso<br/>
> [9] Algoritmo Guloso Randomizado<br/>
> [0] Sair<br/>

Após executar as funções desejadas, é preciso sair do do programa (comando 0) para salvar os resultados no arquivo de saída passado via parâmetro.



## Algoritmos Gulosos

Para os dois algoritmos, guloso e guloso randomizado, o programa irá criar dois arquivos direferentes do arquivo de saída passado no argumento contendo todos os testes realizados. Esse arquivo será guardado na pastra *tests* da raiz do projeto.