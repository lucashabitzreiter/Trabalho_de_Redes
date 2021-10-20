#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <bitset>
#include <sstream>
#include <string>
#include <fstream>

#define TIMEOUT_TIME 30

    /*CODICO SIMULANDO O PROTOCOLO Rtp 3.0:
    Codico desenvolvido por:
    ALEXANDRE BALBINOT
    BRUNO GRUTTNER
    LUCAS HABITZREITER
    LUAN NUNES
    GABRIEL BEHLING
    */

using namespace std;


char app_payload[5][8];
int delay = 0, erro = 0, flag_seqnum = 0,flag_acknum=0,timeout=0;
clock_t tempo_i;

typedef struct {
    int seqnum;
    int acknum;
    char checksum[8];
    char payload[5][8];
}rtp_packet_t;

rtp_packet_t pacote_Alice;
rtp_packet_t pacote_Bob;

//--------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
void time_interrupt(){
    clock_t tempo_f = clock();
    clock_t tics = tempo_f - tempo_i;
    double tempo_e= tics/(double)CLOCKS_PER_SEC;
    if(tempo_e >= TIMEOUT_TIME){
        cout<<"TIMEOUT"<<endl<<endl;
        timeout=1;
    }
}
void checksum() {
    //Calcula o checksum
    char a[8], b[8], complemento[8], soma[8], carry = '0';
    for (int k = 0; k < 4; k++) {
        for (int j = 7; j >= 0; j--) {
            if (k == 0) {
                a[j] = app_payload[k][j];
                b[j] = app_payload[k + 1][j];
            }
            else {
                b[j] = app_payload[k + 1][j];
            }
        }
        for (int i = 7; i >= 0; i--)
        {
            if (a[i] == '0' && b[i] == '0' && carry == '0')
            {
                soma[i] = '0';
                carry = '0';
            }
            else if (a[i] == '0' && b[i] == '0' && carry == '1')
            {
                soma[i] = '1';
                carry = '0';
            }
            else if (a[i] == '0' && b[i] == '1' && carry == '0')
            {
                soma[i] = '1';
                carry = '0';
            }
            else if (a[i] == '0' && b[i] == '1' && carry == '1')
            {
                soma[i] = '0';
                carry = '1';
            }
            else if (a[i] == '1' && b[i] == '0' && carry == '0')
            {
                soma[i] = '1';
                carry = '0';
            }
            else if (a[i] == '1' && b[i] == '0' && carry == '1')
            {
                soma[i] = '0';
                carry = '1';
            }
            else if (a[i] == '1' && b[i] == '1' && carry == '0')
            {
                soma[i] = '0';
                carry = '1';
            }
            else if (a[i] == '1' && b[i] == '1' && carry == '1')
            {
                soma[i] = '1';
                carry = '1';
            }
            else
                break;
        }
        if (carry == '1') {
            for (int i = 7; i >= 0; i--) {
                if (carry == '1' && soma[i] == '1') {
                    soma[i] = '0';
                    carry = '1';
                }
                else if (carry == '1' && soma[i] == '0') {
                    soma[i] = '1';
                    carry = '0';
                    break;
                }
            }
        }
        for (int c = 7; c >= 0; c--) {
            a[c] = soma[c];
        }
    }
    for (int i = 0; i <= 7; i++)
    {
        if (soma[i] == '0') {
            complemento[i] = '1';
            pacote_Alice.checksum[i] = '1';
        }
        else {
            complemento[i] = '0';
            pacote_Alice.checksum[i] = '0';
        }

    }
}
//--------------------------------------------------------------------------------------------------------------
int Acknum() {
    //verifica checksum e retorna acknum
    char a[8], b[8], carry = '0', soma[8], checksum[8];
    int ACK = 0;
    for (int i = 7; i >= 0; i--) {
        checksum[i] = pacote_Bob.checksum[i];

    }
    for (int k = 0; k < 4; k++) {
        for (int j = 7; j >= 0; j--) {
            if (k == 0) {
                a[j] = app_payload[k][j];
                b[j] = app_payload[k + 1][j];
            }
            else {
                b[j] = app_payload[k + 1][j];
            }
        }
        for (int i = 7; i >= 0; i--) {
            if (a[i] == '0' && b[i] == '0' && carry == '0')
            {
                soma[i] = '0';
                carry = '0';
            }
            else if (a[i] == '0' && b[i] == '0' && carry == '1')
            {
                soma[i] = '1';
                carry = '0';
            }
            else if (a[i] == '0' && b[i] == '1' && carry == '0')
            {
                soma[i] = '1';
                carry = '0';
            }
            else if (a[i] == '0' && b[i] == '1' && carry == '1')
            {
                soma[i] = '0';
                carry = '1';
            }
            else if (a[i] == '1' && b[i] == '0' && carry == '0')
            {
                soma[i] = '1';
                carry = '0';
            }
            else if (a[i] == '1' && b[i] == '0' && carry == '1')
            {
                soma[i] = '0';
                carry = '1';
            }
            else if (a[i] == '1' && b[i] == '1' && carry == '0')
            {
                soma[i] = '0';
                carry = '1';
            }
            else if (a[i] == '1' && b[i] == '1' && carry == '1')
            {
                soma[i] = '1';
                carry = '1';
            }
            else
            break;

        }
        if (carry == '1') {
            for (int i = 7; i >= 0; i--) {
                if (carry == '1' && soma[i] == '1') {
                    soma[i] = '0';
                    carry = '1';
                }
                else if (carry == '1' && soma[i] == '0') {
                    soma[i] = '1';
                    carry = '0';
                    break;
                }
            }
        }
        for (int c = 7; c >= 0; c--) {
            a[c] = soma[c];
        }
    }
    for (int i = 7; i >= 0; i--) {
        if (soma[i] == '1' && pacote_Alice.checksum[i] == '0')
        {
            soma[i] = '1';
        }
        else if (soma[i] == '0' && pacote_Alice.checksum[i] == '1')
        {
            soma[i] = '1';
        }
        else {
            return ACK;
            break;
        }
    }
    return ACK = 1;
}
//------------------------------------------------------------------------------------------------------------------------
void transport_layer_input_A();
void transport_layer_input_B();
//camada de comunicaçao
void communication_network() {
        time_interrupt();
        if(flag_acknum == 1){
            pacote_Alice.acknum = pacote_Bob.acknum;
            transport_layer_input_A();
        }else{
        int altera_pacote = rand() % 10;
        cout << "Numero randomico gerado: ";
        cout << altera_pacote << endl;
        if (altera_pacote == 1) {
            pacote_Alice.checksum[4] = 'x';//pacote corrompido
        }
        else if (altera_pacote == 2) {
            delay = 1;
            cout << "Ocorreu um atraso na estrega do pacote, aguarde" << endl;
            Sleep(2000);// atraso
        }
        else if (altera_pacote == 3) {
            erro = 1;//impossibilita de ser enviado
            cout << "Sera feito o reenvio" << endl;
            return;
        }
        time_interrupt();
        if (altera_pacote != 3) {
            pacote_Bob.seqnum = pacote_Alice.seqnum;
            for (int i = 7; i >= 0; i--) {
                pacote_Bob.checksum[i] = pacote_Alice.checksum[i];
            }
            for (int i = 4; i >= 0; i--) {
                for (int j = 7; j >= 0; j--) {
                    pacote_Bob.payload[i][j] = pacote_Alice.payload[i][j];
                }
            }
            transport_layer_input_B();
        }
        }
    }


