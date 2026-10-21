/* ==== FUN_08002ac0 @ 0x8002ac0 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08002ac0(uint param_1,uint param_2,undefined4 param_3,undefined4 param_4)

{
  undefined1 *puVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint *puVar5;
  undefined1 *puVar6;
  uint uVar7;
  undefined4 uStack_44;
  undefined1 auStack_40 [20];
  uint uStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  param_1 = param_1 & 0xff;
  puVar5 = &uStack_c;
  uVar2 = *(uint *)(&DAT_0800b93c + param_1 * 0xc);
  uStack_44 = *(undefined4 *)(&DAT_0800b938 + param_1 * 0xc);
  uStack_c = param_2;
  uStack_8 = param_3;
  uStack_4 = param_4;
  if (uVar2 == 0) {
    uVar3 = 0;
    uVar4 = 0;
  }
  else {
    uVar3 = 0;
    uVar4 = 0;
    puVar1 = auStack_40;
    do {
      switch(uVar2 & 0xf) {
      case 2:
      case 3:
        *puVar1 = (char)*puVar5;
        puVar1 = puVar1 + 1;
        puVar5 = puVar5 + 1;
        break;
      case 4:
      case 5:
        uVar7 = *puVar5;
        *puVar1 = (char)uVar7;
        puVar1[1] = (char)(uVar7 >> 8);
        puVar1 = puVar1 + 2;
        puVar5 = puVar5 + 1;
        break;
      case 6:
      case 7:
        uVar7 = *puVar5;
        *puVar1 = (char)uVar7;
        puVar1[1] = (char)(uVar7 >> 8);
        puVar1[2] = (char)(uVar7 >> 0x10);
        puVar1[3] = (char)(uVar7 >> 0x18);
        puVar1 = puVar1 + 4;
        puVar5 = puVar5 + 1;
        break;
      case 8:
      case 9:
        uVar3 = *puVar5 & 0xffff;
        *puVar1 = (char)*puVar5;
        puVar1 = puVar1 + 1;
        uVar4 = uVar3 + uStack_44._1_1_ & 0xffff;
        uStack_44._0_2_ = CONCAT11((char)uVar4,(byte)uStack_44);
        uStack_44 = CONCAT31(uStack_44._1_3_,(byte)uStack_44 | (byte)(uVar4 >> 8));
        uVar4 = puVar5[1];
        puVar5 = puVar5 + 2;
        break;
      case 10:
        puVar6 = (undefined1 *)*puVar5;
        *puVar1 = *puVar6;
        puVar1[1] = puVar6[1];
        puVar1[2] = puVar6[2];
        puVar1[3] = puVar6[3];
        puVar1[4] = puVar6[4];
        puVar1[5] = puVar6[5];
        puVar1 = puVar1 + 6;
        puVar5 = puVar5 + 1;
        break;
      case 0xb:
        uVar3 = *puVar5 & 0xffff;
        *puVar1 = (char)*puVar5;
        puVar1[1] = (char)(uVar3 >> 8);
        puVar1 = puVar1 + 2;
        uVar4 = uVar3 + uStack_44._1_1_ & 0xffff;
        uStack_44._0_2_ = CONCAT11((char)uVar4,(byte)uStack_44);
        uStack_44 = CONCAT31(uStack_44._1_3_,(byte)uStack_44 | (byte)(uVar4 >> 8));
        uVar4 = puVar5[1];
        puVar5 = puVar5 + 2;
      }
      uVar2 = uVar2 >> 4;
    } while (uVar2 != 0);
  }
  if (_DAT_20000f24 != (code *)0x0) {
    (*_DAT_20000f24)((&DAT_0800b939)[param_1 * 0xc] + '\x04',&uStack_44,uVar3,uVar4);
  }
  return;
}


