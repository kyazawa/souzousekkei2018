/* 【BLE関連処理】 */


/* ③BLE(内蔵) */
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t value = 0;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

/* bleSetup : BLEの初期化設定・BLE接続処理 */
void bleSetup(){
  Serial.println("begin BLE setup.");
  // Create the BLE Device
  BLEDevice::init("HeartRateMeter");
  delay(100);
  Serial.println("BLE initing.");
  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);
  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());
  // Start the service
  Serial.println("BLE starting.");
  pService->start();
  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("BLE advertising.");
}

/* sendBleMsg: BLEでメッセージ送信 */
void sendBleMsg(char * str){
  int msgLength;
  msgLength = strlen(str);

  /* BLEメッセージ送信処理 */
  if (deviceConnected) {
        pCharacteristic->setValue((uint8_t *)str, msgLength); //charじゃなくてuint8_tにしないと怒られる
        pCharacteristic->notify();
        value++;
        delay(10); // bluetooth stack will go into congestion, if too many packets are sent
  }
}

/* BLEﾒｯｾｰｼﾞ受信処理 */
void recvBleMsg(char * str){
  /* なんらかの受信処理！ */
  sprintf(str, "mojiretsu"); 
}

