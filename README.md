# Programação e Algoritmos 2022 2023 - [IADE - UE](https://www.iade.europeia.pt/) <!-- omit in toc -->

## Projeto <!-- omit in toc -->

- [Datas Relevantes](#datas-relevantes)
- [Descrição](#descrição)
- [Instruções](#instruções)
  - [Registar utilizador (RJ)](#registar-utilizador-rj)
  - [Remover utilizador (EJ)](#remover-utilizador-ej)
  - [Listar utilizadores (LJ)](#listar-utilizadores-lj)
  - [Registar espaço de simulação (RE)](#registar-espaço-de-simulação-re)
  - [Remover espaço de simulação (EE)](#remover-espaço-de-simulação-ee)
  - [Registar partícula (RP)](#registar-partícula-rp)
  - [Alterar partícula (AP)](#alterar-partícula-ap)
  - [Simular (S)](#simular-s)
- [Estrutura do projeto](#estrutura-do-projeto)
- [Testes de *input*/*output*](#testes-de-inputoutput)
  - [Utilização de testes](#utilização-de-testes)
- [Entrega](#entrega)
- [Prova de Autoria](#prova-de-autoria)
- [Tecnologias](#tecnologias)
- [Grupo de trabalho](#grupo-de-trabalho)
- [Avaliação](#avaliação)

## Datas Relevantes

| Data                      | Evento                         |
| ------------------------- | ------------------------------ |
| 21/03/2023                | Disponibilização do enunciado. |
| 29/05/2023 23:59:59       | Entrega final do trabalho.     |
| 31/05/2023 --- 02/06/2023 | Provas de autoria.             |

## Descrição

Pretende-se a implementação de uma simulação de partículas sujeitas a forças gravíticas e elétricas.

A simulação decorre num espaço tridimensional, onde um conjunto de partículas, com massa e carga, são colocadas. A cada momento da simulação, cada particula ocupa uma posição no espaço, determinada pelas suas coordenadas $(x,y,z)$.

Existem dois tipos de partículas:

- Estacionárias, que não se movem ao longo da simulação, mas interagem com as outras partículas, através das forças gravíticas e elétricas. Cada simulação contempla zero ou mais partículas estacionárias.
- Móveis, que se movem ao longo da simulação, de acordo com as forças gravíticas e elétricas. Cada simulação contempla uma particula móvel. A partícula móvel pode ter velocidade inicial não nula.

A cada momento, $t$, a partícula móvel é caraterizada por:

- $m$ - Massa, em kg, onde $m \in \mathbb{R}, m \geq 0$.
- $q$ - Carga, em Coulomb, onde $q \in \mathbb{Z}$.
- $p_t = (x_t,y_t,z_t)$ - Coordenadas, em metros, onde $(x,y,z) \in \mathbb{R}^3$.
- $v_t = (v_{x_t}, v_{y_t}, v_{z_t})$ - Velocidade, em metros por segundo, onde $v_{x_t}$, $v_{y_t}$, $v_{z_t} \in \mathbb{R}$.
- $a_t = (a_{x_t}, a_{y_t}, a_{z_t})$ - Aceleração, em metros por segundo ao quadrado, onde $a_{x_t}, a_{y_t}, a_{z_t} \in \mathbb{R}$.
- $Fg_t = (Fg_{x_t}, Fg_{y_t}, Fg_{z_t})$ - Força gravítica, em Newtons, onde $Fg_{x_t}, Fg_{y_t}, Fg_{z_t} \in \mathbb{R}$.
- $Fe_t = (Fe_{x_t}, Fe_{y_t}, Fe_{z_t})$ - Força elétrica, em Newtons, onde $Fe_{x_t}, Fe_{y_t}, Fe_{z_t} \in \mathbb{R}$.
- $F_t = (F_{x_t}, F_{y_t}, F_{z_t})$ - Força total, em Newtons, onde $F_{x_t}, F_{y_t}, F_{z_t} \in \mathbb{R}$. A força total é a soma da força gravítica e da força elétrica.

Com base numa configuração do espaço, é possível efetuar várias simulações.  Uma simulação é definida por:

- $\Delta t$ - Intervalo de tempo entre cada momento da simulação, em milisegundos, onde $\Delta t \in \mathbb{N}, \Delta t > 0$.
- $t_{max}$ - Tempo máximo de simulação, em milisegundos, onde $t_{max} \in \mathbb{N}, t_{max} > \Delta t$.

O objetivo de cada simulação é determinar a trajetória da partícula móvel, ou seja, a sequência de posições ($p_t, \forall t \in [t_0, ..., t_{max}]$) que a partícula ocupa ao longo da simulação. Entre cada momento (ou seja, durante $\Delta t$ milisegundos), o movimento da partícula móvel é considerado uniforme.

A aplicação deve suportar vários utilizadores, cada um com um conjunto de espaços de simulação.

Cada espaço de simulação contém um conjunto de partículas estacionárias e uma partícula móvel. Deve ser possível adicionar e remover partículas, alterar a massa, carga, posição inicial, e velocidade inicial das partículas num espaço de simulação.

Qualquer partícula do espaço pode ser selecionada para ser a partícula móvel, passando a ser considerada a sua velocidade inicial na simulação. Durante a simulação, a velocidade inicial das partículas estacionárias é ignorada.

Para aprovação em Algoritmia e Programação, o trabalho tem que implementar as instruções descritas nas [instruções](#instruções). Estas descrevem um conjunto de instruções com as quais os utilizadores, através do terminal, interagem com o programa. A interação deve seguir rigorosamente as regras descritas pela nas [instruções](#instruções), já que o trabalho será validado com recurso a conjunto de testes rigorosos. Serão disponibilizados testes de treino, semelhantes aos utilizados na validação

## Instruções

Na descrição das várias instruções é indicada a sua sintaxe. Quer para as entradas, como para as saídas, as palavras são separadas por espaços em branco, e cada linha é terminada por um caráter fim de linha (`\n`).

Para cada instrução são indicadas as expressões de saída, quer para execuções com sucesso, quer para insucesso.

No caso de insucesso só deve surgir uma mensagem de erro. Verificando-se várias situações de insucesso em simultâneo, deve surgir apenas a mensagem do primeiro cenário de insucesso considerado, de acordo com a ordem de saídas de insucesso descritas para cada instrução. Qualquer cenário de insucesso não considerado pode ser ignorado, porque não será testado.

Caso o utilizador introduza uma instrução inválida, ou seja, não prevista na lista de instruções desta secção, ou um número de parâmetros errado para uma instrução existente, o programa deve escrever:

    Instrução inválida.

Pode assumir que não existem erros de representação de informação (e.g., texto em vez de valores numéricos).

A descrição de cada instrução pretende ser exaustiva, sem ambiguidades, e suficiente. Não deve ser possível optar entre vários comportamentos possíveis na mesma situação. Se essa situação ocorrer deve entrar em contacto com equipa docente.

A implementação não deve suportar mais instruções do que [as que estão descritas](#instruções). No entanto, nos testes públicos e privados não existirão instruções a começar pela letra `X`, pelo que pode ser utilizada para instruções convenientes ao desenvolvimento.

O programa termina quando for introduzida uma linha em branco, fora do contexto de uma instrução.

### Registar utilizador (RJ)

Regista um novo utilizador. Cada utilizador tem um nome único, sem espaços. Não existe limite para o número de utilizadores registados.

`Nome` é um nome de um utilizador.

Entrada:

        RJ Nome

Saída com sucesso:

        Utilizador registado com sucesso.

Saída com insucesso:

- Quando já existe um utilizador com o mesmo nome:

        Utilizador existente.

### Remover utilizador (EJ)

Remove um utilizador previamente registado. Não é possível remover um utilizador com espaços de simulação sem simulações realizadas.

`Nome` é um nome de um utilizador.

Entrada:

        EJ Nome

Saída com sucesso:

        Utilizador removido com sucesso.

Saída com insucesso:

- Quando o utilizador indicado não existe:

        Utilizador não existente.

- Quando o utilizador indicado tem espaços de simulação sem simulações realizadas:

        Utilizador tem espaços de simulação sem simulações realizadas.

### Listar utilizadores (LJ)

Lista os utilizadores registados por ordem alfabética de `Nome`, indicando o número de total de espaços de simulação, e o número total de simulações realizadas.

`Nome` é um nome de um utilizador, `NumeroDeEspacos` é o número de espaços de simulação, e `NumeroDeSimulacoes` é o número de simulações realizadas.

Entrada:

        LJ

Saída com sucesso (deve surgir uma linha por cada utilizador registado):

        Nome NumeroDeEspacos NumeroDeSimulacoes
        Nome NumeroDeEspacos NumeroDeSimulacoes
        ...

Saída com insucesso:

- Quando não existem utilizadores registados.

        Não existem utilizadores registados.

### Registar espaço de simulação (RE)

Regista um novo espaço de simulação. Cada espaço de simulação tem um identificador numérico único para o utilizador. Não existe limite para o número de espaços de simulação registados por utilizador.

O identificador é gerado automaticamente pelo programa, para cada utilizador. Inicia em 1, e é incrementado em 1 para cada novo espaço de simulação registado pelo utilizador.

`Nome` é um nome de um utilizador, `IdentificadorEspaço` é um identificador de espaço de simulação.

Entrada:

        RE Nome

Saída com sucesso:

        Espaço de simulação registado com identificador IdentificadorEspaço.

Saída com insucesso:

- Quando o utilizador indicado não existe:

        Utilizador inexistente.

### Remover espaço de simulação (EE)

Remove um espaço de simulação previamente registado.

`Nome` é um nome de um utilizador, `IdentificadorEspaço` é um identificador de espaço de simulação.

Entrada:

        EE Nome IdentificadorEspaço

Saída com sucesso:

        Espaço de simulação removido com sucesso.

Saída com insucesso:

- Quando o utilizador indicado não existe:

        Utilizador inexistente.

- Quando o espaço de simulação indicado não existe:

        Espaço de simulação inexistente.

### Registar partícula (RP)

Regista uma nova partícula. Cada partícula tem um identificador numérico único para o espaço de simulação. Não existe limite para o número de partículas registadas por espaço de simulação.

O identificador é gerado automaticamente pelo programa, para cada espaço de simulação. Inicia em 1, e é incrementado em 1 para cada nova partícula registada pelo espaço de simulação.

`Nome` é um nome de um utilizador, `IdentificadorEspaço` é um identificador de espaço de simulação, `IdentificadorParticula` é um identificador de partícula. `Massa` é a massa da partícula, `Carga` é a carga da partícula, `(PosicaoInicialX, PosicaoInicialY, PosicaoInicialZ)` é a posição da partícula, e `(VelocidadeInicialX, VelocidadeInicialY, VelocidadeInicialZ)` é a velocidade inicial da partícula.

Se não existirem valores para a velocidade inicial (i.e., a linha está em branco), esta é considerada nula.

Se não existirem valores para a posição inicial (i.e., a linha está em branco), a instrução deve ser considerada inválida.

Entrada:

        RP Nome IdentificadorEspaço
        Massa Carga
        PosicaoInicialX PosicaoInicialY PosicaoInicialZ
        VelocidadeX VelocidadeY VelocidadeZ

Saída com sucesso:

        Partícula registada com identificador IdentificadorParticula.

Saída com insucesso:

- Quando o utilizador indicado não existe:

        Utilizador inexistente. FEITO

- Quando o espaço de simulação indicado não existe:FEITO

        Espaço de simulação inexistente.

- Quando a massa indicada é inválida: 

        Massa inválida.

### Alterar partícula (AP)

Altera uma partícula previamente registada.

`Nome` é um nome de um utilizador, `IdentificadorEspaço` é um identificador de espaço de simulação, `IdentificadorParticula` é um identificador de partícula. `Massa` é a massa da partícula, `Carga` é a carga da partícula, `(PosicaoInicialX, PosicaoInicialY, PosicaoInicialZ)` é a posição da partícula, e `(VelocidadeInicialX, VelocidadeInicialY, VelocidadeInicialZ)` é a velocidade inicial da partícula.

Se não existirem valores para a velocidade inicial (i.e., a linha está em branco), esta é considerada nula.

Se não existirem valores para a posição inicial (i.e., a linha está em branco), a partícula é removida.

Entrada:

        AP Nome IdentificadorEspaço IdentificadorParticula
        Massa Carga
        PosicaoInicialX PosicaoInicialY PosicaoInicialZ
        VelocidadeX VelocidadeY VelocidadeZ

Saída com sucesso:

- Quando existem valores para a posição inicial:

        Partícula alterada com sucesso.

- Quando não existem valores para a posição inicial:

        Partícula removida com sucesso.

Saída com insucesso:

- Quando o utilizador indicado não existe:

        Utilizador inexistente.

- Quando o espaço de simulação indicado não existe:

        Espaço de simulação inexistente.

- Quando a partícula indicada não existe:

        Partícula inexistente.

- Quando a massa indicada é inválida:

        Massa inválida.

### Simular (S)

Simula o movimento de todas as partículas registadas num espaço de simulação.

`Nome` é um nome de um utilizador, `IdentificadorEspaço` é um identificador de espaço de simulação, `IdentificadorParticula` é o identificador da partícula móvel, `Tempo` é o tempo de simulação, `Passo` é o passo de simulação. `NomeFicheiro` é o nome do ficheiro onde será guardada a simulação no caso de sucesso. Se `NomeFicheiro` for igual a `-`, a simulação é apenas mostrada na consola.

A saída com sucesso é uma tabela com uma linha para cada instante de tempo, e uma coluna para cada variável. A primeira linha contém os nomes das variáveis. Em cada linha, os valores estão separados por vírgulas, sem espaços.

Para cada momento `t` da simulação, as variáveis são:

- `t` é o instante de tempo.
- `px, py, pz` são as coordenadas da partícula no espaço.
- `vx, vy, vz` são as componentes da velocidade da partícula.
- `ax, ay, az` são as componentes da aceleração da partícula.
- `fgx, fgy, fgz` são as componentes da força gravitacional da partícula.
- `fex, fey, fez` são as componentes da força elétrica da partícula.
- `fx, fy, fz` são as componentes da força total da partícula.

Entrada:

        S Nome IdentificadorEspaço IdentificaorPartícula
        Tempo Passo
        NomeFicheiro

Saída com sucesso:

        t,px,py,pz,vx,vy,vz,ax,ay,az,fgx,fgy,fgz,fex,fey,fez,fx,fy,fz
        0,px0,py0,pz0,vx0,vy0,vz0,ax0,ay0,az0,fgx0,fgy0,fgz0,fex0,fey0,fez0,fx0,fy0,fz0
        1,px1,py1,pz1,vx1,vy1,vz1,ax1,ay1,az1,fgx1,fgy1,fgz1,fex1,fey1,fez1,fx1,fy1,fz1
        ...
        T,pxT,pyT,pzT,vxT,vyT,vzT,axT,ayT,azT,fgxT,fgyT,fgzT,fexT,feyT,fezT,fxT,fyT,fzT

Saída com insucesso:

- Quando o utilizador indicado não existe:

        Utilizador inexistente. 

- Quando o espaço de simulação indicado não existe:

        Espaço de simulação inexistente.

- Quando os parâmetros da simulação são inválidos:

        Parâmetros inválidos.

## Estrutura do projeto

A estrutura mínima do projeto deve ser a seguinte:

      projeto
      |-- main.c : ficheiro com o programa.
      |-- iotests : diretório com testes de output, a distribuir pela docência
      |-- figures : diretório com imagens utilizadas neste ficheiro
      |-- README.md : este ficheiro
      |-- REPORT.md : relatório do projeto

O repositório de referência está disponível em <https://github.com/IADE-PA/IADE-PA-2022-2023-Projeto>

Para efetuar a atualizações:

1. Registar o repositório como `upstream` (só deve acontecer uma vez)

        git remote add upstream https://github.com/IADE-PA/IADE-PA-2022-2023-Projeto

2. Atualizar o `upstream` (sempre que existirem atualizações)

        git fetch upstream

3. Obter as alterações (e.g., ficheiro `README.md`)

        git checkout upstream/main README.md

## Testes de *input*/*output*

O projeto é validado através de um conjunto de baterias de teste de *input*/*output*.

Cada bateria é constituída por um ficheiro de entrada e outro e saída. O ficheiro de entrada contém uma sequência de instruções a passar pelo programa que, por sua vez, deve produzir uma sequência de saída *exatamente* igual ao ficheiro de saída. A comparação será feita *byte* a *byte*, pelo que não podem existir quaisquer diferenças para o programa ser considerado válido.

As baterias serão distribuídas através do repositório git de referência, na diretoria `iotests` (será necessário registar o repositório de referência como `upstream`, de acordo com as instruções na secção sobre [estrutura do projeto](#estrutura-do-projeto)).

Os grupos de trabalho devem utilizar as baterias públicas para validar o desenvolvimento do projeto.

### Utilização de testes

Os testes disponibilizados devem ser utilizados por ordem, já que os cenários descritos num teste podem depender dos cenários dos testes anteriores. Na linha de comandos Windows (Command Prompt) e em GNU Linux/MacOs, cada teste deve ser utilizado da seguinte forma:

    main < 1.in > 1.mine.out

A instrução `<` redireciona o *standard input* para o ficheiro indicado, e a instrução `>` redireciona o *standard output* para o ficheiro indicado (cria se não existir, e escreve por cima se existir).

No exemplo, o ficheiro `1.in` contém várias instruções para testar o programa. O ficheiro `1.out` contém as saídas correspondentes às instruções no ficheiro `1.in`. O ficheiro `1.mine.out` será criado com as saídas que o programa gerar. Os ficheiros `1.out` e `1.mine.out` devem ser
idênticos.

A comparação entre ficheiros pode ser feita na linha de comandos, recorrendo ao programa `comp` em Windows, ou `diff` em Linux e MacOS.

A comparação também pode ser feita recorrendo a alguns IDEs ou editores de texto, tal como o [*Visual Studio Code*](https://code.visualstudio.com/). Para tal, abra o diretório do projeto em *File - Open Folder*. Selecione o diretório do projeto confirme em *Select Folder*. Caso não esteja visível, abra o explorador de ficheiros do *Visual Studio Code* em *View - Explorer*.

Selecione os dois ficheiros a comparar (selecione um, e selecione o outro enquanto pressiona na tecla `Ctrl`, ou `Cmd` em MacOS).

![](figures/vsc_two_file_selection.png)

Com os dois ficheiros selecionados, utilize o botão direito do rato em cima da seleção para obter um menu. Escolha *Compare Selected*.  

![](figures/vsc_compare_selected.png)

O *Visual Studio Code* apresenta os dois ficheiros com as diferenças a vermelho e verde. Caso não existam diferenças, as linhas dos dois ficheiros surgem sem cores.

## Entrega

A entrega do projeto é feita no *GitHub Classroom* e na plataforma de *e-learning*.

Deve existir, na raiz do repositório, um ficheiro de relatório `REPORT.md` com a identificação dos elementos do grupo de trabalho, e eventuais comentários relativos a estratégias de implementação adotadas, e/ou à distribuição de tarefas.

A ausência de identificação individual no ficheiro de relatório implica a anulação da participação individual no projeto.

O código fonte entregue será sujeito a validação por um conjunto de testes reservado para esse efeito, e será compilado com a seguinte instrução:

```bash
make main
```

A compilação deve gerar o programa `bin/main`, que será executado com as instruções dos testes.

A entrega no *e-learning* corresponde a um ficheiro `zip` do repositório *GitHub Classroom*, excluindo eventuais ficheiros compilados.

## Prova de Autoria

Todos os projetos entregues serão sujeitos a prova de autoria. Para esse efeito, cada grupo terá que efetuar uma discussão com a docência, de forma a demonstrar que o código entregue foi de facto feito pelo grupo, e que a distribuição de trabalho foi equilibrada.

O calendário das provas de autoria será disponibilizado no *e-learning*, após o prazo de entrega da implementação do projeto.

A não comparência na prova de autoria implica a anulação da participação individual no projeto.

## Tecnologias

O projeto deve ser implementado em C, norma C11. Recomenda-se a utilização de um compilador C11, tal como o *gcc* ou o *clang*:

- gcc versão 11.3, ou superior.
- clang versão 15.0.5, ou superior.

Não podem ser utilizadas extensões à linguagem, mesmo quando suportadas pelos compiladores. Não podem ser utilizadas bibliotecas externas à distribuição padrão (`stdlib`). No caso de dúvida, o corpo docente deve ser consultado.

## Grupo de trabalho

Os grupos de trabalho devem ter entre 3 e 4 pessoas. Idealmente, todos os elementos do grupo devem participar em todos os aspetos do projeto. No entanto, pode existir divisão de tarefas, sendo que esta deve ser equilibrada.

Grupos com um número diferente de pessoas devem ser explicitamente autorizados pelo corpo docente.

## Avaliação

O projeto é avaliado com base em duas componentes: quantitativa (*A*), e qualitativa (*B*). A nota final do projeto é determinada por *(0.5 x A) + (0.5 x B)*.

| Instrução | Peso |
| --------- | :--: |
| RJ        |  2   |
| EJ        |  2   |
| LJ        |  2   |
| RE        |  2   |
| EE        |  2   |
| RP        |  2   |
| AP        |  2   |
| S         |  6   |

A avaliação qualitativa irá considerar que existem várias formas de resolver o problema descrito, mas exige-se a utilização dos instrumentos e métodos apresentados na unidade curricular, nomeadamente:

- Separação entre interface, dados, e lógica da aplicação.
- Justificação clara para as variáveis e operações implementadas.
- Utilização de estruturas de dados adequadas.
- Utilização de algoritmos com o menor custo computacional possível.
- Organização da solução coerente com a metodologia apresentada na unidade curricular.

A implementação estrita de todas as instruções descritas neste enunciado assegura, sem prejuízo de reprovação por irregularidade académica, a nota mínima de 10 valores.

As notas finais do projeto serão disponibilizadas na plataforma de *e-learning*.
