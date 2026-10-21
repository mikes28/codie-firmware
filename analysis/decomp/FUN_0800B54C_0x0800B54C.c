/* ==== FUN_0800B54C @ 0x800b54c ==== */

bool FUN_0800B54C(uint *param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  bool bVar3;
  
  uVar1 = (param_2 & 0xffff) >> 8;
  uVar2 = 1 << (param_2 & 0xff);
  if (uVar1 == 1) {
    uVar2 = uVar2 & *param_1;
  }
  else if (uVar1 == 2) {
    uVar2 = uVar2 & param_1[1];
  }
  else {
    uVar2 = uVar2 & param_1[2];
  }
  bVar3 = false;
  if (uVar2 != 0) {
    bVar3 = (1 << (param_2 >> 0x10 & 0xff) & param_1[7]) != 0;
  }
  return bVar3;
}


