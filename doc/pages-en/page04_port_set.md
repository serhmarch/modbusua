# Port Settings {#sec_port_set}

The gateway can modify certain parameters at runtime. These `ItemReference` entries are readable and writable (Read/Write).

| `ItemReference` | Data Type | Purpose                                                                                   |
|:----------------|:----------|:------------------------------------------------------------------------------------------|
| `Enable`        | `Bool`    | Availability of this port for the execution cycle (1‑available, 0‑unavailable). If the port becomes unavailable, it is automatically closed (connection is dropped)
