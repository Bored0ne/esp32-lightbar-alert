# MQTT Light and Sound project.

## Major goals
- Send countdowns in terms of a timestamp.
```json5
{
  "level": "info | success | warning | error | default",
  "message": "string",
  "alertTime": 16784329842 //Unixy epoch
}
```

## Calculation for lights.
Lights should fill up at a rate of (int) (((double) (alertTime - now) / (double) (alertTime - (timeStarted))) * 30)
Should another timer come in that is sooner than the current timer we should have it queue up or push back the current timer and have it start at that start time later.


## level mapping
| info            | success                                 | warning            | error             | default             |
|-----------------|-----------------------------------------|--------------------|-------------------|---------------------|
| navi_float.mp3  | loz_oot_treasure.mp3, loz_get_heart.mp3 | navi_watch_out.mp3 | navi_annoying.mp3 | navi_hey_listen.mp3 |
| white countdown | rainbow countdown                       | yellow countdown   | red countdown     | green countdown     |
