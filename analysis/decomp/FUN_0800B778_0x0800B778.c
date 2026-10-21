/* ==== FUN_0800B778 @ 0x800b778 ==== */

char * FUN_0800B778(char *param_1,char *param_2,undefined4 *param_3,int param_4)

{
  char cVar1;
  char *pcVar2;
  char *pcVar3;
  char *pcVar4;
  
  if ((param_1 == (char *)0x0) && (param_1 = (char *)*param_3, (char *)*param_3 == (char *)0x0)) {
    pcVar2 = (char *)0x0;
  }
  else {
    do {
      pcVar2 = param_1;
      param_1 = pcVar2 + 1;
      pcVar3 = param_2;
      do {
        cVar1 = *pcVar3;
        pcVar3 = pcVar3 + 1;
        if (cVar1 == '\0') {
          if (*pcVar2 == '\0') {
            *param_3 = 0;
            return (char *)0x0;
          }
          do {
            pcVar4 = param_1;
            param_1 = pcVar4 + 1;
            pcVar3 = param_2;
            do {
              cVar1 = *pcVar3;
              pcVar3 = pcVar3 + 1;
              if (*pcVar4 == cVar1) {
                if (*pcVar4 == '\0') {
                  param_1 = (char *)0x0;
                }
                else {
                  *pcVar4 = '\0';
                }
                *param_3 = param_1;
                return pcVar2;
              }
            } while (cVar1 != '\0');
          } while( true );
        }
      } while (*pcVar2 != cVar1);
    } while (param_4 != 0);
    *param_3 = param_1;
    *pcVar2 = '\0';
  }
  return pcVar2;
}


