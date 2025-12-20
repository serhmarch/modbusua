# modbusua Windows Service Setup Guide

This guide provides comprehensive instructions for creating and using the
modbusua MSI installer for Windows service deployment.

## Quick Start

1. **Install WiX Toolset** (if not already installed)
2. **Build modbusua** in Release mode
3. **Run the MSI builder**: `.\build_msi.bat` or `.\build_msi.ps1`
4. **Install the MSI** on target systems

## Detailed Setup Instructions

### Prerequisites

#### 1. WiX Toolset Installation

Download and install WiX Toolset v3.11 or later:
- Visit: https://wixtoolset.org/releases/
- Download the appropriate version for your system
- Run the installer with Administrator privileges
- Verify installation by opening Command Prompt and typing: `candle.exe -?`

#### 2. modbusua Build

Ensure modbusua is built successfully:

```bash
# Using CMake presets
cmake --preset Release
cmake --build --preset Release

# Or manually
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

The executable should be available at: `build/Release/modbusua.exe`

### Building the MSI Installer

You have three options for building the MSI:

#### Option 1: Batch Script (Recommended for beginners)

```batch
cd install\windows
.\build_msi.bat
```

#### Option 2: PowerShell Script (Recommended for advanced users)

```powershell
cd install\windows
.\build_msi.ps1
```

PowerShell script options:
```powershell
# Basic build
.\build_msi.ps1

# Specify build type
.\build_msi.ps1 -BuildType Debug

# Use specific executable
.\build_msi.ps1 -SourceExe "C:\path\to\modbusua.exe"

# Verbose output with clean build
.\build_msi.ps1 -Verbose -Clean

# Show help
.\build_msi.ps1 -Help
```

#### Option 3: CMake Integration

```bash
# Configure with CMake
cmake --preset Release

# Build the application and MSI
cmake --build --preset Release --target msi

# Or build everything including MSI (if Release mode)
cmake --build --preset Release
```

## MSI Package Details

### Installation Behavior

The MSI installer performs the following actions:

1. **File Installation**
   - Copies `modbusua.exe` to `C:\Program Files\modbusua\`
   - Copies `modbusua.conf` to `C:\Program Files\modbusua\`
   - Copies `modbusua.chm` (help file) if available

2. **Service Registration**
   - Registers "modbusua Gateway Service" as a Windows Service
   - Sets startup type to "Automatic"
   - Configures service to run under LocalSystem account
   - Sets service command line: `modbusua.exe -f "C:\Program Files\modbusua\modbusua.conf"`

3. **Service Management**
   - Stops existing service before upgrade
   - Starts service after installation/upgrade
   - Removes service during uninstallation

4. **System Integration**
   - Adds entry to Add/Remove Programs
   - Creates program menu entries (if configured)
   - Registers uninstall information

### Service Configuration

The modbusua service is configured with:
- **Service Name**: `modbusua`
- **Display Name**: `modbusua Gateway Service`
- **Description**: `OPC UA to Modbus Gateway Service`
- **Startup Type**: `Automatic`
- **Account**: `LocalSystem`
- **Dependencies**: None
- **Recovery**: No automatic recovery configured

## Installation and Usage

### Installing on Target Systems

#### Interactive Installation
1. Copy `modbusua-Setup.msi` to the target system
2. Double-click to launch the installer
3. Follow the installation wizard:
   - Accept license agreement
   - Choose installation directory (default: `C:\Program Files\modbusua`)
   - Complete installation
4. Service starts automatically after installation

#### Silent Installation
```batch
# Silent install
msiexec /i modbusua-Setup.msi /quiet

# Silent install with logging
msiexec /i modbusua-Setup.msi /quiet /l*v "C:\temp\modbusua_install.log"

# Custom installation directory
msiexec /i modbusua-Setup.msi /quiet INSTALLFOLDER="C:\MyPath\modbusua"
```

### Service Management

#### Using Windows Services
1. Open `services.msc`
2. Find "modbusua Gateway Service"
3. Right-click for Start/Stop/Restart options
4. Properties for advanced configuration

#### Using Command Line
```batch
# Start service
net start modbusua

# Stop service
net stop modbusua

# Check service status
sc query modbusua

# Configure service startup
sc config modbusua start= auto
```

#### Using PowerShell
```powershell
# Start service
Start-Service -Name "modbusua"

# Stop service
Stop-Service -Name "modbusua"

# Get service status
Get-Service -Name "modbusua"

# Restart service
Restart-Service -Name "modbusua"
```

### Uninstallation

#### Using Add/Remove Programs
1. Open Control Panel → Programs and Features
2. Find "modbusua Gateway"
3. Click "Uninstall"
4. Follow the uninstallation wizard

#### Using Command Line
```batch
# Silent uninstall
msiexec /x modbusua-Setup.msi /quiet

