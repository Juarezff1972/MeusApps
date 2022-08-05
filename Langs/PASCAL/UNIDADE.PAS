unit unidade;

interface
   Procedure TabelaASCII;
implementation

uses dos;

Procedure TabelaASCII;
begin
   swapvectors;
   exec(getenv('COMSPEC'),'/C c:\msdos\mem /c | more');
   swapvectors;
end;

begin
end.