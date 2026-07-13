<p align="center">
  <img src="https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white" alt="C"/>
  <img src="https://img.shields.io/badge/Raylib-000000?style=for-the-badge&logo=raylib&logoColor=white" alt="Raylib"/>
</p>

# ⚛️ Engine de Física

**Uma Engine de Física 3D em C puro com foco em precisão numérica e comparação de integradores.**

> *"Não basta simular, é preciso conservar a energia."*

---

## Visão Geral

A **Engine de Física** é um simulador de corpos rígidos (`Rigid Body Dynamics`) desenvolvido em C como trabalho acadêmico. O grande diferencial do projeto é a implementação **dual** de integradores numéricos:

- **Método de Euler (Explícito)** – rápido, mas instável para passos longos.
- **Método de Runge-Kutta 4ª Ordem (RK4)** – mais custoso, porém extremamente preciso e estável.

O usuário pode alternar entre os dois métodos **em tempo real** (tecla `R` / `E`) e observar, através de um rastro visual (trail) e da métrica de energia cinética, a degradação da simulação quando o Euler é utilizado.

---

## Principais Funcionalidades

- ✅ **Corpos Rígidos 3D** com posição, velocidade, orientação (Quatérnios) e Tensor de Inércia.
- ✅ **Loop de Física com Passo Fixo** (*Fixed Timestep*) – a física roda a 60 Hz independente do FPS.
- ✅ **Seletor de Integradores (Euler vs RK4)** com chaveamento via teclado.
- ✅ **Colisões Simples** (Esfera-Plano e Esfera-Esfera) com correção de posição (*Slop*).
- ✅ **Visualização 3D Interativa** com câmera orbital (Raylib).
- ✅ **Rastro Dinâmico (Trail)** que evidencia a estabilidade do RK4.
- ✅ **Métricas em Tempo Real** (Energia Cinética, Método ativo, FPS).

---

## Compilação e Execução
### Pré-requisitos
- GCC ou Clang

- Raylib (instalado globalmente ou via package manager)

Linux (apt):

```bash
sudo apt install libraylib-dev
```
Windows (vcpkg ou manual): Baixe o Raylib e coloque na pasta lib/.

### Compilação Manual (GCC)
```bash
gcc -o phoenix src/*.c -Iinclude -lraylib -lm -lGL -lpthread -ldl -lrt -lX11
```

## Controles

 | Tecla | Ação |
 | ----- | ---- |
 | ESC | Fecha a janela |

##  Cenários de Demonstração
O projeto possui 2 cenários pré-configurados no main.c:

1. Órbita Gravitacional – Uma partícula em campo central F = -G/r². Mostra a diferença brutal de estabilidade.

2. Queda com Empilhamento – Três esferas caindo sobre um plano, com colisões e atrito simplificado.

## Divisão de Tarefas (Equipe de 6 Pessoas)

| Módulo | Responsáveis | Entregáveis |
|-- | -- | -- |
| Fundação Matemática | Dupla A | operações vetoriais, normalização, produto, quatérnios e matrizes de rotação. |
| Dinâmica e Estado | Dupla B | struct do corpo, acúmulo de forças/torques e cálculo do tensor de inércia. |
| Integradores (Core) | Dupla B (líder) |  implementação do Euler e RK4 com função de derivada unificada. |
| Detecção e Resposta | Dupla C | detecção Esfera-Plano/Esfera-Esfera e aplicação de impulso linear/angular. |
| Renderização e Loop | Dupla D | janela Raylib, câmera 3D, desenho dos objetos, rastro (trail) e chaveamento de teclas. |
| Testes e Documentação | Todos (Rotativo) | Validação da conservação de energia, integração contínua e elaboração do README. |

## Sobre os Commits

Para manter o repositório organizado e o histórico limpo, utilizamos um padrão simples de commits baseado em **tipos**. Cada mensagem de commit deve começar com uma palavra-chave em maiúsculas, seguida de dois-pontos e uma breve descrição no imperativo.

### Tipos Permitidos

| Tipo | Quando usar |
| :--- | :--- |
| **ADD** | Nova funcionalidade, arquivo ou função implementada. |
| **UPDATE** | Melhoria ou aprimoramento de uma funcionalidade existente (sem quebrar compatibilidade). |
| **FIX** | Correção de bug, erro numérico ou vazamento de memória. |
| **REMOVE** | Remoção de código obsoleto, comentários ou arquivos mortos. |
| **REFACTOR** | Reestruturação do código sem alterar o comportamento externo (ex: renomear variáveis, dividir funções). |

### Exemplos

```bash
ADD: função para calcular atrito

UPDATE: melhorar resposta de colisão com correção posicional (slop)

FIX: resolver divisão por zero na colisão esfera-plano

REMOVE: remover declarações de debug desnecessárias

REFACTOR: separar lógica de renderização do loop principal de física
```

## Arquitetura do Projeto

```
physics-engine/
├── README.md
├── ...
├── include/
│   ├── ...
└── src/
    ├── ...
```

## 🧠 Arquitetura do Sistema

Arquitetura geral do sistema

```mermaid
flowchart TD
    A[Main Loop - Fixed Timestep] --> B[Apply Forces and Torques]
    B --> C{Select Integrator}
    C -->|Press E| D[Euler Integration]
    C -->|Press R| E[RK4 Integration]
    D --> F[Collision Resolution]
    E --> F
    F --> G[Update Trail History]
    G --> H[Render 3D Scene]
    H --> I[Draw Metrics and Controls]
```

##  Agradecimentos
