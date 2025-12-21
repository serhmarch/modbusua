# modbusua System Commands {#sec_sys_cmd}

The gateway can accept commands from the upper layer during runtime. Commands are `ItemReference` of type `Bool`.
Writing `true` triggers execution or places the command into a buffer for later execution. This item type is write‑only
and always returns `false` when read.

List of commands for modbusua:

| Command `ItemReference` | Purpose                          |
|:------------------------|:---------------------------------|
| `ReloadConfig`          | Reload configuration from file   |
