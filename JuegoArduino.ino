#define MAX_SECUENCIAS 3

#define PUSH1 4
#define PUSH2 5
#define PUSH3 6
#define PUSH4 7
#define PUSH_SOLUTION 2
#define PUSH_VERIFY 8

#define LED1 9
#define LED2 10
#define LED3 11
#define LED4 12
#define LED_GAMER 13
#define pinSound 3

#define DELAY_LED 700
#define DELAY_LED_WIN 100
#define DELAY_LED_LOST 500


int PUSHCACHE[8];
int SECUENCIA_MAQUINA[20];
int SECUENCIA_JUGADOR[20];
int LEVEL;
int PASO;
bool isTurnMachine;
bool isTurnUser;

bool isPushed(int push, int val) {
 bool pushed = (val == HIGH && val != PUSHCACHE[push]) ? true : false;
 PUSHCACHE[push] = val;
 return pushed;
}

int randomled() {
  return random(LED1, LED4 + 1);
}

void establecer_leds(int v) {
  digitalWrite(LED1, v);
  digitalWrite(LED2, v);
  digitalWrite(LED3, v);
  digitalWrite(LED4, v);
}

void apagar_leds() {
  establecer_leds(LOW);
}

void turno_maquina() {
  PASO = 0;
  LEVEL += 1;
  
  Serial.println();
  Serial.print("NEXT LEVEL: ");
  Serial.println(LEVEL);
  
  randomSeed(millis());
  generar_secuencia();
  mostrar_secuencia();

  isTurnMachine = false;
  isTurnUser = !isTurnMachine;
  Serial.print("secuencia usuario: ");
}

void generar_secuencia() {
  for(int i = 0; i < LEVEL; i++) {
    SECUENCIA_MAQUINA[i] = randomled();
  }
}

void mostrar_secuencia() {
  int LED;
  for(int i = 0; i < LEVEL; i++) {
    LED = SECUENCIA_MAQUINA[i];
    digitalWrite(LED, HIGH);
    delay(DELAY_LED);
    apagar_leds();
    delay(DELAY_LED);    
  }
}

bool registrar_secuencia(int p1, int p2, int p3, int p4) {

  bool haySecuencia = (p1 == HIGH || p2 == HIGH || p3 == HIGH || p4 == HIGH) ? true : false;
  String led;

  if (haySecuencia) {
    
    if (p1 == HIGH) { SECUENCIA_JUGADOR[PASO] = LED1; led = "LED1"; }
    else if (p2 == HIGH) { SECUENCIA_JUGADOR[PASO] = LED2; led = "LED2"; }
    else if (p3 == HIGH) { SECUENCIA_JUGADOR[PASO] = LED3; led = "LED3"; }
    else if (p4 == HIGH) { SECUENCIA_JUGADOR[PASO] = LED4; led = "LED4"; }
  
    PASO += 1;
    PASO = PASO % 20;
    Serial.print(" ");
    Serial.print(led);
    
    digitalWrite(LED1, p1);
    digitalWrite(LED2, p2);
    digitalWrite(LED3, p3);
    digitalWrite(LED4, p4);
    delay(300);
    apagar_leds();
    delay(300);
    
  }

  return haySecuencia;
}

bool verificar_Secuencia() {
  for(int i = 0; i < LEVEL; i++) {
    if (SECUENCIA_JUGADOR[i] != SECUENCIA_MAQUINA[i]) return false;
  }
  return true;
}

void mostrar_resultado(bool gamer_win) {

  if (gamer_win) {

    Serial.println();
    Serial.println("GANASTE!");
    
    for(int i = 0; i < 5; i++) {
      establecer_leds(HIGH);
      delay(DELAY_LED_WIN);
      establecer_leds(LOW);
      delay(DELAY_LED_WIN);
    }
    
  } else {

    Serial.println();
    Serial.println("PERDISTE!");

    for(int i = 0; i < 2; i++) {
      establecer_leds(HIGH);
      delay(DELAY_LED_LOST);
      establecer_leds(LOW);
      delay(DELAY_LED_LOST);
    }
    
  } 
}

void setup() {
  Serial.begin(9600);
  
  // put your setup code here, to run once:
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED_GAMER, OUTPUT);

  pinMode(PUSH1, INPUT);
  pinMode(PUSH2, INPUT);
  pinMode(PUSH3, INPUT);
  pinMode(PUSH4, INPUT);

  task_reset_game();
}

void loop() {
  
  // put your main code here, to run repeatedly:
  int p1 = digitalRead(PUSH1);
  int p2 = digitalRead(PUSH2);
  int p3 = digitalRead(PUSH3);
  int p4 = digitalRead(PUSH4);
  int p5 = digitalRead(PUSH_SOLUTION);
  int p6 = digitalRead(PUSH_VERIFY);

  bool pushed1 = isPushed(PUSH1, p1);
  bool pushed2 = isPushed(PUSH2, p2);
  bool pushed3 = isPushed(PUSH3, p3);
  bool pushed4 = isPushed(PUSH4, p4);
  bool solution = isPushed(PUSH_SOLUTION, p5);
  bool verify = isPushed(PUSH_VERIFY, p6);

  
  if (verify) {
    isTurnMachine = true;
    if (isTurnUser) {
      task_verificar();
    }    
  }
  
  if (isTurnMachine) {
    turno_maquina();
  }

  else if (isTurnUser) {
    registrar_secuencia(pushed1, pushed2, pushed3, pushed4);
  }

  if (solution) {
    task_solution();
  }

  digitalWrite(LED_GAMER, isTurnUser ? HIGH : LOW);
  
}

void task_reset_game() {
  LEVEL = 0;
  isTurnMachine = false;
  isTurnUser = false;
  
  apagar_leds();

  for (int i; i < 8; i++) {
    PUSHCACHE[i] = LOW;
  }
}

void task_verificar() {
  isTurnUser = false;
  bool exito = verificar_Secuencia();
  mostrar_resultado(exito);

  if (LEVEL == MAX_SECUENCIAS && exito) {
    mostrar_resultado(exito);
    mostrar_resultado(exito);
  }
  
  if (!exito || LEVEL == MAX_SECUENCIAS) task_reset_game();
  
  delay(2000);
}

void task_solution() {
  mostrar_secuencia();
  mostrar_resultado(false);
  task_reset_game();  
}
