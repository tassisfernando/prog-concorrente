import threading
import time
import random

# Definindo o buffer compartilhado e o tamanho máximo do buffer
buffer = []
buffer_size = 10

# Inicializando semáforos
empty = threading.Semaphore(buffer_size)  # Semáforo para espaços vazios
full = threading.Semaphore(0)             # Semáforo para itens no buffer
mutex = threading.Semaphore(1)            # Semáforo para exclusão mútua

def produtor(id):
    while True:
        item = random.randint(1, 100)  # Produz um item aleatório
        empty.acquire()                # Decrementa o semáforo empty
        mutex.acquire()                # Entra na seção crítica
        buffer.append(item)            # Adiciona o item ao buffer
        print(f'Produtor {id} produziu {item}. Buffer: {buffer}')
        mutex.release()                # Sai da seção crítica
        full.release()                 # Incrementa o semáforo full
        time.sleep(random.random())    # Simula tempo de produção

def consumidor(id):
    while True:
        full.acquire()                 # Decrementa o semáforo full
        mutex.acquire()                # Entra na seção crítica
        item = buffer.pop(0)           # Remove o item do buffer
        print(f'Consumidor {id} consumiu {item}. Buffer: {buffer}')
        mutex.release()                # Sai da seção crítica
        empty.release()                # Incrementa o semáforo empty
        time.sleep(random.random())    # Simula tempo de consumo

# Número de threads produtoras e consumidoras
num_produtores = 5
num_consumidores = 1

# Criando e iniciando as threads produtoras
produtores = []
for i in range(num_produtores):
    t = threading.Thread(target=produtor, args=(i,))
    produtores.append(t)
    t.start()

# Criando e iniciando as threads consumidoras
consumidores = []
for i in range(num_consumidores):
    t = threading.Thread(target=consumidor, args=(i,))
    consumidores.append(t)
    t.start()

# Aguardando a conclusão de todas as threads (nunca vai acontecer neste exemplo)
for t in produtores:
    t.join()

for t in consumidores:
    t.join()