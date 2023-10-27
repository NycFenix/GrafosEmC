#Grafico estudo de caso 1
import matplotlib.pyplot as plt
grafos = ['Grafo 1', 'Grafo 2', 'Grafo 3', 'Grafo 4', 'Grafo 5']
tempos_execucao = [0.2, 0.6, 3.5, 40, 224.7]

plt.plot(grafos, tempos_execucao, marker='o', linestyle='-')
plt.xlabel('Grafo')
plt.ylabel('Tempo de Execução (segundos)')
plt.title('Cálculo da Distancia com HEAP')
plt.xticks(rotation=45) 


plt.tight_layout()
plt.show()


#Grafico estudo de caso 2
import matplotlib.pyplot as plt

grafos = ['Grafo 1', 'Grafo 2', 'Grafo 3', 'Grafo 4', 'Grafo 5']

tempos_totais = [112, 473, 3153, 34984, 188689]

medias_amostrais = [1.12, 4.73, 31.53, 349.84, 1886.89]

plt.title('Cálculo de Distância para Múltiplos Vértices de Origem - com HEAP')
plt.bar(grafos, tempos_totais, color='blue', label='Tempo Total (s)')
plt.xlabel('Grafo')
plt.ylabel('Tempo de Execução Total (segundos)')
plt.yscale('log')  
plt.xticks(rotation=45) 


plt.plot(grafos, medias_amostrais, marker='o', linestyle='-', color='red', label='Média Amostral (s/amostra)')


plt.legend()


plt.tight_layout()
plt.show()

