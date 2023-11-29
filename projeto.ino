// TODO 1. Definir o 'indexMaxContatos' pelo número de contatos na tabela do banco de dados
// linha 56
// TODO 2. Enviar os dados da 'confirmaEnvio()' pro node (e consequentemente banco de dados)
// linha 606
// TODO 3. Definir o 'indexMaxMensagens' pelo número de mensagens (de acordo com contato)
// linha 169
// TODO 4.1. Exibir as mensagens dum contato a partir do 'idMensagem' no lcd
// linha 381
// TODO 4.2. Exibir as mensagens dum contato a partir do 'idMensagem' no buzzer e led
// linha 388 e 397
// TODO 5. Mostrar o nome de cada contato na tela de contatos
// linha 88

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
float tempoPress, comecoPress = 0;
int idTela, idContato, idMensagem = 0;
int indexMenu, indexMaxContatos, indexMaxMensagens = 0;
bool lockEscrita = true; // impede que mensagem seja escrita *imediatamente* depois de entrar na tela de envio
int indexMenuMensagem = 1;

//* Variáveis de mensagem
String msgMorse, msgTraduzida, msgTemporaria = "";
// intervalos para escrita e escuta para digitar e expressar mensagem respectivamente
int const intervaloEscritaCurto = 500;
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

    // ? código para determinar o 'indexMaxContatos'
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
}

void telaContato()
{
    lcd.clear();
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
        if (digitalRead(botaoQuatro) && indexMenu != indexMaxContatos)
        {
            indexMenu++;
        }
    }

    if (pressBotaoTres != digitalRead(botaoTres))
    {
        pressBotaoTres = !pressBotaoTres;
    }

    if (pressBotaoDois != digitalRead(botaoDois)) // selecionar contato
    {
        pressBotaoDois = !pressBotaoDois;
        if (digitalRead(botaoDois))
        {
            idContato = indexMenu;
            indexMenu = 0;
            idTela++;
        }
    }
}

