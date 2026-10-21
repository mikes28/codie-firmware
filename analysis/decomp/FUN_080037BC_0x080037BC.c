/* ==== FUN_080037BC @ 0x80037bc ==== */

void FUN_080037BC(undefined4 param_1,undefined2 *param_2)

{
  char cVar1;
  undefined4 uVar2;
  undefined2 local_14;
  char local_12;
  char local_11;
  
  FUN_08003040();
  cVar1 = *(char *)(param_2 + 1);
  if (cVar1 < 0) {
    local_11 = FUN_080001c4(cVar1 * 0x2d,100);
    local_11 = local_11 + -0x37;
  }
  else {
    local_11 = FUN_080001c4(cVar1 * 0x2d,100);
    local_11 = local_11 + '7';
  }
  local_12 = -local_11;
  uVar2 = FUN_080012F0(*param_2);
  uVar2 = FUN_08000818(uVar2,0x42b40000);
  uVar2 = FUN_08000BFC(uVar2,0x40490fdb);
  uVar2 = FUN_08000BFC(uVar2,0x42140000);
  FUN_08000BFC(uVar2,0x3f8ccccd);
  local_14 = FUN_08000380();
  FUN_080036F8(param_1,&local_14);
  return;
}


