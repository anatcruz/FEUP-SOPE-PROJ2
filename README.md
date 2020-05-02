# Trabalho Prático nº 2 de Sistemas Operativos
## Acesso informático aos quartos de banho - Entrega 1
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

Nota: não era necessário implementar as opções nplaces e nthreads na primeira parte (assume-se que são ilimitadas)

### Features
- Todas as funcionalidades estão implementadas de acordo com o enunciado e a nossa interpretação
- Interpretámos a parte do fecho da casa de banho da seguinte maneira: após o ciclo while que processa os pedidos dos clientes e antes do fecho do FIFO público, colocamos uma flag closed a true  e verificamos se ainda há algum pedido em buffer para fazer a leitura, pedido este que irá ser respondido com 2LATE da parte do server. No entanto, esta situação é rara devido ao intervalo de criação dos pedidos pela parte dos clientes e à velocidade de execução dos programas.


### Notas adicionais
- Para compilar usa-se o comando que cria os executáveis necessários a ambos os programas:
```console
make
```
- Para executar corre-se os dois comandos referidos anteriormente em terminais separados

