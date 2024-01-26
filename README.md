# cansat-software

## Wirings

### XBEE 

| XBEE      | Pico         |
| --------- | ------------ |
| 3.3V      | (36) 3V3 OUT |
| GND       | (38) GND     |
| DIN (RX)  | (1) GP0      |
| DOUT (TX) | (2) GP1      |
| RST       |              |

**Makeshift workaround for XBEE connection**
**NOTE: To connect both Xbee modules, disconnect RXTX & RESET BOTH XBEES. Wait for GCS XBEE RSSI to turn green, then connect RXTX**

### RTC

| RTC  | Pico         |
| ---- | ------------ |
| 3.3V | (36) 3V3 OUT |
| GND  | (38) GND     |
| SDA  | (6) GP4      |
| SCL  | (7) GP5      |