# Port Commands {#sec_port_cmd}

The gateway can accept commands from the upper layer during runtime. Commands are `ItemReference` of type `Bool`.
Writing `true` triggers execution or queues the command. This item type is write‑only and always returns `false` when read.

List of port commands:

| Command `ItemReference` | Purpose                      |
|:------------------------|:-----------------------------|
| `StatClear`             | Clear port statistics        |
