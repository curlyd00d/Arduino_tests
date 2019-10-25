// BIBLIOTECA

#include <SoftwareSerial.h> // Biblioteca que emula as portas RX e TX em portas digitais.
#include <DHT.h> // Biblioteca do sensor de temperatura e umidade.
#define DHTPIN A0 // Declarando que o sensor de temperatura e umidade encontra-se na porta analógica A0. 
#define DHTTYPE DHT21 // Puxando na biblioteca as pré-definições para o sensor de temperatura e umidade DHT21.
DHT dht(DHTPIN, DHTTYPE);

// Neste campo está sendo definido as entradas do cooler, da bomba  do sensor de umidade do solo.
#define cooler 8
#define bomba 7
#define umidade A1
SoftwareSerial bluetooth(13,12); // RX,TX(Lembrando que estas portas invertem, assim o RX do sensor conecta no TX do arduino e o TX no RX)

// Campo de declaração de variáveis.
int temp_max = 28; // Temperatura máxima padrão.
int umid_min = 20; // Umidade mínima padrão.
int bombaBT = 0; // Botão para ligar e desligar bomba
int coolerBT = 0; // Botão para ligar e desligar cooler

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  Serial.println(F("DHTxx test!")); // Teste de leitura do sensor de temperatura e umidade(Obrigatório pela própria biblioteca).
  
  // Definição do COOLER e da BOMBA como portas de saída.
  pinMode(cooler, OUTPUT);
  pinMode(bomba, OUTPUT);
  dht.begin();
}


void loop() {
  
  // SENSOR DE BLUETOOTH
  
  // O código abaixo verificará se existe algo conectado, se sim irá ler as variáveis recebidas pelo sensor.
  if (bluetooth.available()){
    temp_max = bluetooth.read();
    umid_min = bluetooth.read();
    bombaBT = bluetooth.read();
    coolerBT = bluetooth.read(); 
  }
  
  // SENSOR DE UMIDADE DO SOLO
  
  int umidadeReal = map(umidade, 1023, 0, 0, 100); // Converte a leitura do sensor que é de 1023 a 0 para de 0 a 100.
  
  bluetooth.print("Umidade do solo: ");
  bluetooth.println(umidadeReal); // Apresentação em tela da leitura convertida.

  // Definido que caso a umidade lida seja menor que a umidade mínima declarada, a bomba será ativada (E apresentará em tela uma informação enquanto a bomba estiver ligada). Caso contrário, a mesma permanecerá desligada.
  if (analogRead(umidadeReal) < umid_min){
    digitalWrite(bomba, LOW);
    bluetooth.println("Bomba ligada");
  }
  else {
    digitalWrite(bomba,HIGH);
  }
  
  // Caso o valor recebido pela variável bombaBT seja 1 a bomba será ligada, se for 0 será desligada.
  if (bombaBT == 1){
    digitalWrite(bomba, LOW);
    bluetooth.println("Bomba ligada");
  }
  if (bombaBT == 0){
    digitalWrite(bomba, HIGH);
    bluetooth.println("Bomba desligada");
  }
// SENSOR DE TEMPERATURA E UMIDADE

  // Campo pré-definido pela biblioteca para leitura de Umidade, temperatura em Celsius.
  float h = dht.readHumidity(); // Leitura da umidade.
  float t = dht.readTemperature(); // Leitura da temperatura em Celsius (the default).
 
  //Caso o arduino não capture dados no sensor, informará o erro na leitura.
  if (isnan(h) || isnan(t)) {
    bluetooth.println(F("Falha na leitura do sensor DHT!"));
    return;
  }
  //Apresentação em tela a Umidade do ar e a Temperatura em Celcius.
  bluetooth.print(F("Umidade ar: "));
  bluetooth.print(h);
  bluetooth.print(F("%  Temperatura: "));
  bluetooth.print(t);
  bluetooth.println(F("C"));

  // Definido que caso a Temperatura em Celsius lida seja maior que a temperatura máxima declarada, o cooler será ativado (E apresentará em tela uma informação enquanto o cooler estiver ligado). Caso contrário, o mesmo permanecerá desligado.
  if (t > temp_max) {
    digitalWrite(cooler, LOW);
    bluetooth.println("Cooler Ligado");
  }
  else {
    digitalWrite(cooler, HIGH);
  }
  
  // Caso o valor recebido pela variável coolerBT seja 1 o cooler será ligado, se for 0 será desligado.
  if (coolerBT == 1){
    digitalWrite(cooler, LOW);
    bluetooth.println("Cooler Ligado");
  }
  if (coolerBT == 0){
    digitalWrite(cooler, HIGH);
    bluetooth.println("Cooler Desligado");
  }
}
