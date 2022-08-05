{************************************************}
{                                                }
{ QuickSort Demo                                 }
{ Copyright (c) 1985,90 by Borland International }
{                                                }
{************************************************}

program QSort;
{$R-,S-}
uses Crt;

{ This program demonstrates the quicksort algorithm, which      }
{ provides an extremely efficient method of sorting arrays in   }
{ memory. The program generates a list of 10000 random numbers  }
{ between 0 and 19999, and then sorts them using the QUICKSORT  }
{ procedure. Finally, the sorted list is output on the screen.  }
{ Note that stack and range checks are turned off (through the  }
{ compiler directive above) to optimize execution speed.        }

const
  Max = 10000;

type
  List = array[1..Max] of integer;

var
  Data: List;
  s   : array[1..20000] of integer;
  I: integer;

{ QUICKSORT sorts elements in the array A with indices between  }
{ LO and HI (both inclusive). Note that the QUICKSORT proce-    }
{ dure provides only an "interface" to the program. The actual  }
{ processing takes place in the SORT procedure, which executes  }
{ itself recursively.                                           }
{-----------------------------------------------------------------------}
procedure QuickSort(var A: List; Lo, Hi: integer);

procedure Sort(l, r: integer);
var
  i, j, x, y: integer;
begin
  i := l; j := r; x := a[(l+r) DIV 2];
  repeat
    while a[i] < x do i := i + 1;
    while x < a[j] do j := j - 1;
    if i <= j then
    begin
      y := a[i]; a[i] := a[j]; a[j] := y;
      i := i + 1; j := j - 1;
    end;
  until i > j;
  if l < j then Sort(l, j);
  if i < r then Sort(i, r);
end;

begin {QuickSort};
  Sort(Lo,Hi);
end;
{-----------------------------------------------------------------------}
{ Distribution Counting }
procedure DistrCount(var A: List; Lo, Hi: integer);
var aux : array[1..20000] of integer;
    i,j : integer;
begin
   for i:=Lo to Hi do aux[i]:=0;
   for j:=1 to max do aux[a[j]]:=aux[a[j]]+1;
   for i:=lo+1 to hi do aux[i]:=aux[i]+aux[i-1];
   for j:=max downto 1 do
   begin
      i:=aux[a[j]];
      s[i]:=a[j];
      aux[a[j]]:=i-1;
   end;
   for i:=1 to max do a[i]:=s[i];
end;
{-----------------------------------------------------------------------}
begin {QSort}
  Write('Now generating 10000 random numbers...');
  Randomize;
  for i := 1 to Max do Data[i] := Random(20000);
  Writeln;
  Write('Now sorting random numbers...');
  readln;
  QuickSort(Data, 1, Max);
  Writeln;
  for i := 1 to max do Write(Data[i]:8);
  readln;
  Write('Now generating 10000 random numbers...');
  for i := 1 to Max do Data[i] := Random(20000);
  Writeln;
  Write('Now sorting random numbers...');
  readln;
  DistrCount(Data, 0, 20000);
  Writeln;
  for i := 1 to max do Write(Data[i]:8);
end.
