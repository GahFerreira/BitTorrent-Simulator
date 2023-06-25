# BitTorrent Simulator

Simulação do protocolo BitTorrent usando threads para a disciplina de Sistemas Operacionais.

Para rodar, use:

`make`

e depois

`./simulador.exe`

É possível usar `make dbg1`, `make dbg2`, etc para mostrar mensagens de debug. 
Quanto maior o número de `dbg`, mais mensagens.

Atenção: é necessário fazer `make clean` entre as compilações.

Nessa versão do programa, as alocações dinâmicas não são removidas ao final.