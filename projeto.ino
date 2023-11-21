// Bibliotecas
// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>

// Definir o endereço, carcateres[16] e quantidades de linha[2] do Display
// LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definição de portas
int const botao = 2;
int const led = 3;
int const envio = 4;
int const portaBuzzer = 7;

// Variáveis genéricas
bool botaoPress = false;  // último estado do botão (usado para detectar cliques)
bool mandandoMsg = false; // enquanto verdairo, permite a escrita duma mensagem
float tempoPress, comecoPress = 0;

// Variáveis de mensagem
String msgMorse, mensagemTraduzida, msgTemporaria = "";

void setup()
{
    pinMode(INPUT, botao);
    pinMode(OUTPUT, led);
    pinMode(INPUT, envio);
    pinMode(OUTPUT, portaBuzzer);

    Serial.begin(9600);
}

String separaFraseMorse() // retorna e *retira* uma "palavra-morse" duma mensagem maior
{
    // define onde está o primeiro caractere espaço e barra
    // retorna o trecho que será traduzido por uma outra função (de acordo com a posição do espaço e barra)
    // retira o mesmo da msgTemporaria

    int const indefinido = -1;
    int indexEspaco = msgTemporaria.indexOf(" ");
    int indexBarra = msgTemporaria.indexOf("/");
    String trechoMorse;

    if (indexEspaco = indefinido)
    {
        if (indexBarra = indefinido)
        {
            trechoMorse = msgTemporaria;
            msgTemporaria = "";
            return trechoMorse;
        }
        else
        {
            if (indexBarra = 0)
            {
                indexBarra = 1;
            }
            trechoMorse = msgTemporaria.substring(0, indexBarra);
            msgTemporaria = msgTemporaria.substring(indexBarra, msgTemporaria.length());
            return trechoMorse;
        }
    }
    else
    {
        if (indexBarra = indefinido)
        {
            trechoMorse = msgTemporaria.substring(0, indexEspaco);
            msgTemporaria = msgTemporaria.substring(indexEspaco + 1, msgTemporaria.length());
            return trechoMorse;
        }
        else
        {
            if (indexEspaco < indexBarra)
            {
                trechoMorse = msgTemporaria.substring(0, indexEspaco);
                msgTemporaria = msgTemporaria.substring(indexEspaco + 1, msgTemporaria.length());
                return trechoMorse;
            }
            else
            {
                if (indexBarra = 0)
                {
                    indexBarra = 1;
                }
                trechoMorse = msgTemporaria.substring(0, indexBarra);
                msgTemporaria = msgTemporaria.substring(indexBarra, msgTemporaria.length());
                return trechoMorse;
            }
        }
    }
}

String bitPraMorse() // converte cliques em caracteres morse
{
    if (botaoPress)
    { // pulsos ativos ( '.' e '-' )
        if (tempoPress < 500)
        {
           tone(portaBuzzer, 294, 100); // Buzzer(som), sua frequência e seu tempo de duração no .
            return ".";
        }
        else
        {
          tone(portaBuzzer, 277, 200); // Buzzer(som), sua frequência e seu tempo de duração no - 
            return "-";
        }
    }
    else
    { // pulsos vazios (espaço entre caracteres, palavras e fim de frase)
        if (tempoPress < 500)
        {
            return ""; // Espaço entre pulsos
        }
        else if (tempoPress < 1000)
        {
            return " "; // Espaço entre letras (ainda em morse)
        }
        else
        {
            // mandandoMsg = false;
            // comecoPress = 0;
            // msg = "";
            return "/"; // Espaço entre palavras
        }
    }
}

void addMorseEmTexto(String morseChar)
{ // adiciona um caractere morse numa mensagem morse (argumento é a função bitPraMorse() )
    msgMorse += morseChar;
    Serial.println(msgMorse);
}

