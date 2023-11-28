//* Bibliotecas e configurações
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // Definir o endereço, carcateres[16] e quantidades de linha[2] do Display

//* Definição de portas
int const botaoUm = 2;     // para cliques de morse e navegar para cima
int const botaoDois = 3;   // para confirmar
int const botaoTres = 4;   // para cancelar e voltar telas
int const botaoQuatro = 5; // para navegar para baixo
int const portaLed = 6;
int const portaBuzzer = 7;

//* Variáveis genéricas
bool pressBotaoUm, pressBotaoDois, pressBotaoTres, pressBotaoQuatro = false; // último estado do botão (usado para detectar cliques)
bool mandandoMsg = false;                                                    // enquanto verdairo, permite a escrita duma mensagem
float tempoPress, comecoPress = 0;
int idTela, idContato, indexMenu = 0;

//* Variáveis de mensagem
String msgMorse, mensagemTraduzida, msgTemporaria = "";
// intervalos para escrita e escuta para digitar e expressar mensagem respectivamente
int const intervaloEscritaCurto = 333;
int const intervaloEscritaLongo = 3 * intervaloEscritaCurto;
int const intervaloEscutaCurto = 250;
int const intervaloEscutaLongo = 3 * intervaloEscutaCurto;
int const intervaloEscutaBarra = 7 * intervaloEscutaCurto;

void setup()
{
    pinMode(INPUT, botaoUm);
    pinMode(INPUT, botaoDois);
    pinMode(INPUT, botaoTres);
    pinMode(INPUT, botaoQuatro);
    pinMode(OUTPUT, portaLed);
    pinMode(OUTPUT, portaBuzzer);
    lcd.begin(); // INICIALIZAÇÃO DO DISPLAY(LCD)
    Serial.begin(9600);
}

void navegarTela()
{
    if (idTela == 0)
    {
        telaContato();
    }
    else if (idTela == 1)
    {
        telaComandos();
    }
    else if (idTela == 2)
    {
        telaEnvio();
    }
    else if (idTela == 3)
    {
        telaListaMensagem();
    }
    else if (idTela == 4)
    {
        telaSenha();
    }
    else if (idTela == 5)
    {
        telaMensagem();
    }
    else
    {
        // tela de erro
    }
}

void telaContato() // todo falta inserir lista de contato (i.e qual o index maximo)
{
    if (pressBotaoUm != digitalRead(botaoUm)) // subir na tela
    {
        pressBotaoUm = !pressBotaoUm;
        if (digitalRead(botaoUm) && indexMenu != 0)
        {
            indexMenu--;
        }
    }

    if (pressBotaoQuatro != digitalRead(botaoQuatro)) // descer na tela
    {
        pressBotaoQuatro = !pressBotaoQuatro;
        if (digitalRead(botaoQuatro))
        {
            indexMenu++;
        }
    }

    if (pressBotaoDois != digitalRead(botaoDois)) // selecionar contato
    {
        pressBotaoDois = !pressBotaoDois;
        if (digitalRead(botaoDois))
        {
            idContato = indexMenu;
            indexMenu = 0;
            telaComandos();
        }
    }
}

