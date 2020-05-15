# Trabalho Prático nº 2 de Sistemas Operativos
## Acesso informático aos quartos de banho - Entrega 2
### Modo de utilização:
```console
Un <-t nsecs> fifoname
```
```console
Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname
```
### Opções de invocação:
Flags | Descrição
----- | -----------
-t nsecs | número (aproximado) de segundos que o programa deve funcionar
fifoname | nome do canal público (FIFO) para comunicação com o servidor
-l nplaces | lotação do Quarto de Banho
-n nthreads | número (máximo) de threads a atender pedidos


### Features
- Todas as funcionalidades estão implementadas de acordo com o enunciado e a nossa interpretação.
- Interpretámos a parte do fecho da casa de banho da seguinte maneira: após o ciclo while que processa os pedidos dos clientes, durante o numero de segundos que a casa de banho deve estar aberta, colocamos uma flag _closed_ a true que muda a resposta do servidor na thread function, e executamos outro ciclo while enquanto conseguimos ler pedidos já enviados para o fifo público (o qual fazemos unlink antes de executar este último ciclo para impedir que os clientes possam fazer mais pedidos). Assim caso haja pedidos à espera de resposta estes receberam CLOSD.
- Para tratar da atribuição dos places, quando estes estão limitados, implementamos uma queue simples (baseada nesta implementação: https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation) que contém os lugares disponíveis. Quando é necessário usar um lugar este é retirado do início da queue, e quando um lugar volta a ficar disponível é introduzido na final da fila.


### Notas adicionais
- Para compilar usa-se o comando, na root do projeto, que cria os executáveis necessários a ambos os programas:
```console
make
```
- Para executar corre-se, tamém na root do projeto, os dois comandos referidos anteriormente em terminais separados.

