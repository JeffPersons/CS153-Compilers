PROGRAM IfTest1;

VAR
    i, j, t, f : integer;

BEGIN {IF statements}
    i := 3;  j := 4; t := 0; f := 0;

    IF i < j THEN t := 300;

    writeln('t = ', t, ', f = ', f);

    IF i = j THEN t := 200
             ELSE f := -200;

    writeln('t = ', t, ', f = ', f);

    {Cascading IF THEN ELSEs.}
    IF      i = 1 THEN f := 10
    ELSE IF i = 2 THEN f := 20
    ELSE IF i = 3 THEN t := 30
    ELSE IF i = 4 THEN f := 40
    ELSE               f := -1;

    writeln('t = ', t, ', f = ', f);

    {The "dandling ELSE".}
    IF i = 3 THEN IF j = 2 THEN t := 500 ELSE f := -500;

    writeln('t = ', t, ', f = ', f);
END.
