# modbusua MSI Installer

This directory contains the WiX Toolset project files for creating a Windows MSI installer for modbusua Gateway.

## Prerequisites

1. **WiX Toolset v3.11 or later** - Download from [https://wixtoolset.org/releases/](https://wixtoolset.org/releases/)
2. **Built modbusua executable** - The `modbusua.exe` file must be built and available
3. **Configuration files** - Ensure `modbusua.conf` exists in the parent directory

## Files Description

- **modbusua.wxs** - Main WiX source file defining the installer structure
- **build_msi.bat** - Batch script to build the MSI package
- **README.md** - This file

## Building the MSI

### Method 1: Using the batch script (Recommended)

1. Open Command Prompt as Administrator
2. Navigate to this directory: `cd install\windows`
3. Run the build script: `build_msi.bat`

The script will:
- Check for WiX Toolset installation
- Verify required source files exist
- Copy necessary files
- Build the MSI package
- Clean up temporary files

### Method 2: Manual build

```batch
# Compile the WiX source
candle.exe modbusua.wxs

# Link and create the MSI
light.exe -ext WixUIExtension -out modbusua-Setup.msi modbusua.wixobj
```

## Installation Features

The MSI installer includes:

### Application Files
- **modbusua.exe** - Main executable installed to `C:\Program Files\modbusua\`
- **modbusua.conf** - Configuration file
- **modbusua.chm** - Help file (if available)

### Windows Service Integration
- Automatically registers modbusua as a Windows Service named "modbusua"
- Service Display Name: "modbusua Gateway Service"
- Service Description: "OPC UA to Modbus Gateway Service"
- Startup Type: Automatic
- Runs under LocalSystem account

### Service Management
The installer handles complete service lifecycle:
- **Installation**: Registers and starts the service
- **Upgrade**: Stops service, updates files, restarts service
- **Uninstallation**: Stops and removes the service

### User Interface
- Standard Windows Installer UI with WiX UI extension
- License agreement dialog
- Installation directory selection
- Progress indicators
- Add/Remove Programs integration

## Service Command Line Arguments

The service is installed with the following command line:
```
"C:\Program Files\modbusua\modbusua.exe" -f "C:\Program Files\modbusua\modbusua.conf"
```

## Customization

### Changing Installation Directory
Edit the `modbusua.wxs` file and modify the `INSTALLFOLDER` directory structure.

### Adding More Files
Add new `<File>` elements within the appropriate `<Component>` sections.

### Service Configuration
Modify the `<ServiceInstall>` element in the `ServiceComponent` to change:
- Service name
- Display name
- Startup type
- Account type
- Command line arguments

### Branding
- Replace `modbusua.ico` with your custom icon
- Modify `License.rtf` with your license text
- Update product information in the `<Product>` element

## Troubleshooting

### Common Issues

1. **WiX Toolset not found**
   - Install WiX Toolset from the official website
   - Add WiX bin directory to your PATH environment variable

2. **modbusua.exe not found**
   - Build the project first: `cmake --build --preset Release`
   - Or manually copy the executable to the Release directory

3. **Service installation fails**
   - Run the installer as Administrator
   - Ensure the executable supports `--install` and `--uninstall` commands

4. **Permission denied during build**
   - Run Command Prompt as Administrator
   - Check file permissions in the build directory

### Build Output

Successful build creates:
- `output\modbusua-Setup.msi` - The final installer package

### Testing the Installer

1. **Install**: Double-click `modbusua-Setup.msi` and follow the wizard
2. **Verify Service**: Check Windows Services for "modbusua Gateway Service"
3. **Verify Files**: Check `C:\Program Files\modbusua\` for installed files
4. **Uninstall**: Use Add/Remove Programs or run `msiexec /x modbusua-Setup.msi`

## Advanced Configuration

### Silent Installation
```batch
# Silent install
msiexec /i modbusua-Setup.msi /quiet

# Silent install with logging
msiexec /i modbusua-Setup.msi /quiet /l*v install.log

# Silent uninstall
msiexec /x modbusua-Setup.msi /quiet
```

### Custom Installation Directory
```batch
msiexec /i modbusua-Setup.msi INSTALLFOLDER="C:\MyCustomPath\modbusua"
```

## Support

For build issues or installer problems, check:
1. WiX Toolset documentation: [https://wixtoolset.org/documentation/](https://wixtoolset.org/documentation/)
2. modbusua project documentation
3. Windows Event Log for service-related issues