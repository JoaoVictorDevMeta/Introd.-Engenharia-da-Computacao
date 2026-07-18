# Tarefas — Status Atual do Projeto

> Legenda: ✅ Feito · 🟡 Parcial/com problemas · ❌ Não iniciado

---

## Dupla A — Fundação Matemática

| Item | Status | Observação |
| -- | -- | -- |
| `vec3_add`, `vec3_sub`, `vec3_scale` | ✅ | `src/vector3d.c` |
| `vec3_dot`, `vec3_product` (cruz) | ✅ | |
| `vec3_length`, `vec3_normalize`, `vec3_distance` | ✅ | |
| `vec3_torque` | ✅ | |
| Quatérnios (`quat_add`, `quat_scale`, `quat_mul`, `quat_conjugate`, `quat_normalize`) | ✅ | `src/quaterniom.c` |
| `quat_from_omega`, `quat_from_axis_angle` | ✅ | |
| Mat3 (`identity`, `rotation_x/y/z`, `mul`, `mul_vec3`, `transpose`, `inverse`, `from_quat`) | ✅ | `src/matriz3x3.c` |
| Testes com asserts (entregável original pedia biblioteca "100% testada com asserts") | ✅ | `tests/testMath.c` |


## Dupla B — Dinâmica e Integradores (o diferencial do projeto) ✅

| Item | Status | Observação |
| -- | -- | -- |
| `struct State` / `struct Derivative` | ✅ | `include/integration.h` |
| `struct RigidBody` (`corposmove.c/h`) | ✅ | Posição, velocidades, força/torque, `BoxShape` com massa e tensor de inércia. `PrintRigidBodies`/`InitializeRigidBodies`/`ComputeForceAndTorqueGravidade`/`simulaRigidoGravidade` agora recebem `numBodies` e operam sobre o array inteiro (testado com N=3) |
| Acúmulo de forças/torques | ✅ | `ComputeForceAndTorqueGravidade` calcula força peso e torque para todos os N corpos do array |
| Cálculo do tensor de inércia | ✅ | `CalculateBoxInertia` corrigido: recebe `BoxShape*` por ponteiro e usa `(1.0/12.0)` (double). Validado à mão: caixa 2x3x4, massa 10 → inércia (20.83, 16.67, 10.83), bate com a fórmula |
| `DerivativeFunction` (assinatura unificada) | ✅ | Tipo definido em `integration.h`. Duas implementações concretas em `integration.c`: `derivGravidade` (queda livre) e `derivMola` (oscilador harmônico, Lei de Hooke) |
| Integrador de Euler | ✅ | `euler()` em `src/integration.c`, funcional |
| Integrador RK4 | ✅ | `rk4()` implementado (4 estágios, pesos 1/6-1/3-1/3-1/6) em `src/integration.c`. Validado contra a solução analítica de queda livre: RK4 bate exatamente (`pos.z = -4.905`), Euler mostra o erro de 1ª ordem esperado (`pos.z = -4.4145`) — a comparação Euler×RK4 funciona |
| Simulação alternativa de partículas (`particulasmove.c/h`) | ✅ | Gravidade, mola (Hooke) e lei da gravitação (`ComputeForcePlanetario2`) implementadas. Typo `vec3_lenght` corrigido para `vec3_length`. `RunSimulationGravity2` implementada (órbita de 2 corpos, ex. Terra-Lua, com reação de Newton) |
| Simulação de corpo rígido sob gravidade (`simulaRigidoGravidade`) | ✅ | Loop de Euler manual, chama `ComputeForceAndTorqueGravidade` corretamente, generalizado para N corpos |

**Decisão de arquitetura (item que estava pendente):** `State`/`Derivative`/`DerivativeFunction` viraram o núcleo genérico dos integradores (`euler`/`rk4`), usado para movimento tipo partícula/ponto-massa via funções de derivada (`derivGravidade`, `derivMola`) — é o que atende o Cenário 1 (Órbita Gravitacional) do README. `RigidBody` continua uma struct especializada (com massa, `BoxShape` e tensor de inércia) com seu próprio loop de Euler em `simulaRigidoGravidade`, porque a assinatura fixa de `DerivativeFunction` (`const State*, Derivative*`, sem parâmetro extra) não tem onde guardar massa/inércia por corpo — não dá para plugar `RigidBody` em `euler`/`rk4` sem redesenhar essa assinatura. Isso atende o Cenário 2 (Queda com Empilhamento) do README.

