Teh Engine
==========

Motor de jogos 3D que utiliza àrvores BSP para acelerar gráficos e detecção de
colisões.


Dependências
------------

Ferramentas:

* gcc
* make
* pkg-config

Bibliotecas:

* OpenGL ES 2.0
* SDL2
* SDL2\_image
* SDL2\_mixer
* check (libcheck, unit testing)
* libgvc (graphviz)
* blender (3D editor)


Compilação
----------

Digite:

    make
    make tehs
    make behs

para compilar todos os programas, modelos 3D e árvores BSP.


Testes
------

Digite:

    make check

aperte enter e começe a rezar.


Executar
--------

Digite:

    build/engine

Controles:

|  tecla   | comando                   |
|----------|---------------------------|
|   `h`    | move para a esquerda      |
|   `j`    | move para trás            |
|   `k`    | move para frente          |
|   `l`    | move para a direita       |
|   `b`    | move para cima            |
|`<espaço>`| move para baixo           |
|   `w`    | visualizar wireframes     |
|   `m`    | controla câmera com mouse |

