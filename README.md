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

Para executar a tarefa 3 (geração da rota de menor tempo), use:

```bash
$ make tarefa3
```

Para visualizar o grafo, com as linhas geradas, o comando usado deverá ser:

```bash
$ make graphread
```
