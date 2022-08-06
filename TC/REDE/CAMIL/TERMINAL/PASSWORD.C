// Pasword de configuracao do TERMINAL e CENTRAL
// Andrey Patitucci
// Carlos E. Cruz
// Lisianne Requiao
// Luciane E. Romeiro
// Raquel Speltz
// Nenhum direito reservado - Copywrong - 1993

// A mensagem "FUNCAO: ...." especifica o que faz a funcao onde a mensagem
// estiver.

int CmpPassword(unsigned long passw);

int CmpPassword(unsigned long passw)
{

// FUNCAO: verifica se passw e a senha correta, se for a correta, retorna
// 1, senao retorna 0.
// ----------------------------------------------------------------------

  unsigned long senha=2340208;

  if(senha==passw)
    return 1;
  else
    return 0;
}