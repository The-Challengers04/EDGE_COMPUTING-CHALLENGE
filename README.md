#Sistema de navegação para PCD Visuais
O Inclui+ desenvolveu um sistema de navegação de pessoas com deficiência visual, do qual se consiste em um sensor de distancia que deve ser disposto nos ombros ou na cabeça do usuário do qual poderá ser notificado sobre obstáculos presentes no seu caminho.
Para acessar o modelo do tinkerCad [Clique aqui](https://www.tinkercad.com/things/6z3wlJXVywO?sharecode=2Wt-mt88Bkw27Qaz3jJ92Pu6_UWkrKaGf1u3C758Ey8)

---
##MONTAGEM
####Materias Necessários Para a Montagem
Componente|quantidade
:-:|:-:
Placa Base Arduino UNO|1
Servo Motor | 1
Sensor ultrassônico(HC-SR04)|1
Piezo | 1

####Modelo de Montagem
![Modelo de montagem do tinkerCad](./images/Modelo%20TinkerCad.JPG)

####Implementação do Código
O codigo pode ser copiado do arquivo:
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

---
##FUNCIONAMENTO
O sensor de distancia ultrassônico aponta para o chão ate a distancia definida no sistema, criando um angulo que é gerenciado pelo servo motor, dessa forma cria uma figura de um triangulo retângulo:
![Imagem ilustrativa da forma criada](./images/Desenho%20do%20Funcionamento%20Padr%C3%A3o.JPG)
Dessa forma, é possível detectar um objeto que entre no raio do sensor ultrassônico e calcular a distancia dele em relação ao usuário e alertá-lo da obstrução.
![Imagem ilustrativa de um obstaculo sendo detectado](./images/Desenho%20Captando%20Obstaculo.JPG)
Ao detectar um obstaculo, o Piezo começara a apitar, e seu tempo de intervalo diminui proporcionalmente em relação a distancia captada.
