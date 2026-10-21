/* ==== FUN_08005848 @ 0x8005848 ==== */

void FUN_08005848(char *param_1,char *param_2)

{
  char cVar1;
  byte bVar2;
  char cVar3;
  char cVar4;
  char cVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  
  uVar7 = (uint)(byte)param_1[1];
  if (uVar7 == 0) {
    param_2[1] = param_1[2];
    *param_2 = param_1[2];
    param_2[2] = param_1[2];
  }
  else {
    cVar1 = *param_1;
    cVar5 = FUN_08000128(cVar1,0x2b);
    bVar2 = param_1[2];
    uVar6 = (uint)bVar2;
    uVar8 = (uint)(byte)((cVar1 + cVar5 * -0x2b) * '\x06');
    cVar4 = (char)(uVar6 * (0xff - ((int)((0xff - uVar8) * uVar7) >> 8)) >> 8);
    cVar1 = (char)(uVar6 * (0xff - uVar7) >> 8);
    cVar3 = (char)(uVar6 * (0xff - ((int)(uVar7 * uVar8) >> 8)) >> 8);
    switch(cVar5) {
    case '\0':
      param_2[1] = bVar2;
      *param_2 = cVar4;
      param_2[2] = cVar1;
      break;
    case '\x01':
      param_2[1] = cVar3;
      *param_2 = param_1[2];
      param_2[2] = cVar1;
      break;
    case '\x02':
      param_2[1] = cVar1;
      *param_2 = param_1[2];
      param_2[2] = cVar4;
      break;
    case '\x03':
      param_2[1] = cVar1;
      *param_2 = cVar3;
      param_2[2] = param_1[2];
      break;
    case '\x04':
      param_2[1] = cVar4;
      *param_2 = cVar1;
      param_2[2] = param_1[2];
      break;
    default:
      param_2[1] = bVar2;
      *param_2 = cVar1;
      param_2[2] = cVar3;
    }
  }
  return;
}


