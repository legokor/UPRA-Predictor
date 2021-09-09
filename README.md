# UPRA-Predictor

Flight predictor for UPRA high altitude ballon flights.
Really WIP.

## Commands
Currenty the program can be operated with the following commands:

### `newflight <FLIGHTNAME> <balloon properties... >`
Start a new flight with name `<FLIGHTNAME>`. After entering this command, you have to provide the default weather data to be used. (See `test_input.txt` for example.) The end of default weather data is marked by a `.` in a new line.

#### Balloon property details
Balloon properies must be provided in the order they appear here

| Property | Description |
| --- | --- |
| BALLOON_DRY_MASS | Mass of empty balloon (kg) |
| PARACHUTE_DRY_MASS | Mass of parachute (kg) |
| PAYLOAD_DRY_MASS | Mass of payload (kg) |
| NECK_LIFT | Lift measured at the neck of the inflated balloon (kg) |
| PARACHUTE_AREA | Area of parachute (m^2) |
| PARACHUTE_DRAG_C | Parachute drag coefficient |
| BALLOON_DRAG_C | Balloon drag coefficient |
| DESIGN_BURST_DIAM | Design burst diameter of balloon (m) |
| LIFTING_GAS_MOLAR_MASS | Molar mass of the used lifting gas (eg. 0.004 for helium) |

### `senduprapacket <FLIGHTNAME> <PACKET_RAW>`
Send a new packet to the predictor. Packet must use the UPRA packet format and be valid.
The data from the packet gets added to the recieved packet data of flight `<FLIGHTNAME>`, and the program computes wind data using the calulcated distance travelled from the last recieved location.

#### UPRA packet format
Currently we're using a 61 byte long [UKHAS](https://ukhas.org.uk/communication:protocol) type package format without checksum for telemetry:
`$$CCCCCCC,iii,hhmmss,(+/-)xxxx.xxx,(+/-)xxxxx.xxx,aaaaa,eeee,ooo,rrr,`

Where:

| Packet field | Description |
| --- | --- |
| $$ | START bytes |
| CCCCCC | callsign |
| iii | Message ID |
| hhmmss | GPS time (UTC) (hh-hours, mm-minutes, ss-seconds) |
| (+/-)xxxx.xxxx | latitude (NMEA format) |
| (+/-)xxxxx.xxxx | longitude (NMEA format) |
| aaaaa | altitude (m) |
| eeee | external temperature (*10 °C -> eee.e°C) |
| ooo | OBC module temperature (*10 °C -> ooo°C) |
| rrr | COM module temperature (*10 °C -> rrr°C) |

Example:
`$$HAxUPRA,022,082633,+4728.413,+01903.649,00131,0327,002,001,`

Currently the predictior uses data from the fields GPS time, Lat, Lon, Alt. Usage of measured external temperature may get added soon.

### `balloonprop-get <FLIGHTNAME> <PROPNAME>`
Get ballon property `<PROPNAME>` of flight `<FLIGHTNAME>`.

### `balloonprop-set <FLIGHTNAME> <PROPNAME> <VALUE>`
Set ballon property `<PROPNAME>` of flight `<FLIGHTNAME>` to `<VALUE>`.

### `predict <FLIGHTNAME>`
Make a new prediction. Currently the prediction gets returned in the following CSV-like way:
`timestamp,latitude,longitude,altitude`
Where:

| Output field | Description |
| --- | --- |
| timestamp | Time stamp in `hhmmss` format |
| latitude | Latitude in decimal degrees |
| longitude | Longitude in decimal degrees |
| altitude | Altitude in metres |

The end of prediction data is marked by a `.` in a new line.

### `endflight <FLIGHTNAME>`
Conclude flight `<FLIGHTNAME>`.
