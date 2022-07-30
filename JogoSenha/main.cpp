#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
    char senha[4];
    char tentativa[5];
    int i;
    int valor;
    char errou;

    system("chcp 1252  > nul");

    for(i=0; i<4; i++)
    {
        senha[i]=0;
        tentativa[i]=0;
    }
    cout<<"Qual a senha de quatro digitos (ex: 0246 ou 1234) a ser descoberta?";
    cin>> senha;
    valor=atoi(senha);
    if(valor==0 || valor > 10000)
    {
        cout<<"Voce informou uma senha errada, o jogo acabou!";
        return 0;
    }
    for(i=0; i<4; i++)
    {
        if(senha[i]<'0' || senha[i]>'9')
        {
            cout<<"Voce informou uma senha errada, o jogo acabou!";
            return 0;
        }
    }
    system("cls");
    errou=0;
    while(errou!=4)
    {
        cout<<"Qual a sua tentativa de quatro digitos?";
        cin>> tentativa;
        errou=0;
        if(senha[0]==tentativa[0])
        {
            cout<<"Você acertou o milhar\n";
            errou++;
        }
        if(senha[1]==tentativa[1])
        {
            cout<<"Você acertou a centena\n";
            errou++;
        }
        if(senha[2]==tentativa[2])
        {
            cout<<"Você acertou a dezena\n";
            errou++;
        }
        if(senha[3]==tentativa[3])
        {
            cout<<"Você acertou a unidade\n";
            errou++;
        }
        if(errou==0)
            cout<<"Você errou tudo\n";
    }
    if(errou==4)
    {
        cout<<"Voce acertou!";
        return 0;
    }
}
