// Definição de portas
int const botao = 2;
int const led = 3;

// Variáveis genéricas
bool botaoPress = false; // último estado do botão (usado para detectar cliques)
bool mandandoMsg = false; // enquanto verdairo, permite a escrita duma mensagem
float tempoPress, comecoPress = 0;
String codigoMorse, trechoMorse = "";

// Variáveis de mensagem
String msg = "";
String mensagemTraduzida = "";


void setup() {
  pinMode(INPUT, botao);
  pinMode(INPUT, led);
  Serial.begin(9600);
}

void traduzLetraMorse(String caracter = "") {
  int indexCaracter;
  if (caracter != "") {
    indexCaracter = trechoMorse.indexOf(caracter);
  } else {
    indexCaracter = trechoMorse.length();
  }
  mensagemTraduzida += trechoMorse.substring(0,indexCaracter);
}

String bitPraMorse() { // converte cliques em caracteres morse

  if (botaoPress) { // pulsos ativos ( '.' e '-' )
    if (tempoPress < 500) {
      return ".";
    } else {
      return "-";
    }
  } else { // pulsos vazios (espaço entre caracteres, palavras e fim de frase)
    if (tempoPress < 500) {
      return ""; // Espaço entre pulsos
    } else if (tempoPress < 1000) {
      traduzLetraMorse(" ");
      return " "; // Espaço entre letras (ainda em morse)
    } else {
      //mandandoMsg = false;
      //comecoPress = 0;
      //msg = "";
      traduzLetraMorse("/");
      return "/"; // Espaço entre palavras
    }
  }
}

void addMorseEmTexto(String morseChar) { // adiciona um caractere morse numa mensagem morse (argumento é a função bitPraMorse() )
  msg += morseChar;
  Serial.println(msg);
}

String morseToCode(String _alfabeto) {
  if (_alfabeto == ".-") {
    return "a";
  }
  else if (_alfabeto == "-...") {
    return "b";
  }
  else if (_alfabeto == "-.-.") {
    return "c";
  }
  else if (_alfabeto == "-..") {
    return "d";
  }
  else if (_alfabeto == ".") {
    return "e";
  }
  else if (_alfabeto == "..-.") {
    return "f";
  }
  else if (_alfabeto == "--.") {
    return "g";
  }
  else if (_alfabeto == "....") {
    return "h";
  }
  else if (_alfabeto == "..") {
    return "i";
  }
  else if (_alfabeto == ".---") {
    return "j";
  }
  else if (_alfabeto == "-.-") {
    return "k";
  }
  else if (_alfabeto == ".-..") {
    return "l";
  }
  else if (_alfabeto == "--") {
    return "m";
  }
  else if (_alfabeto == "-.") {
    return "n";
  }
  else if (_alfabeto == "---") {
    return "o";
  }
  else if (_alfabeto == ".--.") {
    return "p";
  }
  else if (_alfabeto == "--.-") {
    return "q";
  }
  else if (_alfabeto == ".-.") {
    return "r";
  }
  else if (_alfabeto == "...") {
    return "s";
  }
  else if (_alfabeto == "-") {
    return "t";
  }
  else if (_alfabeto == "..-") {
    return "u";
  }
  else if (_alfabeto == "...-") {
    return "v";
  }
  else if (_alfabeto == ".--") {
    return "w";
  }
  else if (_alfabeto == "-..-") {
    return "x";
  }
  else if (_alfabeto == "-.--") {
    return "y";
  }
  else if (_alfabeto == "--..") {
    return "z";
  }
  else {
    return "?";
  }
};

void loop() {

  if (botaoPress != digitalRead(botao)) { // detecta mudanças do estado do botao (foi pressionado ou não)
    if (comecoPress != 0) { // botao foi clicado ao menos uma vez antes
      tempoPress = millis() - comecoPress;
      trechoMorse += bitPraMorse();
      addMorseEmTexto(bitPraMorse());
    }
    else { // botao clicado pela primeira vez
      mandandoMsg = true;
    }
    botaoPress = !botaoPress;
    comecoPress = millis();
  }



  if (digitalRead(2)) {
    digitalWrite(3, 1);
  } else {
    digitalWrite(3, 0);
  }
}
