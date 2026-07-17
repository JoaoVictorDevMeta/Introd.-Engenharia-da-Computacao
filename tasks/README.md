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


## Dupla B — Dinâmica e Integradores (o diferencial do projeto)

| Item | Status | Observação |
| -- | -- | -- |
| `struct State` / `struct Derivative` | ✅ | `include/integration.h` |
| `struct RigidBody` (`corposmove.c/h`) | 🟡 | Existe (posição, velocidades, força/torque, `BoxShape` com massa e tensor de inércia), mas é uma abordagem paralela ao `State`/`Derivative` — não usa `Vec3`/`Quat` do jeito unificado e `PrintRigidBodies`/`InitializeRigidBodies`/`simulaRigidoGravidade` só operam sobre o índice `[0]` (não sobre um array de N corpos) |
| Acúmulo de forças/torques | 🟡 | `ComputeForceAndTorqueGravidade` calcula força peso e torque para 1 corpo; não há acumulador genérico (só o caso gravidade está implementado) |
| Cálculo do tensor de inércia | 🟡 | `CalculateBoxInertia` existe mas tem dois bugs: (1) recebe `BoxShape` por valor, então o `momentOfInertia` calculado nunca é salvo de volta na struct do chamador; (2) `(1/12)` é divisão inteira em C e sempre dá `0`, zerando o resultado |
| `DerivativeFunction` (assinatura unificada) | ✅ | Tipo definido em `integration.h`, mas **nenhuma implementação concreta existe** — `RigidBody` não é integrado via `euler`/`rk4`, tem seu próprio loop manual em `simulaRigidoGravidade` |
| Integrador de Euler | ✅ | `euler()` em `src/integration.c`, funcional (compila e roda sobre `State`/`Derivative`) |
| Integrador RK4 | ❌ | `rk4()` no `.c` não bate com o protótipo do `.h` (`void rk4(){}` vs `void rk4(State*, double, DerivativeFunction)`) — **não compila**, e a comparação Euler×RK4 continua não funcionando |
| Simulação alternativa de partículas (`particulasmove.c/h`) | 🟡 | Gravidade, mola (Hooke) e lei da gravitação implementadas e com assinaturas `.c`/`.h` batendo agora, mas: <br>• `vec3_lenght` (typo, `particulasmove.c:43`) não existe — `vec3_length` é o nome real em `vector3d.h`, então **falha no link** <br>• `RunSimulationGravity2` está declarada no `.h` mas o corpo inteiro está comentado no `.c` (nem stub existe) |
| Simulação de corpo rígido sob gravidade (`simulaRigidoGravidade`) | 🟡 | Loop de Euler manual escrito, mas chama `ComputeForceAndTorque(rigidBody)` — função que não existe; a implementada se chama `ComputeForceAndTorqueGravidade` — **falha no link** |

**Restante (prioridade alta):**
1. Decidir se `particulasmove.c`/`corposmove.c` são unificados sob o padrão `State`/`Derivative`/`DerivativeFunction` ou mantidos como simulações standalone — hoje são três abordagens paralelas e incompatíveis (a de `integration.c`, a de `particulasmove.c` e a de `corposmove.c`).
2. Corrigir o typo `vec3_lenght` → `vec3_length` em `particulasmove.c:43`.
3. Corrigir a chamada `ComputeForceAndTorque` → `ComputeForceAndTorqueGravidade` em `corposmove.c:64`.
4. Corrigir `CalculateBoxInertia`: receber `BoxShape*` por ponteiro e trocar `(1/12)` por `(1.0/12.0)`.
5. Implementar `rk4()` com a assinatura correta do header.
6. Generalizar `RigidBody`/`particulasmove` para arrays de N corpos (hoje ambos operam só sobre o índice `[0]`/1 partícula nos casos não-Euler-simples).
7. Implementar ao menos duas funções de derivada concretas (`DerivativeFunction`) usando `RigidBody`, já que sem elas nenhum dos dois cenários do README roda de forma unificada.

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
1. **Dupla B** tem `RigidBody` rascunhado, mas ainda precisa: corrigir os 3 bugs de link/compilação (`vec3_lenght`, `ComputeForceAndTorque`, `rk4()` com assinatura errada), consertar `CalculateBoxInertia`, unificar `RigidBody` com `State`/`Derivative`/`DerivativeFunction` e generalizar para N corpos — sem isso não há nada para comparar (o objetivo central do projeto) nem para o main.c integrar.
2. **Dupla C** pode avançar em paralelo (detecção/resposta de colisão não depende de B terminar).
3. **Dupla D** está bloqueada em ambas — o `main.c` atual é só uma prova de conceito do Raylib e precisa ser reescrito do zero para usar as structs reais do projeto.
