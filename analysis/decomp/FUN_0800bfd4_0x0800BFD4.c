/* ==== FUN_0800bfd4 @ 0x800bfd4 ==== */

// WARNING: Control flow encountered bad instruction data

void FUN_0800bfd4(int param_1,undefined4 param_2,int param_3)

{
  uint uVar1;
  uint unaff_r5;
  undefined4 unaff_r7;
  undefined4 unaff_r10;
  undefined4 unaff_r11;
  bool in_ZR;
  
  if (in_ZR) {
    uVar1 = unaff_r5 & param_1 << 6 & param_3 >> 0x20;
    *(BADSPACEBASE **)uVar1 = register0x00000054;
    *(undefined4 *)(uVar1 - 4) = unaff_r11;
    *(undefined4 *)(uVar1 - 8) = unaff_r10;
    *(undefined4 *)(uVar1 - 0xc) = unaff_r7;
    *(uint *)(uVar1 - 0x10) = unaff_r5;
    *(int *)(uVar1 - 0x14) = param_3;
    *(uint *)(uVar1 - 0x18) = uVar1;
  }
                    // WARNING: Bad instruction - Truncating control flow here
  halt_baddata();
}


