# Device/PLC Commands {#sec_device_cmd}

The gateway can accept commands from the upper layer during runtime. Commands are `ItemReference` of type `Bool`.
Writing `true` triggers execution or queues the command. This item type is write‑only and always returns `false` when read.

List of device/PLC commands:

| Command `ItemReference` | Purpose                         |
|:------------------------|:--------------------------------|
| `StatClear`             | Clear device statistics         |
