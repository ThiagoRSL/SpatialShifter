Comandos:
	- W e S alteram o n�vel base de refinamento global dos patches.
	- E habilita e desabilita o modo de visualiza��o por wireframe.
	- R habilita e desabilita a textura do terreno.
	- Os bot�es I, J, K e L controlam a movimenta��o do jogador, que se desloca pelo plano.
	- As setas para os lados deslocam a camera nos eixos X, o Shift e o Control esquerdo controlam o Z.
	- Os bot�es 1 e 2 habilitam e desabilitam a ilumina��o das fontes luminosas branca (1) e vermelha (2).
	- W e S rotacionam o terreno em rela��o ao eixo Y, ajuda a visualizar a ilumina��o.
	- O e P controlam o grau de displacement, permitindo aumentar (P) e diminuir (O) a intensidade dos desn�veis do terreno.

Coment�rios:
	- Os n�vel de refinamento de cada patch � definido pela distancia do que seria a proje��o do "jogador" (ponto vermelho) no terreno.
	- Tamb�m � levado em considera��o um n�vel base de detalhamento, que pode ser alterado ao pressionar W e S.
	- A camada externa dos patches com n�veis diferentes de refinamento se adequam � transi��o para evitar os vertices T.
	- A normais foram geradas e utilizadas na ilumina��o.
	- Para o displacement mapping, foi utilizada a mesma textura do terreno. Para visualizar com maior detalhamento o 
	displacement basta aumentar o n�vel de refino e controlar o grau de displacement.
	- Ao inv�s de utilizar a camera, para facilitar a visualiza��o foi simulado a posi��o de um jogador projetado ao terreno, 
	o n�vel de refino varia de acordo com a localiza��o deste jogador, a medida que os patches ficam mais distantes o n�vel de 
	detalhamento de refino diminui.
	- O patch mais pr�ximo do jogador � aquele com maior grau de tesselation.