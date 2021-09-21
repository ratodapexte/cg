# Projeto de Computação Gráfica

Alunos:
* [Victor Hugo de Lima Araujo](https://github.com/ratodapexte)
* [Jonas](https://github.com/)

A versão utilizada foi a MinGW e para compilar o programa é necessário utilizar o seguinte comando no cmd ou powershell do Windows dentro da pasta onde se encontra o main.c:
~~~
gcc main.c -o main.exe -I"C:\MinGW\freeglut\include" -L"C:\MinGW\freeglut\lib" -lfreeglut -lopengl32 -lglu32
~~~
Em seguida, executar o main.exe.

### Implementações
- [X] Cozinha
- [X] Iluminação básica
- [X] Movimentação
- [X] Câmera em primeira pessoa
- [X] Movimentação da porta principal
- [X] Movimentação da porta secundária
- [X] Movimentação das janelas
- [X] Ventilador de teto com movimento em sentido horário
- [X] Quadro com uma bandeira
- [X] Texturas em pelo menos 5 objetos

### Renderização de uma cozinha com:
* 3 canecas
* 3 pratos
* Geladeira
* 2 Portas
* Mesa
* 3 cadeiras
* Ventilador de teto
* Quadro com bandeira


### Referências

* <http://www.glprogramming.com/red/chapter05.html>
* <https://github.com/EduardoLisboa/Projeto_CG>
* <https://raw.githubusercontent.com/valeriojr/monitoria-cg/master/material/book.pdf>