//--------------------------------------------------------------------------------------------------
void transport_layer_output_A();
void app_layer_B();
//camada que recebe resposta de Bob
void transport_layer_input_A() {
    flag_acknum=0;
    time_interrupt();
    cout<<"Envio feito com sucesso"<<endl<<endl;
    app_layer_B();
}
//camada que recebe informaçoes de Alice
void transport_layer_input_B() {
    enum estado_fsm_t { s1, s2, s3 } estados_op;
    estados_op = s1;
     time_interrupt();
    while (estados_op != s3 && pacote_Alice.acknum == 0  && timeout == 0) {
        time_interrupt();
        switch (estados_op) {
        case s1: //código do estado
            pacote_Bob.acknum = Acknum();
            cout<<"Valor de ACK de Bob: ";
            cout << pacote_Bob.acknum<<endl<<endl;
            //system("pause");
            if (pacote_Bob.acknum == 1) {
                estados_op = s2;
            }
            else {
                estados_op = s3;
            }
            break;
        case s2:
            flag_acknum=1;
            communication_network();
            break;
        default:
            break;
        }
    }
    if (estados_op == s3) {
        cout << "Problema no envio, sera refeito";
        system("pause");
    }
}
//camada que prepara a mensagem e faz o envio para a camada de comunicaçao
void transport_layer_output_A() {

    // cout << "trnspt layer comeco a\n";

    enum estado_fsm_t { Wait_for_call_0, Wait_for_AN, Wait_for_call_1 } estados_op;
    estados_op = Wait_for_call_0;
     time_interrupt();
    while (pacote_Alice.acknum == 0 && timeout == 0 ) {
       time_interrupt();
        switch (estados_op) {
        case Wait_for_call_0: //código do estado
           //  cout << "estado 1 comeco a\n";
            pacote_Alice.seqnum = flag_seqnum;
            checksum();
            for (int i = 4; i >= 0; i--) {
                for (int j = 7; j >= 0; j--) {
                    pacote_Alice.payload[i][j] = app_payload[i][j];
                }
            }
            //  cout << "ESTADO 1 FIM a\n";
            estados_op = Wait_for_AN;
            break;
        case Wait_for_AN: //código do estado
            communication_network();
            // cout<<" estado 2 ini\n";
            if (erro = 1) {
                erro = 0;
                estados_op = Wait_for_AN;
                checksum();
            }
            else {
                estados_op = Wait_for_call_1;
            }
            break;
        case Wait_for_call_1:
            pacote_Alice.acknum = 0;
            estados_op = Wait_for_call_0;
            erro = 0;
            cout<<"estado 3\n";
            break;
        default:
            break;
        }
    }
}

