#!/bin/bash

# Verifique se o nome do arquivo.py foi fornecido
if [ "$#" -ne 2 ]; then
    echo "Uso: $0 <arquivo.py> <número de execuções>"
    exit 1
fi

arquivo_py="$1"
n_execucoes="$2"

# Verifique se o arquivo Python existe
if [ ! -f "$arquivo_py" ]; then
    echo "Arquivo $arquivo_py não encontrado!"
    exit 1
fi

# Verifique se o número de execuções é um número válido
if ! [[ "$n_execucoes" =~ ^[0-9]+$ ]]; then
    echo "Número de execuções deve ser um número inteiro positivo!"
    exit 1
fi

# Array para armazenar os tempos de execução
tempos=()

# Executa o arquivo Python n vezes e armazena os tempos de execução
for ((i = 0; i < n_execucoes; i++)); do
    start_time=$(date +%s%N)
    python "$arquivo_py" > /dev/null 2>&1
    end_time=$(date +%s%N)
    
    # Calcula o tempo de execução em segundos
    tempo_execucao=$(echo "scale=9; ($end_time - $start_time) / 1000000000" | bc)
    tempos+=("$tempo_execucao")
done

# Função para calcular a média
media() {
    local total=0
    local count=${#tempos[@]}
    
    for tempo in "${tempos[@]}"; do
        total=$(echo "$total + $tempo" | bc)
    done
    
    echo "scale=9; $total / $count" | bc
}

# Função para calcular o desvio padrão
desvio_padrao() {
    local media=$(media)
    local total=0
    local count=${#tempos[@]}
    
    for tempo in "${tempos[@]}"; do
        dif=$(echo "$tempo - $media" | bc)
        total=$(echo "$total + ($dif * $dif)" | bc)
    done
    
    local variancia=$(echo "scale=9; $total / $count" | bc)
    echo "scale=9; sqrt($variancia)" | bc
}

# Calcula e exibe a média e o desvio padrão
echo "Média dos tempos de execução: $(media)"
echo "Desvio padrão dos tempos de execução: $(desvio_padrao)"