# Modbus Registers {#sec_device_mb}

The following is the Modbus data configuration format (item reference):
`{0|1|3|4}xxxxx[-{0|1|3|4}yyyyy <arraysuffix>|<suffix>][,<params>]`

`{0|1|3|4}<xxxxx>` - Modbus register number, [000001-065536], [100001-165536], [300001-365536], [400001-465536]

`{0|1|3|4}<yyyyy>` — the number of the final Modbus register when reading a register array,
[000001-065536], [100001-165536], [300001-365536], [400001-465536].
In this case, the array is read starting with `{0|1|3|4}<xxxxx>` and ending with `{0|1|3|4}<yyyyy>` inclusive;

`<arraysuffix>` — the item reference data type suffix for reading a register array
(required parameter for a register array). Possible values:
- `B` – the array is of type `String`, which is a sequence of bytes,
each of which is represented by two hexadecimal (HEX) characters
(without spaces or other delimiters).
The characters A–F must be specified in uppercase.
The most significant bits in a byte come first (on the left), and the least significant bits come last (on the right).
In this case, the least significant byte of the register comes first (on the left), and the most significant byte comes second (on the right).
For example, for the following PLC memory snapshot:
  | 400001 | 400002 |...| 400010 |
  |--------|--------|---|--------|
  | 0x0BCD | 0x80FE |...| 0xFC05 |

The resulting `ItemReference` will look like this:
  `400001-400010 B = “CD0BFE80...05FC”`

`<suffix>` — the suffix of the item reference data type (optional parameter). The suffix is described in section \ref sec_data_suffix.
By default (if no suffix is specified), the data type is assumed to be `UInt16` for 3x and 4x registers and `Bool` for 0x and 1x.
The suffix is described in section \ref sec_data_suffix

`<params>` - additional optional item parameters:
`[<period>[=<int>]][,msgid=<id>][,<access>=<ro,wo,rw>]`

- `<period>` - the polling period for this `item` in milliseconds.
If the period is not specified, `DefaultPeriod` from the device configuration is used.

- `msgid=<id>` - Modbus message identifier (optional parameter).\
Explicitly groups Modbus registers into Modbus messages. `<id>` can be a number or a word.

- `<access>=<ro,wo,rw>` - access modifier: `ro` or `r` - read-only,
  `wo` or `w` - write-only, `rw` or `wr` - read or write (default).


## Examples

`400001` - `4x` register number 1 (offset 0), data type `UInt16`, default polling period,
           `msgid` - empty (missing), access - read/write


`400021 U,5000, access=ro` — `4x` register number 21 (offset 20), data type `UInt32`, polling period — 5000 milliseconds,
           `msgid` — empty (none), access — read-only


`400033 LF, period=2000, msgid=analog1, access=rw` - `4x` register number 33 (offset 32), data type `Float64`,
           polling period—2000 milliseconds, `msgid`—`analog1`, access—read/write

`400073 LF, access=wo` - `4x` register number 73 (offset 72), data type `Float64`,
           polling period - default, `msgid` - empty (none), access - write-only