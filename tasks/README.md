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
<<<<<<< HEAD
<<<<<<< HEAD
| Testes com asserts (entregável original pedia biblioteca "100% testada com asserts") | ✅ | `tests/testMath.c` |

=======
| Testes com asserts (entregável original pedia biblioteca "100% testada com asserts") | ❌ | `src/test.c` só imprime um resultado manual do produto vetorial, sem `assert` e sem cobrir quaternion/mat3 |

**Restante:** escrever suite de testes com `assert.h` cobrindo vec3, quat e mat3 (principalmente `quat_normalize`, `mat3_inverse` e `mat3_from_quat`, que são as funções mais sensíveis a erro numérico).

---
>>>>>>> daf9399 (ADD:organização das tasks e status)
=======
| Testes com asserts (entregável original pedia biblioteca "100% testada com asserts") | ✅ | `tests/testMath.c` |

>>>>>>> 98739c9 (ADD: testes de matematica realizados com sucesso)

## Dupla B — Dinâmica e Integradores (o diferencial do projeto)

| Item | Status | Observação |
| -- | -- | -- |
| `struct State` / `struct Derivative` | ✅ | `include/integration.h` — mas não é a `RigidBody` prevista (sem massa, sem tensor de inércia, sem acumulador de força/torque) |
| `struct RigidBody` | ❌ | Não existe em nenhum arquivo do projeto |
| Acúmulo de forças/torques | ❌ | Não existe |
| Cálculo do tensor de inércia | ❌ | Não existe (Mat3 já tem `mat3_inverse`, que é a peça que falta para achar `ω = I⁻¹·L`, mas falta montar o tensor em si) |
| `DerivativeFunction` (assinatura unificada) | ✅ | Tipo definido em `integration.h`, mas **nenhuma implementação concreta existe** (nada converte força/torque acumulados em aceleração linear/angular) |
| Integrador de Euler | ✅ | `euler()` em `src/integration.c`, funcional |
| Integrador RK4 | ❌ | `rk4()` existe só como stub vazio (`void rk4(){}`) — **a comparação Euler×RK4, que é o coração do projeto, ainda não funciona** |
| Simulação alternativa de partículas (`particulasmove.c/h`) | 🟡 | Gravidade, mola e força planetária implementadas, mas: <br>• `vec3_lenght` (typo) em `particulasmove.c:40` — não compila <br>• `ComputeForceMolaHorizontal` no `.c` não bate com `ComputeForceMola` declarada no `.h` <br>• `RunSimulationGravidadeRK` é só protótipo, sem corpo |

**Restante (prioridade alta):**
1. Decidir se `particulasmove.c` é adaptado para o padrão `State`/`Derivative`/`RigidBody` ou descartado — hoje são duas abordagens paralelas e incompatíveis.
2. Criar `struct RigidBody` com massa, tensor de inércia, força/torque acumulados.
3. Implementar `rk4()`.
4. Implementar ao menos duas funções de derivada concretas (uma para órbita gravitacional, outra para queda com colisão), já que sem elas nenhum dos dois cenários do README roda.
5. Corrigir os bugs de compilação em `particulasmove.c`/`.h` se for mantido.

---

## Dupla C — Detecção e Resposta (Colisões)

| Item | Status |
| -- | -- |
| Detecção Esfera-Plano | ❌ |
| Detecção Esfera-Esfera | ❌ |
| Geração de dados de contato (ponto, normal, penetração) | ❌ |
| Resposta de impulso linear/angular | ❌ |
| Correção posicional (slop) | ❌ |

**Restante:** módulo inteiro não iniciado — nenhum arquivo `collision.*` existe no repositório.

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
1. **Dupla B** precisa terminar `RigidBody` + `rk4()` + funções de derivada — sem isso não há nada para comparar (o objetivo central do projeto) nem para o main.c integrar.
2. **Dupla C** pode avançar em paralelo (detecção/resposta de colisão não depende de B terminar).
3. **Dupla D** está bloqueada em ambas — o `main.c` atual é só uma prova de conceito do Raylib e precisa ser reescrito do zero para usar as structs reais do projeto.
