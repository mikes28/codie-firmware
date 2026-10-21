/* ==== FUN_0800aabc @ 0x800aabc ==== */

bool FUN_0800aabc(int param_1,ushort param_2)

{
  return (*(ushort *)(param_1 + 0x10) & param_2) != 0;
}


