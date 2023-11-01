//Bibliotecas 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//Definir o endereço, carcateres[16] e quantidades de linha[2] do Display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definição de portas
int const botao = 2;
int const led = 3;

// Variáveis genéricas
bool botaoPress = false; // último estado do botão (usado para detectar cliques)
bool mandandoMsg = false; // enquanto verdairo, permite a escrita duma mensagem
float tempoPress, comecoPress = 0;
String codigoMorse = "";

// Variáveis de mensagem
int tamanhoMsg = 1;
char* mensagemMorse = (char*)malloc(tamanhoMsg * sizeof(char)); // declara-se um pointer para uma array
String msg = "";

void setup() {
  //Definição das ENTRADAS e SAÍDAS das portas;
  pinMode(INPUT, botao);
  pinMode(INPUT, led);
  
  //Inicialização do Serial e do Display LCD;
  Serial.begin(9600);
  lcd.begin();
}

char bitPraMorse() { // Converte cliques em caracteres morse;

  if (botaoPress) { // pulsos ativos ( '.' e '-' );
    if (tempoPress < 500) {
      return '.';
    } else {
      return '-';
    }
  } else { // pulsos vazios (espaço entre caracteres, paravras e fim de frase);
    if (tempoPress < 500) {
      return '@'; //Não faça nada;
    } else if (tempoPress < 5000) {
      return ' '; // Espaço
    } else {
      mandandoMsg = false;
      comecoPress = 0;
      msg = "";
      return '@';
    }
  }
}

void addMorseEmTexto(char morseChar) { // adiciona um caractere morse numa mensagem morse (argumento é a função bitPraMorse() )

  if (morseChar != '@') {
    //tamanhoMsg++;
    //char* novaMensagemMorse = (char*)malloc(tamanhoMsg * sizeof(char));
    //char arrayMorseChar[2] = {morseChar, '\0'}; // array provisória (para adicionar um único caracter)
    //strcpy(novaMensagemMorse, mensagemMorse);
    //strcat(novaMensagemMorse, arrayMorseChar);
    //mensagemMorse = novaMensagemMorse; // atualizando o pointer da nova array, não a array em si
    //free(novaMensagemMorse);

    //msg = String(mensagemMorse);
    msg += String(morseChar);
    Serial.println(msg);
  }
};
char morseToCode(String _alfabeto) {
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
};

void loop() {

  if (botaoPress != digitalRead(botao)) { // detecta mudanças do estado do botao (foi pressionado ou não)
    if (comecoPress != 0) { // botao foi clicado ao menos uma vez antes
      tempoPress = millis() - comecoPress;
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
// Mostrar alguma informação do Display
lcd.print("O que você quiser colocar");
}
