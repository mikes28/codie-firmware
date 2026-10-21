/* ==== FUN_0800A6A0 @ 0x800a6a0 ==== */

void FUN_0800A6A0(int param_1,uint *param_2)

{
  *(uint *)(param_1 + 0xc) =
       *(uint *)(param_1 + 0xc) & 0xffffd203 |
       param_2[2] | *param_2 | param_2[3] | param_2[4] | param_2[5] | (uint)(byte)param_2[1] << 0xd;
  return;
}