# Interactive uninstall
msiexec /x modbusua-Setup.msi
```

## Configuration

### Service Configuration File

The service uses `modbusua.conf` located in the installation directory. Key configuration sections:

```ini
[Project]
Name=modbusua Gateway
Log.flags="Error|Warning|Info"
Log.output="system file"
Log.file.path="C:\ProgramData\modbusua\logs\modbusua.log"

[OPCUA]
Port=4840

[Port]
Name=PORT1
Type=TCP
Host="127.0.0.1"
Port=502
```

### Log Files

Default log locations:
- **System Event Log**: Windows Event Viewer → Windows Logs → Application
- **File Log**: `C:\Program Files\modbusua\log.csv` (configurable)
- **Service Log**: Windows Event Viewer → Applications and Services Logs

### Working Directory

The service runs from: `C:\Program Files\modbusua\`
All relative paths in configuration are relative to this directory.

## Troubleshooting

### Installation Issues

#### WiX Toolset Not Found
```
Error: WiX Toolset is not installed or not in PATH
```
**Solution**: Install WiX Toolset and ensure it's in your PATH environment variable.

#### Source Files Missing
```
Error: modbusua.exe not found
```
**Solution**: Build the project first or specify the correct path to the executable.

#### Permission Denied
```
Error: Access denied during installation
```
**Solution**: Run the installer as Administrator.

### Service Issues

#### Service Won't Start
1. Check Windows Event Log for error details
2. Verify configuration file syntax
3. Ensure all dependencies are available
4. Check file permissions

#### Service Stops Unexpectedly
1. Review application logs
2. Check system resources
3. Verify network connectivity
4. Validate configuration parameters

#### Configuration Not Applied
1. Restart the service after configuration changes
2. Verify configuration file path and permissions
3. Check for syntax errors in configuration

### Common Solutions

#### Reinstall Service Manually
```batch
# Stop and remove service
net stop modbusua
sc delete modbusua

# Reinstall using MSI
msiexec /i modbusua-Setup.msi /quiet
```

#### Reset Service Configuration
```batch
# Reset to automatic startup
sc config modbusua start= auto

# Set service account
sc config modbusua obj= LocalSystem

# Set service display name
sc config modbusua displayname= "modbusua Gateway Service"
```

#### Repair Installation
```batch
# Repair using MSI
msiexec /fa modbusua-Setup.msi
```

## Advanced Topics

### Custom Service Configuration

To modify service behavior, edit the `modbusua.wxs` file before building:

```xml
<!-- Change service startup type -->
<ServiceInstall Start="demand" />  <!-- Manual startup -->
<ServiceInstall Start="auto" />    <!-- Automatic startup -->
<ServiceInstall Start="disabled" /><!-- Disabled -->

<!-- Change service account -->
<ServiceInstall Account=".\ServiceAccount" Password="[SERVICE_PASSWORD]" />

<!-- Add service dependencies -->
<ServiceInstall>
  <ServiceDependency Id="Tcpip" />
  <ServiceDependency Id="Eventlog" />
</ServiceInstall>
```

### Multiple Instances

To support multiple modbusua instances:
1. Modify service name in WiX file
2. Use different installation directories
3. Configure different ports and configurations
4. Build separate MSI packages

### Automated Deployment

For enterprise deployment, consider:
- Group Policy software installation
- System Center Configuration Manager
- PowerShell DSC (Desired State Configuration)
- Ansible/Puppet automation

Example PowerShell DSC configuration:
```powershell
Configuration modbusuaDeployment {
    Import-DscResource -ModuleName PSDesiredStateConfiguration
    
    Package modbusua {
        Ensure = "Present"
        Name = "modbusua Gateway"
        Path = "\\server\share\modbusua-Setup.msi"
        ProductId = ""
        Arguments = "/quiet"
    }
    
    Service modbusuaService {
        Name = "modbusua"
        State = "Running"
        StartupType = "Automatic"
        DependsOn = "[Package]modbusua"
    }
}
```

## Support and Maintenance

### Regular Maintenance Tasks

1. **Monitor Service Health**
   - Check service status regularly
   - Review log files for errors
   - Monitor system resources

2. **Update Procedures**
   - Stop service before updates
   - Backup configuration files
   - Install new MSI package
   - Verify service restart

3. **Backup Important Files**
   - Configuration files
   - Log files
   - Custom certificates or keys

### Monitoring Scripts

PowerShell monitoring script example:
```powershell
# Check modbusua service health
$service = Get-Service -Name "modbusua" -ErrorAction SilentlyContinue
if ($service -eq $null) {
    Write-Warning "modbusua service not found"
} elseif ($service.Status -ne "Running") {
    Write-Warning "modbusua service is not running: $($service.Status)"
    # Attempt to start
    Start-Service -Name "modbusua"
} else {
    Write-Host "modbusua service is running normally"
}
```

For additional support, consult:
- modbusua documentation
- Windows Event Logs
- Service-specific log files
- System administrator resources