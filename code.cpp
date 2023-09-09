#include <Servo.h>

double pi = 2 * acos(0.0);

// DEFININDO PINOS
const int TRIGGER_PIN = 7;
const int ECHO_PIN = 6;
const int SERVO = 9;

// DEFININDO VARIAVEIS DO SISTEMA
unsigned int Altura = 175;            // Medida em cm
unsigned int DistanciaDesejada = 200; // Medida em cm
int Tolerancia = 5;                   // Medida de tolerancia que há entre a distancia obtida e a distancia determinada
int intervalo_de_aviso = 2 * 1000;

long objDetLT = -intervalo_de_aviso; // Object Detectado Pela Ultima Vez

long HipDistancia = 0;
double ServoAngle = 0;

Servo servo; // Instanciando a classe Servo

void setup()
{
    Serial.begin(9600); // Inicializa a comunicação serial

    // DEFININDO PINAGEM DO SISTEMA
    pinMode(TRIGGER_PIN, OUTPUT); // Configura o pino Trigger como saída
    pinMode(ECHO_PIN, INPUT);     // Configura o pino Echo como entrada

    //=========== Calculando a medida de Distancia ====================
    HipDistancia = sqrt((long)Altura * Altura + DistanciaDesejada * DistanciaDesejada);
    Serial.print("Medida De Distancia a seguir: ");
    Serial.println(HipDistancia);

    //============= Calculando o angulo do Servo ==========================
    ServoAngle = getAngleOf(Altura, DistanciaDesejada);
    Serial.print("Angulo do Servo Motor: ");
    Serial.println(ServoAngle);

    // Configurando o Servo
    servo.attach(SERVO);     // Inicializando o servo
    servo.write(ServoAngle); // Direcionando o servo para o angulo desejado
}

void loop()
{

    delay(50); // Aguarda 50ms entre medições para evitar interferências
    // Realiza uma medição de distância em centímetros

    unsigned int distancia = calcularDistancia(); // Obtem a distancia capturada pelo sensor

    if ((HipDistancia - distancia) > Tolerancia)
    {
        // Há um obstaculo sendo detectado

        // Calcular a altura do objeto
        int AlturaObjeto = cos(ServoAngle * pi / 180) * distancia;
        AlturaObjeto = Altura - AlturaObjeto;

        // Calcula a distancia do objeto em relação ao usuario
        int DistanciaObjeto = sin(ServoAngle * pi / 180) * distancia;

        Serial.print("Altura:");
        Serial.println(AlturaObjeto);

        Serial.print("Distancia:");
        Serial.println(DistanciaObjeto);
    }
    else if ((HipDistancia - distancia) < -Tolerancia)
    {
        // Há um "degrau/buraco/desnivel" à frente

        // Calcular a altura do buraco
        int ProfundidadeBuraco = cos(ServoAngle * pi / 180) * distancia;
        ProfundidadeBuraco = (ProfundidadeBuraco - Altura) * -1; // Altura real

        // Calcula a distancia do buraco em relação ao usuario
        int DistanciaBuraco = sin(ServoAngle * pi / 180) * distancia;

        Serial.print("Altura: -");
        Serial.println(ProfundidadeBuraco);

        Serial.print("Distancia: ");
        Serial.println(DistanciaBuraco);
    }

} // Fim Loop()

// ======================= FUNÇÕES =========================

// Função para calcular a distância em centímetros
unsigned int calcularDistancia()
{
    // Envia um pulso de 10µs no pino Trigger para iniciar a medição
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    // Lê o tempo de duração do pulso recebido no pino Echo
    unsigned long duracaoPulso = pulseIn(ECHO_PIN, HIGH);

    // Converte a duração do pulso em microssegundos para centímetros
    // (a velocidade do som é de aproximadamente 29,1 microssegundos por centímetro)
    unsigned int distancia = duracaoPulso / 58;

    return distancia;
}

// Função para calcular o angulo que o sensor deve ser
//  ajustado para se adequar aos padrões estabelecidos
double getAngleOf(int Altura, int DistanciaDjd)
{
    double tang = (double)DistanciaDjd / Altura;
    double angle = -1;
    for (double i = 1; i < 90; i++)
    {
        double tangI = tan(i * pi / 180);
        if (tang > tangI)
            continue;
        if (tang == tangI)
        {
            angle = i;
            break;
        }

        while (tang < tangI)
        {
            i -= 0.01;
            tangI = tan(i * pi / 180);
        }
        angle = i;
        break;
    }
    return angle;
}