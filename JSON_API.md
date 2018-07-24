## Command formats

### New flight
```json
{
    "cmd": "newflight",
    "flightname": "MATef-2",
    "balloonprops": {
        "BALLOON_DRY_MASS": 123,
        "PARACHUTE_DRY_MASS": 123,
        "PAYLOAD_DRY_MASS": 123,
        "NECK_LIFT": 123,
        "PARACHUTE_AREA": 123,
        "PARACHUTE_DRAG_C": 123,
        "BALLOON_DRAG_C": 123,
        "DESIGN_BURST_DIAM": 123
    },
    "weatherdata": [
        {"hght": 123, "temp": 123, "pres": 321, "wdir": 312, "wspd": 123},
        {"hght": 143, "temp": 121, "pres": 324, "wdir": 310, "wspd": 125},
        ...
    ]
}
```

### Sending an UPRA packet
```json
{
    "cmd": "senduprapacket",
    "flightname": "MATef-2",
    "packet": "$$HAxUPRA,12..."
}
```

### Getting a balloon property
```json
{
    "cmd": "balloonprop_get",
    "propname": " ... "
}
```

### Setting balloon properties
Note: you are not required to include all properties every time

```json
{
    "cmd": "balloonprop_set",
    "bprops": {
        "BALLOON_DRY_MASS": 123,
        "PARACHUTE_DRY_MASS": 123,
        ...
    }
}
```

### Requesting a prediction
```json
{
    "cmd": "predict",
    "flightname": "MATef-2",
    "timestep": 5
}
```

### Ending a flight
```json
{
    "cmd": "endflight",
    "flightname": "MATef-2"
}
```

## Response formats

### Standard success (for commands that only recieve data)
```json
{
    "success": true,
    "msg": "optional message here"
}
```

### Standard error
```json
{
    "success": false,
    "msg": "error message here"
}
```

### Getting balloon properties
Note: `balloonprop_get` returns everything if no `propname` is specified, otherwise only the property named `propname`.

```json
{
    "success": true,
    "bprops": {
        "BALLOON_DRY_MASS": 123,
        "PARACHUTE_DRY_MASS": 123,
        "PAYLOAD_DRY_MASS": 123,
        "NECK_LIFT": 123,
        "PARACHUTE_AREA": 123,
        "PARACHUTE_DRAG_C": 123,
        "BALLOON_DRAG_C": 123,
        "DESIGN_BURST_DIAM": 123
    }
}
```

### Getting a prediction
```json
{
    "success": true,
    "prediction": [
        {"tstamp": "<ISO timestamp>", "lat": 42.0, "lon": 19.0, "alt": 123},
        {"tstamp": "<ISO timestamp>", "lat": 42.1, "lon": 19.1, "alt": 143},
        ...
    ]
}
```
