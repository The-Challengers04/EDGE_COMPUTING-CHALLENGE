#include <Servo.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

#define HEIGHT_ADRESS 0
#define DISTANCE_ADRESS 1
#define TOLERANCE_ADRESS 2

double pi = 2 * acos(0.0);

// DEFININDO PINOS
const int TRIGGER_PIN = 7;
const int ECHO_PIN = 6;
const int SERVO = 9;

// DEFININDO VARIAVEIS DO SISTEMA
unsigned int Altura;            // Medida em cm
unsigned int DistanciaDesejada; // Medida em cm
int Tolerancia;                   // Medida de tolerancia que há entre a distancia obtida e a distancia determinada

long HipDistancia = 0;
double ServoAngle = 0;

Servo servo; // Instanciando a classe Servo

void adjustServoAngle(){
    ServoAngle = getAngleOf(Altura, DistanciaDesejada);
    servo.write(ServoAngle); // Direcionando o servo para o angulo desejado
}

void setHeight(unsigned int height){
    Altura = height;
    EEPROM.write(HEIGHT_ADRESS, height);
}
void setWantedDistance(unsigned int distance){
    DistanciaDesejada = distance;
    EEPROM.write(DISTANCE_ADRESS, distance);
}
void setTolerance(int tolerance){
    Tolerancia = tolerance;
    EEPROM.write(TOLERANCE_ADRESS, tolerance);
}

void adjustHipDistance(){
    HipDistancia = sqrt((long)Altura * Altura + DistanciaDesejada * DistanciaDesejada);
}

void adjustMeasures(unsigned int height,unsigned int wantedDistance,int tolerance){
    setHeight(height);
    setWantedDistance(wantedDistance);
    setTolerance(tolerance);
    adjustServoAngle();
    adjustHipDistance();
}

void readMeasuresFromMemory(){
    Altura = EEPROM.read(HEIGHT_ADRESS);
    DistanciaDesejada = EEPROM.read(DISTANCE_ADRESS);
    Tolerancia = EEPROM.read(TOLERANCE_ADRESS);
}

void setup()
{
    Serial.begin(9600); // Inicializa a comunicação serial

    // DEFININDO PINAGEM DO SISTEMA
    pinMode(TRIGGER_PIN, OUTPUT); // Configura o pino Trigger como saída
    pinMode(ECHO_PIN, INPUT);     // Configura o pino Echo como entrada

    servo.attach(SERVO);
    
    readMeasuresFromMemory();
    adjustServoAngle();
    adjustHipDistance();
    
}

void loop()
{
    StaticJsonDocument<200> jsonDocument;
    StaticJsonDocument<200> jsonBuffer;
    unsigned long startTime = millis();
    while (millis() - startTime < 50) {
        // salvar o JSON vindo do serial
        while (Serial.available() > 0) {
            DeserializationError error = deserializeJson(jsonBuffer, Serial);
            if (error) {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.c_str());
                return;
            }
            // mudar a altura, distancia e tolerancia
            if (jsonBuffer.containsKey("height")) {
                setHeight(jsonBuffer["height"]);
            }
            if (jsonBuffer.containsKey("wantedDistance")) {
                setWantedDistance(jsonBuffer["wantedDistance"]);
            }
            if (jsonBuffer.containsKey("tolerance")) {
                setTolerance(jsonBuffer["tolerance"]);
            }
            jsonBuffer.clear();
            adjustServoAngle();
            adjustHipDistance();
        }
    }
    // delay(50); 

    unsigned int distancia = calcularDistancia(); // Obtem a distancia capturada pelo sensor

    if ((HipDistancia - distancia) > Tolerancia)
    {
        // Há um obstaculo sendo detectado

        // Calcular a altura do objeto
        int AlturaObjeto = cos(ServoAngle * pi / 180) * distancia;
        AlturaObjeto = Altura - AlturaObjeto;

        // Calcula a distancia do objeto em relação ao usuario
        int DistanciaObjeto = sin(ServoAngle * pi / 180) * distancia;

        jsonDocument["obstacleDetected"] = true;
        jsonDocument["distance"] = DistanciaObjeto;
        jsonDocument["height"] = AlturaObjeto;
    }
    else if ((HipDistancia - distancia) < -Tolerancia)
    {
        // Há um "degrau/buraco/desnivel" à frente

        // Calcular a altura do buraco
        int ProfundidadeBuraco = cos(ServoAngle * pi / 180) * distancia;
        ProfundidadeBuraco = (ProfundidadeBuraco - Altura) * -1; // Altura real

        // Calcula a distancia do buraco em relação ao usuario
        int DistanciaBuraco = sin(ServoAngle * pi / 180) * distancia;

        jsonDocument["obstacleDetected"] = true;
        jsonDocument["distance"] = DistanciaBuraco;
        jsonDocument["height"] = ProfundidadeBuraco;
    }
    else {
        jsonDocument["obstacleDetected"] = true;
        jsonDocument["distance"] = 0;
        jsonDocument["height"] = 0;
    }

    String jsonString;
    serializeJson(jsonDocument, jsonString);

    Serial.println(jsonString);
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