#!/bin/bash
# =============================================================================
# RUN OUTBREAK
rm -f data2;
mink=11;
maxk=14;
./EAGLE -v -min $mink -max $maxk -t -i -r HG R1:R2:R3:R4:R5:R6:R7:R8:R9:R10:R11:R12:R13:R14:R15:R16:R17:R18:R19:R20:R21:R22:R23:R24:R25:R26:R27:R28:R29:R30:R31:R32:R33:R34:R35:R36:R37:R38:R39:R40:R41:R42:R43:R44:R45:R46:R47:R48:R49:R50:R51:R52:R53:R54:R55:R56:R57:R58:R59:R60:R61:R62:R63:R64:R65:R66:R67:R68:R69:R70:R71:R72:R73:R74:R75:R76:R77:R78:R79:R80:R81:R82:R83:R84:R85:R86:R87:R88:R89:R90:R91:R92:R93:R94:R95:R96:R97:R98:R99:R100
for((x=$mink ; x<=$maxk ; ++x));
  do
  for((y=1 ; y<=100 ; ++y));
    do
    ./mink $y $x R$y-k$x.eg
    cat R$y-k$x.eg.mink >> data2
    rm -f R$y-k$x.eg.mink ;
    done
  done
./rebat 1 30436 $maxk data2
# =============================================================================
