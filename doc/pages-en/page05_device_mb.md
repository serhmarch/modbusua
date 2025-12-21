# Modbus Registers {#sec_device_mb}

Below is the configuration format for Modbus data (item reference):
`{0|1|3|4}xxxxx[-{0|1|3|4}yyyyy [<arraysuffix>]]|[<suffix>][,<period>][,msgid=<id>]`

`{0|1|3|4}<xxxxx>` — Modbus register number, ranges: [000001‑065536], [100001‑165536], [300001‑365536], [400001‑465536]

`{0|1|3|4}<yyyyy>` — end Modbus register for array reads,
[000001‑065536], [100001‑165536], [300001‑365536], [400001‑465536].
Reads an array starting from `{0|1|3|4}<xxxxx>` through `{0|1|3|4}<yyyyy>` inclusive.

`<arraysuffix>` — data type suffix for reading an array of registers (mandatory for arrays). Possible values:
- `B` — the array is a `String` representing a sequence of bytes, each shown by two hexadecimal characters (HEX)
  (no spaces or separators). Letters A‑F must be uppercase. The high bits in a byte go first (left), low bits last (right).
  The low byte of the register goes first (left), the high byte second (right).
  Example memory snapshot:
  | 400001 | 400002 |...| 400010 |
  |--------|--------|---|--------|
  | 0x0BCD | 0x80FE |...| 0xFC05 |

  Resulting `ItemReference`:
  `400001-400010 B = "CD0BFE80...05FC"`

`<suffix>` — item reference data type suffix (optional). See \ref sec_data_suffix.
By default (when suffix omitted) the data type is `UInt16` for 3x and 4x registers and `Bool` for 0x and 1x.
See \ref sec_data_suffix.

`<period>` — polling period for the `item` in milliseconds. If omitted, `DefaultPeriod` from device configuration is used.

`msgid=<id>` — Modbus message identifier (optional). Explicitly groups Modbus registers into Modbus messages. `<id>` can be a number or a word.

Examples of `ItemReference` values:
* `400001` — single Holding Register 400001 as `UInt16` (default type)
* `400001 F` — 2 Holding Register (32 -bit) starting at 400001 as `Float`
* `400001-400010 B` — Holding Registers 400001 to 400010 as a byte array in `String` format
* `400001 LF, 5000, msgid=1` — Holding Registers 400001 and 400002 as `Double`, polled every 5000 ms
and grouped in Modbus message with ID 1

