/* ==== FUN_08007830 @ 0x8007830 ==== */

void FUN_08007830(byte *param_1,undefined1 *param_2,int param_3)

{
  byte *pbVar1;
  
  if (param_3 != 0) {
    pbVar1 = param_1 + (param_3 - 1U & 0xff) + 1;
    do {
      if ((*param_1 & 1) == 0) {
        *param_2 = 0x60;
      }
      else {
        *param_2 = 4;
      }
      if ((*param_1 & 2) == 0) {
        param_2[1] = 0x60;
      }
      else {
        param_2[1] = 4;
      }
      if ((*param_1 & 4) == 0) {
        param_2[2] = 0x60;
      }
      else {
        param_2[2] = 4;
      }
      if ((*param_1 & 8) == 0) {
        param_2[3] = 0x60;
      }
      else {
        param_2[3] = 4;
      }
      if ((*param_1 & 0x10) == 0) {
        param_2[4] = 0x60;
      }
      else {
        param_2[4] = 4;
      }
      if ((*param_1 & 0x20) == 0) {
        param_2[5] = 0x60;
      }
      else {
        param_2[5] = 4;
      }
      if ((*param_1 & 0x40) == 0) {
        param_2[6] = 0x60;
      }
      else {
        param_2[6] = 4;
      }
      if ((char)*param_1 < '\0') {
        param_2[7] = 4;
      }
      else {
        param_2[7] = 0x60;
      }
      param_1 = param_1 + 1;
      param_2 = param_2 + 8;
    } while (param_1 != pbVar1);
  }
  return;
}