//camada que inicializa as variaveis
void transport_layer_init() {

    pacote_Alice.seqnum = 0;
    pacote_Alice.acknum = 0;
    pacote_Alice.checksum[0] = 0;
    pacote_Alice.checksum[1] = 0;
    pacote_Alice.checksum[2] = 0;
    pacote_Alice.checksum[3] = 0;
    pacote_Alice.checksum[4] = 0;
    pacote_Alice.checksum[5] = 0;
    pacote_Alice.checksum[6] = 0;
    pacote_Alice.checksum[7] = 0;
    pacote_Bob.seqnum = 0;
    pacote_Bob.acknum = 0;
    pacote_Bob.checksum[0] = 0;
    pacote_Bob.checksum[1] = 0;
    pacote_Bob.checksum[2] = 0;
    pacote_Bob.checksum[3] = 0;
    pacote_Bob.checksum[4] = 0;
    pacote_Bob.checksum[5] = 0;
    pacote_Bob.checksum[6] = 0;
    pacote_Bob.checksum[7] = 0;

    transport_layer_output_A();
}

//camada de aplicaçao de Alice
void app_layer_A() {
    transport_layer_init();
}
void fun_app_receive() {
    char mensagem2[8] = {'0','0','0','0','0','0','0','0'};
    string mensagem;
    for(int i=0;i<5;i++){
        for(int j=0;j<8;j++ ){
            mensagem2[j]=app_payload[i][j];
        }
        char c = strtol(mensagem2, 0, 2);
        mensagem+=c;
    }
    cout<<"A mensagem recebida foi: ";
    cout<<mensagem<<endl;
    cout<<endl;
}
//camada de aplicaçao de Bob
void app_layer_B() {
    fun_app_receive();
}
int main() {

    //-----------------------------------------------------------------------------------------------------------------------------//
    srand(time(NULL));
    bool mandar_mensg = true;
    int opcao;
    int num_mensg = 1;
    bitset<8> mybits;
    string mensagem ="00000000";
    tempo_i = clock();
    while (mandar_mensg) {
        string myString;
    cout<<endl<<"digite a mensagem a ser enviada a bob: ";
    cin>>myString;
    cout<<endl<<"Mensagem em Binario: "<<endl;
  for (int i = 0; i < 5; i++)
  {
        mybits = bitset<8>(myString.c_str()[i]);
        mensagem= mybits.to_string<char,string::traits_type,string::allocator_type>();
                for(int j=0;j<8;j++){
            app_payload[i][j]= mensagem[j];
            cout<<"   "<<app_payload[i][j];
                }
    cout<<endl;
  }
    cout<<endl;
        if (num_mensg % 2 == 0) {
            flag_seqnum = 1;
        }
        else {
            flag_seqnum = 0;
        }
        app_layer_A();
        if(timeout == 0){
        cout << "DESEJA MANDAR OUTRA MENSAGEM? DIGITE 1 PARA SIM, 0 PARA NAO: ";
        cin >> opcao;
        if (opcao == 1) {
            mandar_mensg = true;
            num_mensg++;
        }
        else
            mandar_mensg = false;

       }else{
           cout<<"Deseja reconectar? 1 para SIM ou 0 para NAO";
           cin>>opcao;
           if(opcao == 1){
                timeout = 0;
                tempo_i = clock();
           }else{
                mandar_mensg = false;
           }
           }

    }


    return 0;
}
