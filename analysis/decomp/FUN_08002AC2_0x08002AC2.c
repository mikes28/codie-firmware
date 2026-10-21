/* ==== FUN_08002AC2 @ 0x8002ac2 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08002AC2(void)

{
  uint uVar1;
  undefined1 *puVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint *puVar6;
  undefined1 *puVar7;
  uint uVar8;
  byte in_stack_00000000;
  undefined4 local_34;
  undefined1 local_30 [20];
  
  uVar1 = (uint)in_stack_00000000;
  puVar6 = (uint *)&stack0x00000004;
  uVar3 = *(uint *)(&DAT_0800b93c + uVar1 * 0xc);
  local_34 = *(undefined4 *)(&DAT_0800b938 + uVar1 * 0xc);
  if (uVar3 == 0) {
    uVar4 = 0;
    uVar5 = 0;
  }
  else {
    uVar4 = 0;
    uVar5 = 0;
    puVar2 = local_30;
    do {
      switch(uVar3 & 0xf) {
      case 2:
      case 3:
        *puVar2 = (char)*puVar6;
        puVar2 = puVar2 + 1;
        puVar6 = puVar6 + 1;
        break;
      case 4:
      case 5:
        uVar8 = *puVar6;
        *puVar2 = (char)uVar8;
        puVar2[1] = (char)(uVar8 >> 8);
        puVar2 = puVar2 + 2;
        puVar6 = puVar6 + 1;
        break;
      case 6:
      case 7:
        uVar8 = *puVar6;
        *puVar2 = (char)uVar8;
        puVar2[1] = (char)(uVar8 >> 8);
        puVar2[2] = (char)(uVar8 >> 0x10);
        puVar2[3] = (char)(uVar8 >> 0x18);
        puVar2 = puVar2 + 4;
        puVar6 = puVar6 + 1;
        break;
      case 8:
      case 9:
        uVar4 = *puVar6 & 0xffff;
        *puVar2 = (char)*puVar6;
        puVar2 = puVar2 + 1;
        uVar5 = uVar4 + local_34._1_1_ & 0xffff;
        local_34._0_2_ = CONCAT11((char)uVar5,(byte)local_34);
        local_34 = CONCAT31(local_34._1_3_,(byte)local_34 | (byte)(uVar5 >> 8));
        uVar5 = puVar6[1];
        puVar6 = puVar6 + 2;
        break;
      case 10:
        puVar7 = (undefined1 *)*puVar6;
        *puVar2 = *puVar7;
        puVar2[1] = puVar7[1];
        puVar2[2] = puVar7[2];
        puVar2[3] = puVar7[3];
        puVar2[4] = puVar7[4];
        puVar2[5] = puVar7[5];
        puVar2 = puVar2 + 6;
        puVar6 = puVar6 + 1;
        break;
      case 0xb:
        uVar4 = *puVar6 & 0xffff;
        *puVar2 = (char)*puVar6;
        puVar2[1] = (char)(uVar4 >> 8);
        puVar2 = puVar2 + 2;
        uVar5 = uVar4 + local_34._1_1_ & 0xffff;
        local_34._0_2_ = CONCAT11((char)uVar5,(byte)local_34);
        local_34 = CONCAT31(local_34._1_3_,(byte)local_34 | (byte)(uVar5 >> 8));
        uVar5 = puVar6[1];
        puVar6 = puVar6 + 2;
      }
      uVar3 = uVar3 >> 4;
    } while (uVar3 != 0);
  }
  if (_DAT_20000f24 != (code *)0x0) {
    (*_DAT_20000f24)((&DAT_0800b939)[uVar1 * 0xc] + '\x04',&local_34,uVar4,uVar5);
  }
  return;
}


