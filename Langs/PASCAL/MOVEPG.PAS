     _dest,_orig         : word;
{-------------------------------------------------------------------}
Procedure MovePagina;
begin
   inline($b9/$80/$3e/               { mov cx,3E80  }
          $33/$ff/                   { xor di,di    }
          $8b/$16/>_dest/            { mov dx,_DEST }
          $8e/$c2/                   { mov es,dx    }
          $33/$f6/                   { xor si,si    }
          $8b/$16/>_orig/            { mov dx,_ORIG }
          $1e/                       { push ds      }
          $8e/$da/                   { mov ds,dx    }
          $d1/$e9/                   { shr cx,1     }
          $f3/$a5/                   { rep movsw    }
          $1f);                      { pop ds       }

          { Pagina 0 : $b800 }  { Pagina 8 : $a400 }
          { Pagina 1 : $8800 }  { Pagina 9 : $a800 }
          { Pagina 2 : $8c00 }  { Pagina 10: $ac00 }
          { Pagina 3 : $9000 }  { Pagina 11: $b000 }
          { Pagina 4 : $9400 }  { Pagina 12: $b400 }
          { Pagina 5 : $9800 }
          { Pagina 6 : $9c00 }
          { Pagina 7 : $a000 }
end;