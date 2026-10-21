/* ==== Reset_Handler @ 0x8002904 ==== */

// WARNING: This function may have set the stack pointer

void Reset_Handler(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  for (iVar1 = 0; (undefined1 *)(iVar1 + 0x20000000) < &DAT_20000f04; iVar1 = iVar1 + 4) {
    *(undefined4 *)(iVar1 + 0x20000000) = *(undefined4 *)((int)&DAT_0800c720 + iVar1);
  }
  for (puVar2 = (undefined4 *)&DAT_20000f08; puVar2 < (undefined4 *)0x20001030; puVar2 = puVar2 + 1)
  {
    *puVar2 = 0;
  }
  FUN_08002958();
  FUN_0800B6B0();
  FUN_08009FA4();
  do {
                    // WARNING: Do nothing block with infinite loop
  } while( true );
}


