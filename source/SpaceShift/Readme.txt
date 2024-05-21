Comandos:
	- W e S alteram o nível base de refinamento global dos patches.
	- E habilita e desabilita o modo de visualização por wireframe.
	- R habilita e desabilita a textura do terreno.
	- Os botões I, J, K e L controlam a movimentação do jogador, que se desloca pelo plano.
	- As setas para os lados deslocam a camera nos eixos X, o Shift e o Control esquerdo controlam o Z.
	- Os botões 1 e 2 habilitam e desabilitam a iluminação das fontes luminosas branca (1) e vermelha (2).
	- W e S rotacionam o terreno em relação ao eixo Y, ajuda a visualizar a iluminação.
	- O e P controlam o grau de displacement, permitindo aumentar (P) e diminuir (O) a intensidade dos desníveis do terreno.

Comentários:
	- Os nível de refinamento de cada patch é definido pela distancia do que seria a projeção do "jogador" (ponto vermelho) no terreno.
	- Também é levado em consideração um nível base de detalhamento, que pode ser alterado ao pressionar W e S.
	- A camada externa dos patches com níveis diferentes de refinamento se adequam à transição para evitar os vertices T.
	- A normais foram geradas e utilizadas na iluminação.
	- Para o displacement mapping, foi utilizada a mesma textura do terreno. Para visualizar com maior detalhamento o 
	displacement basta aumentar o nível de refino e controlar o grau de displacement.
	- Ao invés de utilizar a camera, para facilitar a visualização foi simulado a posição de um jogador projetado ao terreno, 
	o nível de refino varia de acordo com a localização deste jogador, a medida que os patches ficam mais distantes o nível de 
	detalhamento de refino diminui.
	- O patch mais próximo do jogador é aquele com maior grau de tesselation.