# Sistema de navegação para PCD Visuais
O Inclui+ desenvolveu um sistema de navegação de pessoas com deficiência visual, do qual se consiste em um sensor de distancia que deve ser disposto nos ombros ou na cabeça do usuário do qual poderá ser notificado sobre obstáculos presentes no seu caminho.

Para acessar o modelo do tinkerCad [Clique aqui](https://www.tinkercad.com/things/9lrOj3GNvcq)

---
DRAFT
===
![Draft da estrutura do sistema](./images/DraftEstrutura.jpg)
---
MONTAGEM DO DISPOSITIVO
==============
#### Materias Necessários Para a Montagem do Sistema
|             Componente             | quantidade |                                                  Descrição                                                   |
| :--------------------------------: | :--------: | :----------------------------------------------------------------------------------------------------------: |
|     **Placa Base Arduino UNO**     |     1      |                 Vai receber as informações do sensor e redirecionar os dados para o celular                  |
|          **Servo Motor**           |     1      |                           Utilizado para posicionar o *HC-SR04* no angulo correto                            |
|  **Sensor ultrassônico(HC-SR04)**  |     1      |                          Responsável por detectar obstáculos na direção do usuário                           |
| **Modulo Bluetooth (HC-05/HC-06)** |     1      | Será utilizado para realizar uma conexão com o celular e enviar os dados de obstáculos no caminho do usuário |



#### Modelo de Montagem
![Modelo de montagem do tinkerCad](./images/Modelo%20TinkerCad.JPG)
![Modelo de conexão do Modulo de Bluetooth](./images/ModeloConexãoHC-06.jpg)

#### Implementação do Código
O codigo pode ser copiado do arquivo: [code](./code.cpp)
Apos colar ele na IDE do Arduino, deve mudar alguns padrões para o funcionamento correto:  
```C++
//DEFININDO VARIAVEIS DO SISTEMA
unsigned int Altura = 175; // Medida em cm
unsigned int DistanciaDesejada = 200; // Medida em cm
int Tolerancia = 5; // Medida de tolerancia que há entre a distancia obtida e a distancia determinada
int intervalo_de_aviso = 2 * 1000;// Intervalo em milisegundos que o arduino imprime a informação no Serial Monitor
```

#### Regras
* O Servo Motor deve Ser conectado no pino 9 ou 10
* O sensor ultrassonico deve ser acoplado ao Servo motor
* A conexão do ***Modulo Bluetooth*** deve ser conectado somente apos salvar o código no Arduino
* O TR do ***Modulo Bluetooth*** deve se conectar com o TX do Arduino
* O TX do ***Modulo Bluetooth*** deve se conectar com o TR do Arduino


---
FUNCIONAMENTO
====
O sensor de distancia ultrassônico aponta para o chão ate a distancia definida no sistema, criando um angulo que é gerenciado pelo servo motor, dessa forma cria uma figura de um triangulo retângulo:
![Imagem ilustrativa da forma criada](./images/Desenho%20do%20Funcionamento%20Padr%C3%A3o.JPG)
Dessa forma, é possível detectar um objeto que entre no raio do sensor ultrassônico e calcular a distancia dele em relação ao usuário e alertá-lo da obstrução.
![Imagem ilustrativa de um obstaculo sendo detectado](./images/Desenho%20Captando%20Obstaculo.JPG)

---
Aplicativo
===
Esses dados vão ser enviados para o dispositivo Android conectado pelo Bluetooth.
![Imagem ilustrativa da conexão do arduino com o celular](./images/conexaoBluetooth.jpg)
Em nosso aplicativo, esses dados serão convertidos em bips sonoros que podem ser reproduzidos pelo próprio dispositivo ou outros conectados, como um fone de vibração óssea. 
![Tela de Bem-vindo do app](./images/BemVindoApp.jpg)

![Tela de dados do app](./images/RecebendoDadosApp.jpg)
> Essa é apenas uma tela para demostrar os dados que ele vai analisar. O objetivo é que o aplicativo rode em segundo plano reproduzindo apenas o bipes para o usuário.

O intervalo dos bips vai diminuir proporcionalmente em relação a distancia do objeto detectado.

Outra funcionalidade que nosso aplicativo oferece é o envio da localização atual do usuário para o um sistema de Nuvem. Possibilitando que familiares e/ou cuidadores possam agir caso o individuo se perca. 
![Tela de compartilhamento da localização em tempo real](./images/CompartilharLocalizacaoApp.jpg).
