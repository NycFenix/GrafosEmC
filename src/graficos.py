#grafico estudo de caso 1
import matplotlib.pyplot as plt

# Nomes dos grafos
grafos = ['Grafo 1', 'Grafo 2', 'Grafo 3', 'Grafo 4', 'Grafo 5']

# Tempos de execução correspondentes em segundos
tempos_execucao = [0.2, 0.6, 3.5, 40, 224.7]

# Crie um gráfico de linha
plt.plot(grafos, tempos_execucao, marker='o', linestyle='-')
plt.xlabel('Grafo')
plt.ylabel('Tempo de Execução (segundos)')
plt.title('Cálculo da Distancia com HEAP')
plt.xticks(rotation=45)  # Rotação dos rótulos do eixo x para melhor leitura

# Exiba o gráfico
plt.tight_layout()
plt.show()

# grafico estudo de caso 2

import matplotlib.pyplot as plt

# Nomes dos grafos
grafos = ['Grafo 1', 'Grafo 2', 'Grafo 3', 'Grafo 4', 'Grafo 5']

# Tempos totais de execução em segundos
tempos_totais = [112, 473, 3153, 34984, 188689]

# Médias amostrais em segundos
medias_amostrais = [1.12, 4.73, 31.53, 349.84, 1886.89]

# Crie um gráfico de barras para os tempos totais
plt.title('Cálculo de Distância para Múltiplos Vértices de Origem - com HEAP')
plt.bar(grafos, tempos_totais, color='blue', label='Tempo Total (s)')
plt.xlabel('Grafo')
plt.ylabel('Tempo de Execução Total (segundos)')
plt.yscale('log')  # Escala logarítmica para lidar com valores grandes
plt.xticks(rotation=45)  # Rotação dos rótulos do eixo x para melhor leitura

# Adicione uma segunda série de dados para as médias amostrais
plt.plot(grafos, medias_amostrais, marker='o', linestyle='-', color='red', label='Média Amostral (s/amostra)')

# Adicione uma legenda
plt.legend()

# Exiba o gráfico
plt.tight_layout()
plt.show()

