# Renderização de Objetos 3D em C  

Este projeto apresenta a renderização de objetos 3D básicos no terminal utilizando a linguagem C. Os objetos disponíveis são:  
- Cubo (`cube.c`)  
- Cilindro (`cylinder.c`)  
- Vetor (`vector.c`)  

A renderização é realizada diretamente no terminal por meio de caracteres ASCII, com cálculos de perspectiva, iluminação e rotação aplicados a cada frame.  

## 🗂 Estrutura do Projeto  

- **`cube.c`**: Código para renderizar um cubo tridimensional rotacionando em tempo real.  
- **`cylinder.c`**: Código para renderizar um cilindro tridimensional.  
- **`vector.c`**: Código para visualização de vetores em 3D no terminal.  

## 📋 Requisitos  

Certifique-se de que sua máquina atende aos seguintes requisitos:  
- Sistema operacional: Linux, macOS ou Windows (com suporte a `gcc`).  
- Compilador GCC instalado.  
- Terminal com suporte a caracteres ANSI (para ocultar e reposicionar o cursor).  

## 🔧 Compilação e Execução  

Siga os passos abaixo para compilar e executar qualquer um dos arquivos:  

1. **Clone o repositório**:  
   ```bash
   git clone https://github.com/yaaggo/ascii-solids-rotation.git
   cd ascii-solids-rotation
   ```  

2. **Compile o arquivo desejado** (exemplo para `cube.c`):  
   ```bash
   gcc cube.c -o cube -lm
   ```  

3. **Execute o programa**:  
   ```bash
   ./cube
   ```  

4. **Saia do programa**:  
   Pressione `Ctrl + C` para interromper a execução.  

## 🎨 Controles e Funcionalidades  

- A rotação dos objetos ocorre automaticamente em torno de seus eixos.  
- A iluminação é simulada por meio de uma fonte de luz direcional fixa, com base em cálculos de normalização e ângulos de incidência.  

## 🪡 Descrição dos Arquivos  

### `cube.c`  
Renderiza um cubo em 3D rotacionando continuamente.  
- **Definições principais**:  
  - Tamanho do cubo: `CUBE_SIZE`.  
  - Distância da câmera: `CAM_DISTANCE`.  
  - Escala da projeção: `SCALE`.  

### `cylinder.c`  
Renderiza um cilindro tridimensional.  
- **Detalhes importantes**:  
  - Raio e altura definidos no código.  
  - Representado com precisão na projeção do terminal.  

### `vector.c`  
Exibe vetores em 2D com uma perspectiva simples.  
- **Funcionalidade adicional**:  
  - Manipulação de vetores para diferentes direções.  

## 🚧 Personalizações  

Você pode ajustar os parâmetros no início de cada arquivo para:  
- Alterar o tamanho e escala dos objetos.  
- Modificar a velocidade de rotação (`ROTATION_ANGLE`).  
- Ajustar a fonte de luz (`ld`).  


### Cilindro:  
Projeção de um cilindro estática ou rotacionada no terminal.  

### Vetor:  
Representação de vetores em 2D com rotação dinâmica.  
