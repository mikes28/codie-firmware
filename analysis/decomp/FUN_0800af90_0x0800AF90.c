/* ==== FUN_0800af90 @ 0x800af90 ==== */

void FUN_0800af90(ushort *param_1,ushort param_2)

{
  *param_1 = *param_1 & 0xfff7;
  *param_1 = param_2 | *param_1;
  return;
}


