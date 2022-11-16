![banner](https://user-images.githubusercontent.com/53827143/202071076-881e2e90-d34e-447e-9d36-7fef01bbec6e.png)

## Introdução
Emulador de CHIP-8, uma linguagem de programação interpretada, criada nos anos 70, que era
rodada em máquinas virtuais em microcomputadores da época, feito com C e SDL2.

## Demonstração

### Invaders
https://user-images.githubusercontent.com/53827143/202067693-081f1e90-50fe-45ea-8929-dbe3e73b7662.mp4

### Pong
https://user-images.githubusercontent.com/53827143/202067860-e970010e-2472-4de2-af4a-3247874a8a5a.mp4


[ROMs utilizadas.](https://www.pong-story.com/chip8/)

## Rodando

### Windows
Com o git instalado, clone o repositório através do comando:
```bash
git clone https://github.com/joao3/chip8-emulator.git
```
Para fazer a compilação, você pode pesquisar sobre como *linkar* a biblioteca SDL2 com o seu compilador e compilar os arquvivos c presentes na pasta raíz do projeto.

Se for utilizar o meu Makefile, você irá precisar dos seguintes itens:
- [Make instalado.](https://stackoverflow.com/questions/32127524/how-to-install-and-use-make-in-windows)
- [Compilador GCC (mingw).](https://sourceforge.net/projects/mingw/)
- SDL2. 
  1. Na raíz do projeto, cria uma pasta `lib`, dentro dela, uma pasta `SDL2`.
  2. [Baixe o zip *devel* do SDL2 para mingw.](https://github.com/libsdl-org/SDL/releases/tag/release-2.24.2)
  3. Dentro do zip baixado, abra a pasta correta de acordo com a versão do seu `gcc`:
      - 64 bits -> x86_64-w64-mingw32
      - 32 bits -> i686-w64-mingw32
  4. Extraia as pastas `include` e `lib` para a pasta `SDL2`, criada no item 1.

Na pasta raíz, compile rodando o comando:
```bash
make
```

Assumindo que você possui uma rom, rode o emulador com o comando: 
```bash
.\main.exe rom
```

## Referência
- <https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/>  
- <https://en.wikipedia.org/wiki/CHIP-8>
