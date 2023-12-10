## Soluções para Problemas Difíceis : Traveling Salespeson Problem

O trabalho foi desenvolvido utilizando a linguagem de programação C++, portanto um arquivo `Makefile` acompanha o repositório, e pode ser utilizado (através do comando `make`) para compilar o programa.
O programa estará disponível como um arquivo executável de nome `tp2`.

Todas as instâncias estão disponíveis na pasta `data` e são acessadas pelo programa, basta passar o nome do dataset para executar o programa com o dataset especificado, por exemplo:
```sh
./tp2 berlin52
```
```sh
./tp2 eil51
```

Alternativamente, foi criado um arquivo do tipo `bash` nomeado `run_datasets.sh` que irá executar o código com todas as instâncias disponíveis de forma sequencial, salvando os resultados em um arquivo `run_output.txt`. Isso pode ser feito através da seguinte sequência de comandos:
```bash
chmod +x run_datasets.sh
./run_datasets.sh
```

Ademais, estão disponíveis os documentos `relatorio.pdf` que é um documento contendo toda a argumentação, experimentação e análise de resultados do trabalho. E também, como pedido, o arquivo `tests_output.xlsx` que é a tabela completa com todos os resultados.