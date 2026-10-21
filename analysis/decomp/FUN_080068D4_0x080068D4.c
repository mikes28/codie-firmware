/* ==== FUN_080068D4 @ 0x80068d4 ==== */

char * FUN_080068D4(int param_1,char *param_2,int param_3)

{
  uint extraout_r1;
  char *pcVar1;
  char *pcVar2;
  char cVar3;
  
  *param_2 = '\0';
  pcVar2 = param_2;
  if (param_3 - 2U < 0x23) {
    do {
      pcVar1 = pcVar2;
      FUN_080001b0(param_1,param_3);
      if (extraout_r1 < 10) {
        cVar3 = (char)extraout_r1 + '0';
      }
      else {
        cVar3 = (char)extraout_r1 + 'W';
      }
      *pcVar1 = cVar3;
      param_1 = FUN_08000128(param_1,param_3);
      pcVar2 = pcVar1 + 1;
    } while (param_1 != 0);
    pcVar1[1] = '\0';
    pcVar2 = param_2;
    if (param_2 < pcVar1) {
      do {
        cVar3 = *pcVar2;
        *pcVar2 = *pcVar1;
        *pcVar1 = cVar3;
        pcVar2 = pcVar2 + 1;
        pcVar1 = pcVar1 + -1;
      } while (pcVar2 < pcVar1);
    }
  }
  return param_2;
}