void telaComandos() // todo identificar qual contato foi selecionado
{
    if (pressBotaoUm != digitalRead(botaoUm)) // subir na tela
    {
        pressBotaoUm = !pressBotaoUm;
        if (digitalRead(botaoUm) && indexMenu != 0)
        {
            indexMenu--;
        }
    }

    if (pressBotaoQuatro != digitalRead(botaoQuatro)) // descer na tela
    {
        pressBotaoQuatro = !pressBotaoQuatro;
        if (digitalRead(botaoQuatro))
        {
            indexMenu++;
        }
    }

    if (pressBotaoDois != digitalRead(botaoDois)) // selecionar contato
    {
        pressBotaoDois = !pressBotaoDois;
        if (digitalRead(botaoDois))
        {
            if (indexMenu = 0)
            {
                telaEnvio();
            }
            else
            {
                telaListaMensagem();
            }
        }
    }

    if (pressBotaoTres != digitalRead(botaoTres))
    {
        pressBotaoTres = !pressBotaoTres;
        if (digitalRead(botaoTres))
        {
            indexMenu = 0;
            idTela--;
        }
    }
}
void telaEnvio()
{
    escreveMorse();

    confirmaEnvio();

    if (pressBotaoTres != digitalRead(botaoTres))
    {
        pressBotaoTres = !pressBotaoTres;
        if (digitalRead(botaoTres))
        {
            msgMorse, mensagemTraduzida, msgTemporaria = "";
            indexMenu = 0;
            idTela--;
        }
    }
}
void telaListaMensagem()
{
    if (pressBotaoUm != digitalRead(botaoUm)) // subir na tela
    {
        pressBotaoUm = !pressBotaoUm;
        if (digitalRead(botaoUm) && indexMenu != 0)
        {
            indexMenu--;
        }
    }

    if (pressBotaoQuatro != digitalRead(botaoQuatro)) // descer na tela
    {
        pressBotaoQuatro = !pressBotaoQuatro;
        if (digitalRead(botaoQuatro))
        {
            indexMenu++;
        }
    }

    if (pressBotaoTres != digitalRead(botaoTres))
    {
        pressBotaoTres = !pressBotaoTres;
        if (digitalRead(botaoTres))
        {
            indexMenu = 0;
            idTela = 1;
        }
    }
}
void telaSenha()
{
    if (pressBotaoTres != digitalRead(botaoTres) && digitalRead(botaoTres))
    {
        idTela--;
    }
}
void telaMensagem()
{
    if (pressBotaoTres != digitalRead(botaoTres))
    {
        pressBotaoTres = !pressBotaoTres;
        if (digitalRead(botaoTres))
        {
            idTela = 3;
        }
    }
}

String separaFraseMorse() // remove e retorna uma "palavra-morse" duma mensagem maior
{
    // esta função irá retornar uma "palavra-morse" (ex .--.) ou a barra (espaço no alfanumérico)
    // e ira remover a mesma da var msgTemporaria
    // como são espaços e barras que separam as palavras, ela primeiro busca seu index na string
    // logo, vê qual está mais próxima de 0 (exceto -1), removendo e retornando o trecho de 0 ao index

    int const indefinido = -1;
    int indexEspaco = msgTemporaria.indexOf(" ");
    int indexBarra = msgTemporaria.indexOf("/");
    String trechoMorse = "";

    if (indexEspaco = indefinido) // não há espaços
    {
        if (indexBarra = indefinido) // não há barras
        {
            trechoMorse = msgTemporaria;
            msgTemporaria = "";
        }
        else // há barra mas não espaços
        {
            if (indexBarra = 0) // barra é o primeiro caractere
            {
                indexBarra = 1; // irá retornar a barra
            }
            trechoMorse = msgTemporaria.substring(0, indexBarra);
            msgTemporaria = msgTemporaria.substring(indexBarra, msgTemporaria.length());
        }
    }
    else // há espaço
    {
        if (indexBarra = indefinido) // há espaço mas não barras
        {
            trechoMorse = msgTemporaria.substring(0, indexEspaco);
            msgTemporaria = msgTemporaria.substring(indexEspaco + 1, msgTemporaria.length());
        }
        else // há barra e espaço
        {
            if (indexEspaco < indexBarra) // espaço vem antes da barra
            {
                trechoMorse = msgTemporaria.substring(0, indexEspaco);
                msgTemporaria = msgTemporaria.substring(indexEspaco + 1, msgTemporaria.length());
            }
            else // barra vem antes do espaço
            {
                if (indexBarra = 0) // barra é o primeiro caractere
                {
                    indexBarra = 1; // irá retornar a barra
                }
                trechoMorse = msgTemporaria.substring(0, indexBarra);
                msgTemporaria = msgTemporaria.substring(indexBarra, msgTemporaria.length());
            }
        }
    }

    return trechoMorse;
}

void limpaMensagemMorse() // remove espaços e/ou barra no final da var msgMorse
{
    int const indexUltimoChar = msgMorse.length() - 1;
    int const indexUltimoEspaco = msgMorse.lastIndexOf(" ");
    int const indexUltimaBarra = msgMorse.lastIndexOf("/");
    if (indexUltimoEspaco == indexUltimoChar || indexUltimaBarra == indexUltimoEspaco)
    {
        msgMorse = msgMorse.substring(0, indexUltimoChar);
    }
}

void escreveMorse() // chamada quando inicia-se a escrita de uma mensagem morse
{
    if (digitalRead(botaoUm))
    {
        tone(portaBuzzer, 294);
    }
    else
    {
        noTone(portaBuzzer);
    }

    if (pressBotaoUm != digitalRead(botaoUm)) // detecta mudanças do estado do botao (foi pressionado ou não)
    {
        if (comecoPress != 0) // botao foi clicado ao menos uma vez *antes*
        {
            tempoPress = millis() - comecoPress; // define duração do clique
            addMorseEmTexto(bitPraMorse());
        }
        else // botao clicado pela primeira vez
        {
            mandandoMsg = true;
        }
        pressBotaoUm = !pressBotaoUm;
        comecoPress = millis();
    }
}

