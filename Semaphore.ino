/* I2C用セマフォ！
 * 使用後は必ず解放すること！！！
 */

/* セマフォ変数（グローバル変数） */
int i2cSemaphore = 1;

/* V命令：資源解法 */
void i2cSemaphoreV(){
  i2cSemaphore = 1;
}

/* P命令：資源獲得 */
boolean i2cSemaphoreP(){
  if(i2cSemaphore == 1){
    i2cSemaphore--; /* デクリメント */
    return 1; /* 資源獲得成功 */
  }else{
    return 0; /* 資源獲得失敗（使用中） */
  }
}

