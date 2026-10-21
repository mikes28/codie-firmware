/* ==== FUN_08006828 @ 0x8006828 ==== */

void FUN_08006828(int param_1,undefined1 param_2)

{
  undefined1 local_c;
  undefined1 local_b;
  undefined1 local_a;
  
  if (param_1 != 0) {
    local_c = (undefined1)param_1;
    local_b = (undefined1)((uint)param_1 >> 8);
    local_a = param_2;
    FUN_080065DC(0x21,3,&local_c,0);
  }
  return;
}


