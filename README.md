# SAA0356_2021-2_Bancada-de-Testes-DO160
Desenvolvimento de uma banca de testes para cumprimento com capítulo 16 da DO160

Professores:

Glauco Augusto de Paula Caurin
Daniel Varela Magalhães

Aluno:

André Oliveira Borges

# Escopo do projeto
O projeto tem como objetivo o desenvolvimento de uma bancada de testes para o capítulo 16 da norma DO160. Esse capítulo tem como título "Power Inputs", e define condições de entrada de tensão pelas quais o equipamento a ser testado deve ser submetido.

# Implementação
Uma placa controladora de propósito geral define as saídas de tensão de uma fonte controladora com interface GPIB. Essas saídas terão o perfil de tensão definidos pela norma. A tensão e corrente são medidas e servem de input para a placa controladora, afim que se tenha um registro e seja possível confirmar que o perfil do teste foi alcançado.

Uma interface com o usuário permite input de qual teste será executado, bem como acompanhar em tempo as etapas de execução do teste em si.

Sensores de pressão, umidade e temperatura são utilizados para apresentação ao usuário e também registro das condições atmosféricas durante o teste, ainda que esses não sejam compulsórios para sua execução.
