/* ==== FUN_080080AC @ 0x80080ac ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_080080AC(undefined2 *param_1)

{
  byte bVar1;
  undefined1 uVar2;
  undefined1 uVar3;
  int iVar4;
  uint local_20;
  int local_1c;
  undefined1 auStack_18 [8];
  
  FUN_0800A4DC(auStack_18);
  while( true ) {
    disableIRQinterrupts();
    enableIRQinterrupts();
    if (_DAT_20000622 == 0) {
      return 0;
    }
    FUN_0800A500(auStack_18,&local_20);
    if ((local_1c != 0) || (10000 < local_20)) break;
    uVar3 = FUN_08002E44(&DAT_2000061c);
    *(undefined1 *)param_1 = *(undefined1 *)((int)param_1 + 1);
    bVar1 = *(byte *)(param_1 + 1);
    *(byte *)((int)param_1 + 1) = bVar1;
    uVar2 = *(undefined1 *)((int)param_1 + 3);
    *(undefined1 *)(param_1 + 1) = uVar2;
    *(undefined1 *)((int)param_1 + 3) = uVar3;
    if ((bVar1 < 0x81) &&
       (iVar4 = FUN_08002A60(CONCAT13(*(undefined1 *)((int)param_1 + 3),CONCAT12(uVar2,*param_1))),
       iVar4 != 0)) {
      return 1;
    }
  }
  return 0;
}


