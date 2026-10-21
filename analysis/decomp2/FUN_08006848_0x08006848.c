/* ==== FUN_08006848 @ 0x8006848 ==== */

void FUN_08006848(undefined4 *param_1,int param_2,int param_3)

{
  int iVar1;
  uint uVar2;
  undefined1 *puVar3;
  
  if (param_1 == (undefined4 *)0x0) {
    return;
  }
  iVar1 = -(param_2 + 9U & 0xfffffff8);
  puVar3 = &stack0xffffffe8 + iVar1;
  *puVar3 = (char)*param_1;
  (&stack0xffffffe9)[iVar1] = (char)((uint)*param_1 >> 8);
  uVar2 = param_2 - 1U & 0xff;
  if (param_2 != 0) {
    puVar3[uVar2 + 2] = *(undefined1 *)(param_3 + uVar2);
    uVar2 = uVar2 - 1 & 0xff;
    if (uVar2 != 0xff) {
      if ((param_2 - 1U & 1) != 0) {
        puVar3[uVar2 + 2] = *(undefined1 *)(param_3 + uVar2);
        uVar2 = uVar2 - 1 & 0xff;
        if (uVar2 == 0xff) goto LAB_080068a4;
      }
      do {
        puVar3[uVar2 + 2] = *(undefined1 *)(param_3 + uVar2);
        uVar2 = uVar2 - 1 & 0xff;
        puVar3[uVar2 + 2] = *(undefined1 *)(param_3 + uVar2);
        uVar2 = uVar2 - 1 & 0xff;
      } while (uVar2 != 0xff);
    }
  }
LAB_080068a4:
  FUN_080065DC(0x2b,param_2 + 2U & 0xff,puVar3,0);
  return;
}