void telaComandos()
{
    lcd.clear();
    if (indexMenu == 0)
    {
        // lcd.println("ENVIAR MSG.   <-");
        // lcd.println("VER MENSAGENS   ");
        lcd.print("ENVIAR MSG.   <-VER MENSAGENS   ");
        delay(50);
    }
    else
    {
        // lcd.println("ENVIAR MSG.     ");
        // lcd.println("VER MENSAGENS <-");
        lcd.print("ENVIAR MSG.     VER MENSAGENS <-");
        delay(50);
    }

    if (pressBotaoUm != digitalRead(botaoUm)) // subir na tela
    {
        pressBotaoUm = !pressBotaoUm;
        if (digitalRead(botaoUm) && indexMenu == 1)
        {
            indexMenu--;
        }
    }

    if (pressBotaoQuatro != digitalRead(botaoQuatro)) // descer na tela
    {
        pressBotaoQuatro = !pressBotaoQuatro;
        if (digitalRead(botaoQuatro) && indexMenu == 0)
        {
            indexMenu++;
        }
    }

    if (pressBotaoDois != digitalRead(botaoDois)) // selecionar contato
    {
        pressBotaoDois = !pressBotaoDois;
        if (digitalRead(botaoDois))
        {
            if (indexMenu == 0)
            {
                idTela++;
            }
            else
            {
                indexMenu = 0;
                // ? código para determinar o 'indexMaxMensagens' através do 'idContato'
                idTela = 3;
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
    lcd.clear();
    // lcd.println("ESCREVA SUA MSG ");
    // lcd.println("EM MORSE        ");
    lcd.print("ESCREVA SUA MSG ");
    lcd.print("EM MORSE        ");

    if (!lockEscrita)
    {
        escreveMorse();
    }

    if (pressBotaoDois != digitalRead(botaoDois))
    {
        pressBotaoDois = !pressBotaoDois;

        if (digitalRead(botaoDois))
        {
            confirmaEnvio();
        }
    }

    if (pressBotaoTres != digitalRead(botaoTres))
    {
        pressBotaoTres = !pressBotaoTres;

        if (digitalRead(botaoTres))
        {
            lockEscrita = true;
            noTone(portaBuzzer); // validação somente
            msgMorse = "";
            indexMenu = 0;
            comecoPress = 0;
            idTela--;
        }
    }

    if (!digitalRead(botaoUm))
    {
        lockEscrita = false;
    }
}

String exibirItemMensagem(int indexMsg, bool selecionada = false)
{
    String num = String(indexMsg);
    int numEspaco = 13 - num.length();
    String trechoMsg = num + ".";

    for (int i = 0; i < numEspaco; i++)
    {
        trechoMsg += " ";
    }

    if (selecionada)
    {
        trechoMsg += "<-";
    }
    else
    {
        trechoMsg += "  ";
    }
    return trechoMsg;
}

void telaListaMensagem()
{
    lcd.clear();
    if (indexMaxMensagens = 0)
    {
        // lcd.println("NÃO HÁ MENSAGENS");
        // lcd.println("COM ESTE CONTATO");
        lcd.print("NÃO HÁ MENSAGENSCOM ESTE CONTATO");
    }
    else if (indexMaxMensagens = 1)
    {
        // lcd.println("1.            <-");
        lcd.print("1.            <-");
    }
    else
    {
        if (indexMenu == indexMenuMensagem - 1)
        {
            // lcd.println(exibirItemMensagem(indexMenu, true));
            // lcd.println(exibirItemMensagem(indexMenuMensagem));
            lcd.print(exibirItemMensagem(indexMenu, true) + exibirItemMensagem(indexMenuMensagem));
        }
        else
        {
            // lcd.println(exibirItemMensagem(indexMenu - 1));
            // lcd.println(exibirItemMensagem(indexMenu, true));
            lcd.print(exibirItemMensagem(indexMenu - 1) + exibirItemMensagem(indexMenu, true));
        }
    }

    if (pressBotaoUm != digitalRead(botaoUm)) // subir na tela
    {
        pressBotaoUm = !pressBotaoUm;
        if (digitalRead(botaoUm) && indexMenu != 0)
        {
            indexMenu--;
            if (indexMenu == indexMenuMensagem - 2)
            {
                indexMenuMensagem--;
            }
        }
    }

    if (pressBotaoQuatro != digitalRead(botaoQuatro)) // descer na tela
    {
        pressBotaoQuatro = !pressBotaoQuatro;
        if (digitalRead(botaoQuatro) && indexMenu != indexMaxMensagens)
        {
            indexMenu++;
            if (indexMenu > indexMenuMensagem)
            {
                indexMenuMensagem++;
            }
        }
    }

    if (pressBotaoDois != digitalRead(botaoDois))
    {
        pressBotaoDois = !pressBotaoDois;

        if (digitalRead(botaoDois))
        {
            idMensagem = indexMenu;
            idTela++;
        }
    }

    if (pressBotaoTres != digitalRead(botaoTres))
    {
        pressBotaoTres = !pressBotaoTres;
        if (digitalRead(botaoTres))
        {
            indexMenu = 0;
            idTela = 1;
            indexMenuMensagem = 1;
        }
    }
}

void telaSenha()
{
    lcd.clear();
    lcd.println("ESCREVA O CODIGO");
    lcd.println("DE SEGURANÇA    ");
    // lcd.print("ESCREVA O CÓDIGODE SEGURANÇA    ");

    if (!lockEscrita)
    {
        escreveMorse();
    }

    if (pressBotaoDois != digitalRead(botaoDois))
    {
        pressBotaoDois = !pressBotaoDois;

        if (digitalRead(botaoDois))
        {
            lockEscrita = true;
            noTone(portaBuzzer); // validação somente
            limpaMensagemMorse();

            if (msgMorse == "... . -. .... .-")
            {
                idTela++;
            }
            else
            {
                lcd.clear();
                lcd.println("CÓDIGO DE SEGU- ");
                lcd.println("RANÇA INCORRETO ");
                // lcd.print("CÓDIGO DE SEGU- RANÇA INCORRETO ");
                delay(4000);
                msgMorse = "";
            }
        }
    }

    if (pressBotaoTres != digitalRead(botaoTres))
    {
        pressBotaoTres = !pressBotaoTres;
        if (digitalRead(botaoTres))
        {
            lockEscrita = true;
            noTone(portaBuzzer); // validação somente
            msgMorse = "";
            comecoPress = 0;
            idTela--;
        }
    }

    if (!digitalRead(botaoUm))
    {
        lockEscrita = false;
    }
}

void telaMensagem()
{
    lcd.clear();
    // String mensagem = Serial.read();
    // String mensagemDestraduzida = destraduzirMensagem(mensagem);
    // Serial.print(mensagem);

    if (pressBotaoUm != digitalRead(botaoUm))
    {
        pressBotaoUm = !pressBotaoUm;
        if (digitalRead(botaoUm))
        {
            // espressaMensagem(portaLed, mensagemDestraduzida);
        }
    }

    if (pressBotaoDois != digitalRead(botaoDois))
    {
        pressBotaoDois = !pressBotaoDois;
        if (digitalRead(botaoDois))
        {
            // espressaMensagem(portaBuzzer, mensagemDestraduzida);
        }
    }

    if (pressBotaoTres != digitalRead(botaoTres))
    {
        pressBotaoTres = !pressBotaoTres;
        if (digitalRead(botaoTres))
        {
            idTela = 3;
        }
    }
}

String destraduzirDigito(String digitoAlfanumerico)
{
    if (digitoAlfanumerico == "a")
        return ".-";
    else if (digitoAlfanumerico == "b")
        return "-...";
    else if (digitoAlfanumerico == "c")
        return "-.-.";
    else if (digitoAlfanumerico == "d")
        return "-..";
    else if (digitoAlfanumerico == "e")
        return ".";
    else if (digitoAlfanumerico == "f")
        return "..-.";
    else if (digitoAlfanumerico == "g")
        return "--.";
    else if (digitoAlfanumerico == "h")
        return "....";
    else if (digitoAlfanumerico == "i")
        return "..";
    else if (digitoAlfanumerico == "j")
        return ".---";
    else if (digitoAlfanumerico == "k")
        return "-.-";
    else if (digitoAlfanumerico == "l")
        return ".-..";
    else if (digitoAlfanumerico == "m")
        return "--";
    else if (digitoAlfanumerico == "n")
        return "-.";
    else if (digitoAlfanumerico == "o")
        return "---";
    else if (digitoAlfanumerico == "p")
        return ".--.";
    else if (digitoAlfanumerico == "q")
        return "--.-";
    else if (digitoAlfanumerico == "r")
        return ".-.";
    else if (digitoAlfanumerico == "s")
        return "...";
    else if (digitoAlfanumerico == "t")
        return "-";
    else if (digitoAlfanumerico == "u")
        return "..-";
    else if (digitoAlfanumerico == "v")
        return "...-";
    else if (digitoAlfanumerico == "w")
        return ".--";
    else if (digitoAlfanumerico == "x")
        return "-..-";
    else if (digitoAlfanumerico == "y")
        return "-.--";
    else if (digitoAlfanumerico == "z")
        return "--..";
    else if (digitoAlfanumerico == "0")
        return "-----";
    else if (digitoAlfanumerico == "1")
        return ".----";
    else if (digitoAlfanumerico == "2")
        return "..---";
    else if (digitoAlfanumerico == "3")
        return "...--";
    else if (digitoAlfanumerico == "4")
        return "....-";
    else if (digitoAlfanumerico == "5")
        return ".....";
    else if (digitoAlfanumerico == "6")
        return "-....";
    else if (digitoAlfanumerico == "7")
        return "--...";
    else if (digitoAlfanumerico == "8")
        return "---..";
    else if (digitoAlfanumerico == "9")
        return "----.";
}

String destraduzirMensagem(String mensagem)
{
    String msgDestraduzida = "";

    for (int i = 0; i < mensagem.length(); i++)
    {
        String trechoMensagem = mensagem.substring(i, i + 1);
        msgDestraduzida += destraduzirDigito(trechoMensagem);
    }

    return msgDestraduzida;
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
        if (comecoPress != 0) // botao já foi clicado ao menos uma vez *antes*
        {
            tempoPress = millis() - comecoPress; // define duração do clique
            addMorseEmTexto(bitPraMorse());
        }
        pressBotaoUm = !pressBotaoUm;
        comecoPress = millis();
    }
}

void confirmaEnvio() // sempre sendo chamada, esta envia a mensagem escrita em morse
{
    lockEscrita = true;
    noTone(portaBuzzer); // validação somente

    if (msgMorse != "")
    {
        limpaMensagemMorse();
        msgTemporaria = msgMorse;
        do
        {
            msgTraduzida += traduzPalavraMorse(separaFraseMorse());
        } while (msgTemporaria != "");

        // ? código para enviar mensagem pro node e banco (não sei se o trecho abaixo funciona)
        Serial.println(msgTraduzida);
        Serial.println(idContato);

        comecoPress = 0;
        msgMorse = "";
        msgTraduzida = "";

        // lcd.println("MENSAGEM ENVIA- ");
        // lcd.println("DA COM SUCESSO  ");
        lcd.print("MENSAGEM ENVIA- DA COM SUCESSO  ");
        delay(4000);
    }
    else
    {
        // lcd.println("MENSAGENS VAZIAS");
        // lcd.println("SÃO INVALIDAS   ");
        lcd.print("MENSAGENS VAZIASSÃO INVALIDAS   ");
        delay(4000);
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
        return "a";
    else if (palavraMorse == "-...")
        return "b";
    else if (palavraMorse == "-.-.")
        return "c";
    else if (palavraMorse == "-..")
        return "d";
    else if (palavraMorse == ".")
        return "e";
    else if (palavraMorse == "..-.")
        return "f";
    else if (palavraMorse == "--.")
        return "g";
    else if (palavraMorse == "....")
        return "h";
    else if (palavraMorse == "..")
        return "i";
    else if (palavraMorse == ".---")
        return "j";
    else if (palavraMorse == "-.-")
        return "k";
    else if (palavraMorse == ".-..")
        return "l";
    else if (palavraMorse == "--")
        return "m";
    else if (palavraMorse == "-.")
        return "n";
    else if (palavraMorse == "---")
        return "o";
    else if (palavraMorse == ".--.")
        return "p";
    else if (palavraMorse == "--.-")
        return "q";
    else if (palavraMorse == ".-.")
        return "r";
    else if (palavraMorse == "...")
        return "s";
    else if (palavraMorse == "-")
        return "t";
    else if (palavraMorse == "..-")
        return "u";
    else if (palavraMorse == "...-")
        return "v";
    else if (palavraMorse == ".--")
        return "w";
    else if (palavraMorse == "-..-")
        return "x";
    else if (palavraMorse == "-.--")
        return "y";
    else if (palavraMorse == "--..")
        return "z";
    else if (palavraMorse == ".----")
        return "1";
    else if (palavraMorse == "..---")
        return "2";
    else if (palavraMorse == "...--")
        return "3";
    else if (palavraMorse == "....-")
        return "4";
    else if (palavraMorse == ".....")
        return "5";
    else if (palavraMorse == "-....")
        return "6";
    else if (palavraMorse == "--...")
        return "7";
    else if (palavraMorse == "---..")
        return "8";
    else if (palavraMorse == "----.")
        return "9";
    else if (palavraMorse == "-----")
        return "0";
    else if (palavraMorse == "/")
        return " ";
    else
        return "?";
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
    navegarTela();

    // Serial.println("idTela = " + String(idTela));
    // Serial.println("idContato = " + String(idContato));
    // Serial.println("idMensagem = " + String(idMensagem));
    // Serial.println("indexMenu = " + String(indexMenu));
    // Serial.println("indexMaxContatos = " + String(indexMaxContatos));
    // Serial.println("indexMaxMensagens = " + String(indexMaxMensagens));
    // Serial.println("indexMenuMensagem = " + String(indexMenuMensagem));
    Serial.println("msgMorse = " + String(msgMorse));
    // Serial.println("msgTraduzida = " + String(msgTraduzida));

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
