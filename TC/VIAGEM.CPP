cont=0;
if(string1[i]=='(') string2[i]=cont; count++;
if(string1[i]==')') string2[i]=cont; count--;

- cada byte que nao seja parenteses recebe um valor inicial, que e incremen-
  tado cada vez que ele se repete.
- os parenteses recebem valores a partir 0 ate 69, o algoritmo
  faz com que os parenteses que se complementam tenham valores iguais.

  ((((4*5))+10)+7)

//                bytes
//0  1  2  3  4  5  6  7  8  9  10 11  12 13 14


                                                    oper   --> > 90
  0  1  2  3  90 80 91 3  2  70 92  1  71 93 0      * ou / --> > 80 e < 89
  (  (  (  (  4  *  5  )  )  +  10  )  +  7  )      + ou - --> > 70 e < 79
  |  |  |  |___________|  |         |        |  parenteses --> < 69
  |  |  |_________________|         |        |
  |  |______________________________|        |
  |__________________________________________|

1) le o string2 e pega somente os valores iguais. Dentre estes valores, pega
   os menores - no caso, os bytes string2[0] e string2[14].

2) copia um substring a partir do '(' ate


