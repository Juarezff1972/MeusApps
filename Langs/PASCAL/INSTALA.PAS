{$M 8192,0,0}
uses dos;
var f : file;
begin
   if (paramstr(1)<>'a:')and(paramstr(1)<>'A:')
    and(paramstr(1)<>'b:')and(paramstr(1)<>'B:') then halt(0);
   chdir('c:\');
   mkdir('Agenda');
   chdir('agenda');
   swapvectors;
   exec(getenv('COMSPEC'),'/c copy '+paramstr(1)+'\*.*');
   swapvectors;
   mkdir('Pacientes');
   assign(f,'pacientes');
   setfattr(f,hidden);
   assign(f,'medicos.agn');
   setfattr(f,hidden);
   assign(f,'agenda.cfg');
   setfattr(f,hidden);
end.
