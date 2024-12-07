# Projeto e Análise de Algorítmos - Trabalho A2

*Desenvolvido por Guilherme Moreira Castilho, Paulo César Gomes Rodrigues, Pedro
Santos Tokar e Vitor Matheus do Nascimento Moreira.*

-------------------------------------------------------------------------------

## Sobre o repositório

Esse repositório é destinado à segunda avaliação da matéria de Projeto e Análise
de Algorítmos, lecionada no 4º período da graduação de Ciência de Dados e
Inteligência Artificial da FGV-EMAp. O trabalho consiste em modelar um sistema
que permita à cidade fictícia de Vargas estruturar seus meios de transporte.

São três grandes subtarefas que compõe o trabalho, sendo elas:

1 - Elaborar algorítmos e um sistema que seja capaz de auxiliar a cidade na
construção de um sistema de metrôs, indicando onde devem ser construídas estações
e como as ruas devem ser escavadas para minimizar os custos para a cidade;

2 - Elaborar algorítmos e um sistema que seja capaz de auxiliar a cidade na
elaboração de uma única linha de ônibus cíclica, que deve ter uma rota feita
priorizando a passagem por pontos turísticos;

3 - Elaborar algorítmos e um sistema que permita a uma pessoa ver quais meios de
transporte devem ser tomados para minimizar o tempo de deslocamento entre dois
pontos, além do quanto a pessoa irá gastar seguindo a rota sugerida.

Além de módulos para realizar as tarefas citadas, também existem módulos para
gerar grafos de teste (feitos em Python) e visualizá-los.

## Rodando o sistema

Nosso trabalho conta com o comando `make` para gerenciar as compilações. Antes de
usar qualquer outro comando, é importante gerar o grafo que será usado. Para isso,
basta usar, na raiz do repositório:

```bash
$ make graphgen
```

Para ver como é feita a execução da tarefa 1 (geração da linha de metrô), basta
usar o comando

```bash
$ make tarefa1
```

Para ver como é feita a execução da tarefa 2 (geração da linha de ônibus), o
comando usado deverá ser:

```bash
$ make tarefa2
```

É importante ter em mente que é necessário executar as tarefas 1 e 2 para que as
linhas sejam geradas e que seja possível visualizá-las.

Para executar a tarefa 3 (geração da rota de menor tempo), use:

```bash
$ make tarefa3
```

Para visualizar o grafo, com as linhas geradas, o comando usado deverá ser:

```bash
$ make graphread
```

Esse commando irá mostrar uma janela semelhante à essa:

![A screenshot showing the generated graph and lines](https://raw.githubusercontent.com/vitor-n/PAA-A2-Project/main/graphshow.png)

As cores das arestas dos grafos representam a qual região eleas pertencem. Arestas
pretas representam a linha de metrô gerada e arestas cinzas representam a rota
do ônibus gerada.

## Organização do repositório

- `data`: contém todas as informações relativas ao grafo gerado e sendo
operado.

- `extras`: contém o código python para geração de grafos aleatórios e para a
exibição deles.

- `include`: contém os arquivos de header (`.h`), que exportam as funções e classes
definidas no projeto.

- `scripts`: contém scripts shell usados no processo de compilação.

- `src`: contém o código fonte do projeto.

- `src/drivers`: contém diferentes funções `main` para diferentes tarefas.

- `src/structures`: contém código fonte relativo às classes usadas no projeto

- `src/trafficAPI`: contém código fonte relativo à API de transito usada para
determinar a velocidade máxima em determinados horários.
