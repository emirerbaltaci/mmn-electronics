import socket
import sys
import ncom_protocol as ncom
import json
import os

UDP_IP = "127.0.0.1"
UDP_PORT = 5005

def load_payload_definitions():
    base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    def_path = os.path.join(base_dir, "ncom_def.json")
    try:
        with open(def_path, 'r') as f:
            ncom_def = json.load(f)
    except FileNotFoundError:
        return {}
        
    payloads = {}
    for section_key, section_data in ncom_def.get("includes", {}).items():
        if section_data.get("enabled"):
            sec_path = os.path.join(base_dir, section_data.get("path"))
            try:
                with open(sec_path, 'r') as sf:
                    sec_def = json.load(sf)
                    for msg in sec_def.get("messages", []):
                        payload_info = []
                        for field in msg.get("payload", []):
                            desc = f"{field['name']} ({field.get('encoding', field['type'])})"
                            payload_info.append(desc)
                        payloads[msg["id"]] = payload_info
            except FileNotFoundError:
                pass
    return payloads

def main():
    payload_defs = load_payload_definitions()
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    print("Type message name and arguments separated by spaces.")
    print("Example: COMMAND CLEAR_FLAGS 0")
    print("Example: TARGETS 1 500.0 0.0 0.0 0.0")
    print("Type 'help' for available messages, 'exit' to quit.")
    print("Type 'toggle_hb' to toggle heartbeat printing in main.py.")
    
    while True:
        try:
            line = input("\nCLI > ").strip()
            if not line:
                continue
            if line.lower() in ('exit', 'quit'):
                print("Exiting CLI...")
                sys.exit(0)
                
            parts = line.split()
            msg_name = parts[0].upper()
            
            if msg_name == 'TOGGLE_HB':
                payload = json.dumps({"command": "TOGGLE_HB"})
                sock.sendto(payload.encode('utf-8'), (UDP_IP, UDP_PORT))
                print("Sent request to main.py -> TOGGLE_HB")
                continue
                
            if msg_name == 'HELP':
                print("\nAvailable messages to send:")
                for name, msg_id in ncom.Messages.NAME_TO_ID.items():
                    print(f"  {name} (ID: {msg_id})")
                print("")
                continue
                
            if msg_name not in ncom.Messages.NAME_TO_ID:
                print(f"Unknown message: {msg_name}")
                continue
                
            msg_id = ncom.Messages.NAME_TO_ID[msg_name]
            fmt_string = ncom.Messages.FORMATS[msg_id].replace(ncom.ENDIAN_CHAR, '')
            expected_args = len(fmt_string)
            
            if len(parts) - 1 != expected_args:
                print(f"Error: {msg_name} expects {expected_args} arguments (format {fmt_string}). You provided {len(parts) - 1}.")
                if msg_id in payload_defs:
                    print(f"Expected payload format: {', '.join(payload_defs[msg_id])}")
                continue
                
            # Parse arguments
            args = []
            valid = True
            for i, arg_str in enumerate(parts[1:]):
                char = fmt_string[i]
                try:
                    # special case for Enums like COMMAND_CMD_ID
                    if msg_name == 'COMMAND' and i == 0 and not arg_str.isdigit():
                        if arg_str.upper() in ncom.COMMAND_CMD_ID:
                            args.append(ncom.COMMAND_CMD_ID[arg_str.upper()])
                        else:
                            print(f"Unknown command: {arg_str}")
                            valid = False
                            break
                    elif char in 'fdf':
                        args.append(float(arg_str))
                    else:
                        args.append(int(arg_str, 0)) # supports hex 0x and dec
                except ValueError:
                    print(f"Error parsing argument {i+1} '{arg_str}' for format '{char}'")
                    valid = False
                    break
                    
            if not valid:
                continue
                
            # Send message ID and args to main.py via UDP JSON
            payload = json.dumps({"msg_id": msg_id, "args": args})
            sock.sendto(payload.encode('utf-8'), (UDP_IP, UDP_PORT))
            print(f"Sent request to main.py -> {msg_name} {args}")
            
        except EOFError:
            break
        except KeyboardInterrupt:
            print("\nType 'exit' to quit.")
        except Exception as e:
            print(f"\n[CLI] Error: {e}")

if __name__ == "__main__":
    main()
