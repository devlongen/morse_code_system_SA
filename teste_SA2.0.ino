String alfabeto = ""; 
String codigoMorse = "";
int buttonPin = 2;  // Use o pino correto para o botão
bool isButtonPressed = false;

char charToMorse(String _alfabeto) {
    if (_alfabeto == ".-") {
    return 'a';
  }
  else if (_alfabeto == "-...") {
    return 'b';
  }
  else if (_alfabeto == "-.-.") {
    return 'c';
  }
  else if (_alfabeto == "-..") {
    return 'd';
  }
  else if (_alfabeto == ".") {
    return 'e';
  }
  else if (_alfabeto == "..-.") {
    return 'f';
  }
  else if (_alfabeto == "--.") {
    return 'g';
  }
  else if (_alfabeto == "....") {
    return 'h';
  }
  else if (_alfabeto == "..") {
    return 'i';
  }
  else if (_alfabeto == ".---") {
    return 'j';
  }
  else if (_alfabeto == "-.-") {
    return 'k';
  }
  else if (_alfabeto == ".-..") {
    return 'l';
  }
  else if (_alfabeto == "--") {
    return 'm';
  }
  else if (_alfabeto == "-.") {
    return 'n';
  }
  else if (_alfabeto == "---") {
    return 'o';
  }
  else if (_alfabeto == ".--.") {
    return 'p';
  }
  else if (_alfabeto == "--.-") {
    return 'q';
  }
  else if (_alfabeto == ".-.") {
    return 'r';
  }
  else if (_alfabeto == "...") {
    return 's';
  }
  else if (_alfabeto == "-") {
    return 't';
  }
  else if (_alfabeto == "..-") {
    return 'u';
  }
  else if (_alfabeto == "...-") {
    return 'v';
  }
  else if (_alfabeto == ".--") {
    return 'w';
  }
  else if (_alfabeto == "-..-") {
    return 'x';
  }
  else if (_alfabeto == "-.--") {
    return 'y';
  }
  else if (_alfabeto == "--..") {
    return 'z';
  }
}

void setup() {
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(buttonPin) && !isButtonPressed) {
    delay(200);  // Debounce delay
    isButtonPressed = true;
    codigoMorse += ".";
    Serial.println("Button Pressed test");
  }
  
  if (!digitalRead(buttonPin) && isButtonPressed) {
    delay(200);  // Debounce delay
    isButtonPressed = false;
    codigoMorse += "-";
    Serial.println("Button Released test");
  }
  
  // TESTE PARA VER SE O CÓDIGO DIGITA E TRADUZ ALGO
  if (codigoMorse.length() > 0) {
    char letra = charToMorse(codigoMorse);
    if (letra != ' ') {
      alfabeto += letra;
    }
    codigoMorse = "";
  }
  /
  // Outras lógicas e ações com base em "alfabeto" aqui, se necessário.
}
