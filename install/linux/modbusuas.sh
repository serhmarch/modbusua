#!/bin/bash

# @CN_APP_NAME@ Service Management Script
# This script manages @CN_APP_NAME@ systemd Service installation/uninstallation

CN_SCRIPT_NAME=$(basename $0)
CN_APP_NAME=@CN_APP_NAME@
CN_APP_PARAMS=""
CN_DEFAULT_EXE_FILE="/usr/bin/$CN_APP_NAME"
CN_DEFAULT_SERVICE_NAME=$CN_APP_NAME
CN_SERVICE_DESCRIPTION="@CN_APP_DESC@"

CN_EXE_FILE="$CN_DEFAULT_EXE_FILE"
CN_SERVICE_NAME="$CN_DEFAULT_SERVICE_NAME"

CN_ACTION="install"

while [ -n "$1" ]; do
    case "$1" in
        --help|-h|-\?)
            echo
            echo "$CN_APP_NAME Service Management Script"
            echo "================================="
            echo
            echo "Usage: $CN_SCRIPT_NAME [CN_ACTION] [OPTIONS] [-- $CN_APP_NAME OPTIONS]"
            echo
            echo "Actions:"
            echo "  -i [NAME], --install [NAME]    Install $CN_APP_NAME as systemd service (start on demand). NAME - optional, short for '-n NAME'"
            echo "  -u [NAME], --uninstall [NAME]  Uninstall $CN_APP_NAME service (stop if running). NAME - optional, short for '-n NAME'"
            echo "  -h, -?, --help                 Show this help message"
            echo
            echo "Options:"
            echo "  -e FILE, --exe FILE   Executable filename (default: $CN_DEFAULT_EXE_FILE)"
            echo "  -n NAME, --name NAME  Service name (default: $CN_DEFAULT_SERVICE_NAME)"
            echo
            echo "$CN_APP_NAME options:"
            echo "  -- delimiter before $CN_APP_NAME options"
            echo "Example:"
            echo "  $CN_SCRIPT_NAME -i ${CN_APP_NAME}2 -- -f ${CN_APP_NAME}2.conf --logdir /var/log/${CN_APP_NAME}2"
            echo "To see all $CN_APP_NAME options, run:"
            echo "  $CN_APP_NAME --help"
            echo
            echo "Description:"
            echo "  This script manages the $CN_APP_NAME Gateway systemd service installation"
            echo "  and uninstallation. Administrator privileges are required."
            echo
            echo "Examples:"
            echo "  $CN_SCRIPT_NAME --install                    Install with defaults"
            echo "  $CN_SCRIPT_NAME -i -e myserv.exe -n MyServ   Install custom service"
            echo "  $CN_SCRIPT_NAME -i MyServ -e myserv.exe      Install custom service, same as above"
            echo "  $CN_SCRIPT_NAME --uninstall --name MyServ    Remove custom service"
            echo "  $CN_SCRIPT_NAME -u MyServ                    Remove custom service, same as above"
            echo "  $CN_SCRIPT_NAME --help                       Show this help"
            echo
            echo "Default Configuration:"
            echo "  Executable  : $CN_DEFAULT_EXE_FILE"
            echo "  Service Name: $CN_DEFAULT_SERVICE_NAME"
            echo "  Display Name: $CN_SERVICE_DESCRIPTION"
            echo
            exit 0
            ;;
        --install|-i)
            CN_ACTION="install"
            # if next parameter not begins with '-', treat it as CN_SERVICE_NAME
            if [ -n "$2" ] && [[ "$2" != -* ]]; then
                CN_SERVICE_NAME="$2"
                shift
            fi
            ;;
        --uninstall|-u)
            CN_ACTION="uninstall"
            # if next parameter not begins with '-', treat it as CN_SERVICE_NAME
            if [ -n "$2" ] && [[ "$2" != -* ]]; then
                CN_SERVICE_NAME="$2"
                shift
            fi
            ;;
        --exe|-e)
            CN_EXE_FILE="$2"
            shift
            ;;
        --name|-n)
            CN_SERVICE_NAME="$2"
            shift
            ;;
        --)
            shift
            CN_APP_PARAMS="$CN_APP_PARAMS $*"
            break
            ;;
        *)
            echo "Unknown parameter: $1"
            echo "Use --help to see available options."
            exit 1
            ;;
    esac
    shift
done

# Check for administrator privileges
if [ $EUID -ne 0 ]; then
   echo "This script must be run as root" >&2
   exit 1
fi

CN_APP_PARAMS="-s $CN_SERVICE_NAME $CN_APP_PARAMS"

CN_SERVICE_FILE="/etc/systemd/system/${CN_SERVICE_NAME}.service"

CN_WORKING_DIR="/var/lib/$CN_SERVICE_NAME"

if [ "$CN_ACTION" = "install" ]; then
    if [ ! -f "$CN_EXE_FILE" ]; then
        echo "Executable file '$CN_EXE_FILE' not found!"
        exit 1
    fi

    echo "Installing $CN_SERVICE_NAME service..."

    if cat <<EOF > "$CN_SERVICE_FILE"
[Unit]
Description=$CN_SERVICE_DESCRIPTION
After=network.target
[Service]
Type=forking
ExecStart=$CN_EXE_FILE $CN_APP_PARAMS
ExecReload=kill -HUP $MAINPID
WorkingDirectory=$CN_WORKING_DIR
#Restart=on-failure
#RestartSec=5
#StandardOutput=journal
#StandardError=journal

[Install]
WantedBy=multi-user.target
EOF
    then
        echo "Service file '$CN_SERVICE_FILE' of $CN_SERVICE_NAME service installed."
    else
        echo "Failed to create $CN_SERVICE_NAME service file '$CN_SERVICE_FILE'.\n$CN_SERVICE_NAME service installation failed."
        exit 1
    fi

    if mkdir -p "$CN_WORKING_DIR"; then
        echo "Working directory '$CN_WORKING_DIR' successfully created."
    fi

    if systemctl daemon-reload; then
        echo "systemd daemon reloaded."
    fi
    #systemctl enable "$CN_SERVICE_NAME"
    #systemctl start "$CN_SERVICE_NAME"

    echo "$CN_SERVICE_NAME service installed."
else 
    echo "Uninstalling $CN_SERVICE_NAME service..."

    if systemctl stop "$CN_SERVICE_NAME"; then
        echo "$CN_SERVICE_NAME service stopped."
    fi

    if systemctl disable "$CN_SERVICE_NAME"; then
        echo "$CN_SERVICE_NAME service disabled."
    fi
    
    if rm -f "$CN_SERVICE_FILE"; then
        echo "Service file '$CN_SERVICE_FILE' of $CN_SERVICE_NAME service removed."
    fi

    if systemctl daemon-reload; then
        echo "systemd daemon reloaded."
    fi

    echo "$CN_SERVICE_NAME service uninstalled."
fi