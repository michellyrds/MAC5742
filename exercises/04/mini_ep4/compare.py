#!/usr/bin/env python3
import subprocess

def run_c_code(binary):
  """Executa o código C e retorna o valor lido."""
  process = subprocess.Popen([f"./{binary}"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
  stdout, stderr = process.communicate()

  # Extrai o valor lido da saída padrão
  for line in stdout.splitlines():
      if "Valor lido:" in line:
          return line.split(":")[1].strip() 
  return None  # Em caso de erro na execução

# Executar ambas as versões e coletar resultados
resultados_original = []
resultados_fixed = []
for _ in range(100):
  resultados_original.append(run_c_code("long-problem"))
  resultados_fixed.append(run_c_code("long-problem-fixed"))

# Contar ocorrências para cada versão
contagem_original_0x12345678 = resultados_original.count('0x12345678')
contagem_original_0x0 = resultados_original.count('0x0')
contagem_original_0x8765432112345678 = resultados_original.count('0x8765432112345678')

contagem_fixed_0x12345678 = resultados_fixed.count('0x12345678')
contagem_fixed_0x0 = resultados_fixed.count('0x0')
contagem_fixed_0x8765432112345678 = resultados_fixed.count('0x8765432112345678')

# Imprimir e comparar resultados
print("Resultados do código original (com problema de concorrência):")
print(f"Número de ocorrências de 0x12345678: {contagem_original_0x12345678}")
print(f"Número de ocorrências de 0x0: {contagem_original_0x0}")
print(f"Número de ocorrências de 0x8765432112345678: {contagem_original_0x8765432112345678}")

print("\nResultados do código corrigido (com mutex):")
print(f"Número de ocorrências de 0x12345678: {contagem_fixed_0x12345678}")
print(f"Número de ocorrências de 0x0: {contagem_fixed_0x0}")
print(f"Número de ocorrências de 0x8765432112345678: {contagem_fixed_0x8765432112345678}")