**Bugs extras corrigidos (não listados originalmente):**
- `PrintRigidBodies` imprimia sempre `rigidBody[0]` dentro do loop (em vez de `rigidBody[i]`) e passava um `Vec3` inteiro para `%.2lf` (comportamento indefinido) — corrigido para imprimir `angle.x/.y/.z` do corpo `i`.
- `corposmove.c`/`particulasmove.c` chamavam `rand()`/`srand()`/`time()` sem incluir `<stdlib.h>`/`<time.h>` — adicionados.

**Verificação:** todos os arquivos de A+B compilam limpos com `-Wall -Wextra` e sem warnings; testado em runtime (queda livre Euler vs RK4 vs solução analítica, mola com RK4 por 1000 passos, inércia de caixa, `RigidBody` com N=3).

---

## Dupla C — Detecção e Resposta (Colisões)

| Item | Status |
| -- | -- |
| Detecção Esfera-Plano | ❌ |
| Detecção Esfera-Esfera | 🟡 |
| Geração de dados de contato (ponto, normal, penetração) | 🟡 |
| Resposta de impulso linear/angular | 🟡 |
| Correção posicional (slop) | 🟡 |

**Restante:** Módulo inteiro ainda precisa ser válidado por meio de testes, após isso é posível marcar como concluído. Detecção esfera-plano ainda precisa ser feito (simples). Resposta de impulso angular tambem ainda precisa ser implementada.

---

## Dupla D — Renderização, Cenários e Loop Principal

| Item | Status | Observação |
| -- | -- | -- |
| Janela Raylib + câmera 3D | 🟡 | `src/main.c` abre janela e câmera, mas é rotação por setas do teclado, não câmera orbital |
| Loop de física com passo fixo (60 Hz) | ❌ | Loop atual roda física direto no framerate do Raylib, sem *fixed timestep* |
| Uso do `State`/`RigidBody` do projeto | ❌ | `main.c` usa `Vector3` nativo do Raylib com física ad-hoc (bola quicando), não usa `Vec3`, `Quat`, `State` nem `euler`/`rk4` do projeto — é um protótipo isolado de teste do Raylib |
| Seletor de integrador (teclas E/R) | ❌ | Não existe no código atual |
| Rastro dinâmico (trail) | ❌ | |
| Métricas em tela (Energia Cinética, método ativo, FPS) | 🟡 | Só mostra posição/velocidade da bola, sem energia cinética nem método ativo |
| Cenário 1 — Órbita Gravitacional | ❌ | |
| Cenário 2 — Queda com empilhamento (3 esferas + colisão) | ❌ | |

**Restante:** este é o módulo com maior distância entre o README e o código — `main.c` precisa ser reescrito para efetivamente instanciar `RigidBody`s e chamar `euler`/`rk4` do módulo de integração, e depende de C (colisões) e B (RK4 + dinâmica) estarem prontos antes de fazer sentido plugar os dois cenários.

---

## Testes e Documentação (Todos — Rotativo)

| Item | Status |
| -- | -- |
| README principal com arquitetura, controles, compilação | ✅ | |
| Validação de conservação de energia (Euler vs RK4) | ❌ | Depende de RK4 e da dinâmica estarem prontos |
| Suite de testes automatizados | ❌ | Só `test.c` manual, sem asserts |

---

## Resumo de bloqueios

A ordem de dependência real hoje é:
1. **Dupla B** ✅ concluída — `euler`/`rk4` funcionais e validados contra a solução analítica, `RigidBody` generalizado para N corpos, `CalculateBoxInertia` correto, `DerivativeFunction`s concretas (`derivGravidade`, `derivMola`) implementadas. Já existe o que comparar (objetivo central do projeto) e o que o `main.c` pode integrar.
2. **Dupla C** pode avançar em paralelo (detecção/resposta de colisão não depende de B).
3. **Dupla D** está desbloqueada em relação a B; falta apenas C (colisões) para ambos os cenários do README funcionarem ponta a ponta. O `main.c` atual ainda é só uma prova de conceito do Raylib e precisa ser reescrito para usar `State`/`RigidBody`/`euler`/`rk4`.