void confirmaEnvio() // sempre sendo chamada, esta envia a mensagem escrita em morse
{
    if (pressBotaoDois != digitalRead(botaoDois))
    {
        pressBotaoDois = !pressBotaoDois;

        if (digitalRead(botaoDois))
        {
            limpaMensagemMorse();
            msgTemporaria = msgMorse;
            do
            {
                mensagemTraduzida += traduzPalavraMorse(separaFraseMorse());
            } while (msgTemporaria != "");

            // envio para o node aqui

            espressaMensagem(portaBuzzer, msgMorse);
        }
    }
}

String bitPraMorse() // converte cliques de botão em caracteres morse (retorna)
{
    if (pressBotaoUm) // pulsos ativos ( '.' e '-' )
    {
        if (tempoPress < 40) // impede problemas de contato com botões de """boa qualidade"""
        {
            return "";
        }
        else if (tempoPress < intervaloEscritaCurto)
        {
            // tone(portaBuzzer, 294, 100); // Buzzer(som), sua frequência e seu tempo de duração no .
            return ".";
        }
        else
        {
            // tone(portaBuzzer, 277, 200); // Buzzer(som), sua frequência e seu tempo de duração no -
            return "-";
        }
    }
    else // pulsos vazios (espaço entre caracteres, palavras e fim de frase)
    {
        if (tempoPress < intervaloEscritaCurto)
        {
            return ""; // Espaço entre pulsos
        }
        else if (tempoPress < intervaloEscritaLongo)
        {
            return " "; // Espaço entre letras (ainda em morse)
        }
        else
        {
            return "/"; // Espaço entre palavras
        }
    }
}

void addMorseEmTexto(String morseChar) // adiciona um caractere morse numa mensagem morse (argumento é a função bitPraMorse())
{
    msgMorse += morseChar;
    Serial.println(msgMorse); // SAÍDA DE DADOS PARA O SERIAL MODE;
    // lcd.print(msgMorse);      // SAÍDA DE DADOS PARA O DISPLAT LCD;
}

String traduzPalavraMorse(String palavraMorse) // traduz uma "palavra-morse" pra caractere alfanumérico (argumento é a função separaFraseMorse())
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
    else if (palavraMorse == ".----")
    {
        return "1";
    }
    else if (palavraMorse == "..---")
    {
        return "2";
    }
    else if (palavraMorse == "...--")
    {
        return "3";
    }
    else if (palavraMorse == "....-")
    {
        return "4";
    }
    else if (palavraMorse == ".....")
    {
        return "5";
    }
    else if (palavraMorse == "-....")
    {
        return "6";
    }
    else if (palavraMorse == "--...")
    {
        return "7";
    }
    else if (palavraMorse == "---..")
    {
        return "8";
    }
    else if (palavraMorse == "----.")
    {
        return "9";
    }
    else if (palavraMorse == "-----")
    {
        return "0";
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

void espressaMensagem(int _porta, String _msgMorse) // expressa mensagem tanto em LED quanto buzzer (de acordo com porta)
{
    digitalWrite(_porta, 0);
    noTone(_porta);

    while (_msgMorse != "")
    {
        String charAtual = _msgMorse.substring(0, 1);
        _msgMorse = _msgMorse.substring(1, _msgMorse.length());

        if (charAtual == ".")
        {
            digitalWrite(portaLed, 1);
            tone(_porta, 294);
            delay(intervaloEscutaCurto);
        }
        else if (charAtual == "-")
        {
            digitalWrite(portaLed, 1);
            tone(_porta, 294);
            delay(intervaloEscutaLongo);
        }
        else if (charAtual == " ")
        {
            digitalWrite(portaLed, 0);
            noTone(_porta);
            delay(intervaloEscutaCurto);
        }
        else if (charAtual == "/")
        {
            digitalWrite(portaLed, 0);
            noTone(_porta);
            delay(intervaloEscutaBarra);
        }

        digitalWrite(portaLed, 0);
        noTone(_porta);
        delay(intervaloEscutaCurto);
    }
}

void loop()
{
    escreveMorse();
    confirmaEnvio();

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
}