String traduzPalavraMorse(String palavraMorse) // traduz uma "palavra-morse" pra caractere alfanumérico
{
    if (palavraMorse == ".-")
    {
        return "a";
    }
    else if (palavraMorse == "-...")
    {
        return "b";
    }
    else if (palavraMorse == "-.-.")
    {
        return "c";
    }
    else if (palavraMorse == "-..")
    {
        return "d";
    }
    else if (palavraMorse == ".")
    {
        return "e";
    }
    else if (palavraMorse == "..-.")
    {
        return "f";
    }
    else if (palavraMorse == "--.")
    {
        return "g";
    }
    else if (palavraMorse == "....")
    {
        return "h";
    }
    else if (palavraMorse == "..")
    {
        return "i";
    }
    else if (palavraMorse == ".---")
    {
        return "j";
    }
    else if (palavraMorse == "-.-")
    {
        return "k";
    }
    else if (palavraMorse == ".-..")
    {
        return "l";
    }
    else if (palavraMorse == "--")
    {
        return "m";
    }
    else if (palavraMorse == "-.")
    {
        return "n";
    }
    else if (palavraMorse == "---")
    {
        return "o";
    }
    else if (palavraMorse == ".--.")
    {
        return "p";
    }
    else if (palavraMorse == "--.-")
    {
        return "q";
    }
    else if (palavraMorse == ".-.")
    {
        return "r";
    }
    else if (palavraMorse == "...")
    {
        return "s";
    }
    else if (palavraMorse == "-")
    {
        return "t";
    }
    else if (palavraMorse == "..-")
    {
        return "u";
    }
    else if (palavraMorse == "...-")
    {
        return "v";
    }
    else if (palavraMorse == ".--")
    {
        return "w";
    }
    else if (palavraMorse == "-..-")
    {
        return "x";
    }
    else if (palavraMorse == "-.--")
    {
        return "y";
    }
    else if (palavraMorse == "--..")
    {
        return "z";
    }
    else if (palavraMorse == "/")
    {
        return " ";
    }
    else
    {
        return "?";
    }
}

void morseEmLed(int portaLed, String _msgMorse)
{ // converte qualquer mensagem morse em pulsos num LED
    digitalWrite(portaLed, 0);

    while (_msgMorse != "")
    {

        String charAtual = _msgMorse.substring(0, 1);
        _msgMorse = _msgMorse.substring(1, _msgMorse.length());

        if (charAtual == ".")
        {
            digitalWrite(portaLed, 1);
            delay(333);
        }
        else if (charAtual == "-")
        {
            digitalWrite(portaLed, 1);
            delay(1000);
        }
        else if (charAtual == " ")
        {
            digitalWrite(portaLed, 0);
            delay(333);
        }
        else if (charAtual == "/")
        {
            digitalWrite(portaLed, 0);
            delay(1000);
        }
    }
}

void morseEmBuzzer(int portaBuzzer, String _msgMorse)
{ // converte qualquer mensagem morse em pulsos num LED
    noTone(portaBuzzer);

    while (_msgMorse != "")
    {

        String charAtual = _msgMorse.substring(0, 1);
        _msgMorse = _msgMorse.substring(1, _msgMorse.length());

        if (charAtual == ".")
        {
            tone(portaBuzzer, 294);
            delay(333);
        }
        else if (charAtual == "-")
        {
            tone(portaBuzzer, 294);
            delay(1000);
        }
        else if (charAtual == " ")
        {
            noTone(portaBuzzer);
            delay(333);
        }
        else if (charAtual == "/")
        {
            noTone(portaBuzzer);
            delay(1000);
        }
    }
}

void loop()
{
    if (botaoPress != digitalRead(botao)) // detecta mudanças do estado do botao (foi pressionado ou não)
    {
        if (comecoPress != 0) // botao foi clicado ao menos uma vez *antes*
        {
            tempoPress = millis() - comecoPress;
            addMorseEmTexto(bitPraMorse());
        }
        else // botao clicado pela primeira vez
        {
            mandandoMsg = true;
        }
        botaoPress = !botaoPress;
        comecoPress = millis();
    }

    // if (digitalRead(2)) {
    //   digitalWrite(3, 1);
    // } else {
    //   digitalWrite(3, 0);
    // }

    // Mostrar alguma informação do Display
    // lcd.print("O que você quiser colocar");

    // ao clicar num botão secundário:
    // - traduz do morse para alfanumérico
    // - envia para o node-red
    if (digitalRead(envio))
    {
        msgTemporaria = msgMorse;
        do
        {
            mensagemTraduzida += traduzPalavraMorse(separaFraseMorse());
        } while (msgTemporaria != "");

        // envio para o node aqui
    }
}
