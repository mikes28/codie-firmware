/* ==== FUN_0800B3A8 @ 0x800b3a8 ==== */

bool FUN_0800B3A8(int param_1,ushort param_2)

{
  return (*(ushort *)(param_1 + 0x10) & param_2) != 0 && (*(ushort *)(param_1 + 0xc) & param_2) != 0
  ;
}


