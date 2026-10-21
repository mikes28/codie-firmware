/* ==== FUN_0800693C @ 0x800693c ==== */

char * FUN_0800693C(int param_1,char *param_2,int param_3)

{
  char *pcVar1;
  char cVar2;
  uint extraout_r1;
  char *pcVar3;
  char *pcVar4;
  
  *param_2 = '\0';
  if (param_3 - 2U < 0x23) {
    pcVar1 = param_2;
    pcVar4 = param_2;
    if (param_1 < 0) {
      *param_2 = '-';
      pcVar4 = param_2 + 1;
      param_1 = -param_1;
      pcVar1 = pcVar4;
    }
    do {
      pcVar3 = pcVar1;
      FUN_080001b0(param_1,param_3);
      if (extraout_r1 < 10) {
        cVar2 = (char)extraout_r1 + '0';
      }
      else {
        cVar2 = (char)extraout_r1 + 'W';
      }
      *pcVar3 = cVar2;
      param_1 = FUN_08000128(param_1,param_3);
      pcVar1 = pcVar3 + 1;
    } while (param_1 != 0);
    pcVar3[1] = '\0';
    if (pcVar4 < pcVar3) {
      do {
        cVar2 = *pcVar4;
        *pcVar4 = *pcVar3;
        *pcVar3 = cVar2;
        pcVar4 = pcVar4 + 1;
        pcVar3 = pcVar3 + -1;
      } while (pcVar4 < pcVar3);
    }
  }
  return param_2;
}


