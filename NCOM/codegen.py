# MIT License
#
# Copyright (c) 2026 MM Nautronics
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import json
import os
from datetime import datetime

TYPE_MAP = {
    "uint8":  {"c": "uint8_t",  "py": "B", "size": 1},
    "int8":   {"c": "int8_t",   "py": "b", "size": 1},
    "uint16": {"c": "uint16_t", "py": "H", "size": 2},
    "int16":  {"c": "int16_t",  "py": "h", "size": 2},
    "uint32": {"c": "uint32_t", "py": "I", "size": 4},
    "int32":  {"c": "int32_t",  "py": "i", "size": 4},
    "float64": {"c": "double",  "py": "d", "size": 8}
}

def write_license(f, comment_style="c"):
    license_text = [
        "MIT License\n",
        "\n",
        "Copyright (c) 2026 MM Nautronics\n",
        "\n",
        "Permission is hereby granted, free of charge, to any person obtaining a copy\n",
        "of this software and associated documentation files (the \"Software\"), to deal\n",
        "in the Software without restriction, including without limitation the rights\n",
        "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n",
        "copies of the Software, and to permit persons to whom the Software is\n",
        "furnished to do so, subject to the following conditions:\n",
        "\n",
        "The above copyright notice and this permission notice shall be included in all\n",
        "copies or substantial portions of the Software.\n",
        "\n",
        "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n",
        "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n",
        "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n",
        "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n",
        "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n",
        "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n",
        "SOFTWARE.\n"
    ]
    if comment_style == "c":
        f.write("/*\n")
        for line in license_text:
            f.write(" * " + line)
        f.write(" */\n\n")
    else:
        for line in license_text:
            f.write("# " + line)
        f.write("\n")

def generate_c_header(data, output_file):
    protocol = data["protocol"].upper()
    version = data["version"]
    sync_byte_1 = f"0x{int(data['sync_byte_1']):02X}"
    sync_byte_2 = f"0x{int(data['sync_byte_2']):02X}"
    timestamp = datetime.now().strftime("%d.%m.%Y %H:%M:%S")
    
    os.makedirs(os.path.dirname(output_file), exist_ok=True)
    with open(output_file, 'w') as f:
        write_license(f, comment_style="c")
        f.write(f"/*\n * {protocol} C Header File\n * Auto-generated on: {timestamp}\n * Protocol Version: {version}\n */\n\n")
        
        f.write(f"#ifndef INC_{protocol}_PROTOCOL_H_\n")
        f.write(f"#define INC_{protocol}_PROTOCOL_H_\n\n")
        
        f.write("#include <stdint.h>\n")
        f.write("#include <stddef.h>\n\n")
        
        f.write(f"#define {protocol}_SYNC_BYTE_1 {sync_byte_1}\n")
        f.write(f"#define {protocol}_SYNC_BYTE_2 {sync_byte_2}\n\n")
        
        max_payload = data.get('max_payload_size_bytes', 255)
        sync_byte_count = data.get('sync_byte_count', 2)
        header_len = data.get('header_size_bytes', 5)
        footer_len = data.get('footer_size_bytes', 2)
        f.write(f"#define {protocol}_MAX_PAYLOAD_LEN {max_payload}\n")
        f.write(f"#define {protocol}_HEADER_LEN {header_len}\n")
        f.write(f"#define {protocol}_FOOTER_LEN {footer_len}\n")
        f.write(f"#define {protocol}_OVERHEAD_LEN ({protocol}_HEADER_LEN + {protocol}_FOOTER_LEN)\n")
        f.write(f"#define {protocol}_SYNCBYTE_COUNT {sync_byte_count}\n\n")
        
        # 1. Message IDs
        f.write(f"typedef enum {{\n")
        for msg in data['messages']:
            f.write(f"    {protocol}_MSG_{msg['name'].upper()} = {msg['id']},\n")
        f.write(f"}} {protocol}_MsgID_t;\n\n")
        
        # 2. Enums and Constants
        for msg in data['messages']:
            for field in msg['payload']:
                if 'enum' in field:
                    enum_name = f"{protocol}_{msg['name'].upper()}_{field['name'].upper()}"
                    for key, val in field['enum'].items():
                        f.write(f"#define {enum_name}_{key} {val}\n")
                    f.write("\n")
                    
                if 'flags' in field:
                    flag_prefix = f"{protocol}_FLAG_{msg['name'].upper()}"
                    for key, val in field['flags'].items():
                        f.write(f"#define {flag_prefix}_{key} (1 << {val})\n")
                    f.write("\n")

        # 3. Structs
        for msg in data['messages']:
            struct_name = f"{protocol}_Payload_{msg['name']}_t"
            f.write(f"// ID {msg['id']}: {msg.get('desc', '')}\n")
            f.write(f"typedef struct __attribute__((packed)) {{\n")
            
            payload_size = 0
            for field in msg['payload']:
                if field.get('encoding') == 'float32':
                    c_type = 'float'
                else:
                    c_type = TYPE_MAP[field['type']]['c']
                if 'array_length' in field:
                    arr_len = int(field['array_length'])
                    f.write(f"    {c_type} {field['name']}[{arr_len}];\n")
                    payload_size += TYPE_MAP[field['type']]['size'] * arr_len
                else:
                    f.write(f"    {c_type} {field['name']};\n")
                    payload_size += TYPE_MAP[field['type']]['size']
            
            f.write(f"}} {struct_name};\n")
            # Useful constant for buffer management
            f.write(f"#define {protocol}_LEN_{msg['name'].upper()} {payload_size}\n\n")

        # 4. Function Prototypes
        f.write("// Pack/Unpack Functions\n")
        f.write("// Return value is the size of the payload\n")
        for msg in data['messages']:
            msg_name_lower = msg['name'].lower()
            struct_name = f"{protocol}_Payload_{msg['name']}_t"
            f.write(f"size_t {protocol.lower()}_pack_{msg_name_lower}(uint8_t *buf, const {struct_name} *msg);\n")
            f.write(f"size_t {protocol.lower()}_unpack_{msg_name_lower}(const uint8_t *buf, {struct_name} *msg);\n")

        f.write(f"\n#endif /* INC_{protocol}_PROTOCOL_H_ */\n")
    print(f"Generated C Header: {output_file}")


def generate_c_source(data, output_file):
    protocol = data["protocol"].upper()
    timestamp = datetime.now().strftime("%d.%m.%Y %H:%M:%S")
    version = data["version"]
    
    os.makedirs(os.path.dirname(output_file), exist_ok=True)
    with open(output_file, 'w') as f:
        write_license(f, comment_style="c")
        
        f.write(f"/*\n * {protocol} C Source File\n * Auto-generated on: {timestamp}\n * Protocol Version: {version}\n */\n\n")
        
        f.write(f"#include \"{protocol.lower()}_protocol.h\"\n")
        f.write("#include <string.h>\n\n")
        
        for msg in data['messages']:
            msg_name_lower = msg['name'].lower()
            struct_name = f"{protocol}_Payload_{msg['name']}_t"
            
            # Pack
            f.write(f"size_t {protocol.lower()}_pack_{msg_name_lower}(uint8_t* buf, const {struct_name}* msg) {{\n")
            f.write(f"    memcpy(buf, msg, sizeof({struct_name}));\n")
            f.write(f"    return sizeof({struct_name});\n")
            f.write("}\n\n")

            # Unpack
            f.write(f"size_t {protocol.lower()}_unpack_{msg_name_lower}(const uint8_t* buf, {struct_name}* msg) {{\n")
            f.write(f"    memcpy(msg, buf, sizeof({struct_name}));\n")
            f.write(f"    return sizeof({struct_name});\n")
            f.write("}\n\n")
            
    print(f"Generated C Source: {output_file}")


def generate_python_file(data, output_file):
    protocol = data["protocol"].upper()
    version = data["version"]
    sync_byte_1 = f"0x{int(data['sync_byte_1']):02X}"
    sync_byte_2 = f"0x{int(data['sync_byte_2']):02X}"
    timestamp = datetime.now().strftime("%d.%m.%Y %H:%M:%S")
    
    os.makedirs(os.path.dirname(output_file), exist_ok=True)
    with open(output_file, 'w') as f:
        write_license(f, comment_style="py")
    
        f.write(f"'''\nNCOM Python Module\nAuto-generated on: {timestamp}\nVersion: {version}\n'''\n\n")
        f.write("import struct\n\n")
        
        endian_char = ">" if data.get("endianness") == "big" else "<"
        f.write(f"ENDIAN_CHAR = '{endian_char}'\n\n")
        
        f.write(f"SYNC_BYTE_1 = {sync_byte_1}\n")
        f.write(f"SYNC_BYTE_2 = {sync_byte_2}\n\n")
        
        max_payload = data.get('max_payload_size_bytes', 255)
        sync_byte_count = data.get('sync_byte_count', 2)
        header_len = data.get('header_size_bytes', 5)
        footer_len = data.get('footer_size_bytes', 2)
        f.write(f"MAX_PAYLOAD_LEN = {max_payload}\n")
        f.write(f"HEADER_LEN = {header_len}\n")
        f.write(f"FOOTER_LEN = {footer_len}\n")
        f.write(f"OVERHEAD_LEN = HEADER_LEN + FOOTER_LEN\n")
        f.write(f"SYNCBYTE_COUNT = {sync_byte_count}\n\n")

        for msg in data["messages"]:
            for field in msg["payload"]:
                if "enum" in field:
                    dict_name = f"{msg['name'].upper()}_{field['name'].upper()}"
                    f.write(f"{dict_name} = {{\n")
                    for key, val in field["enum"].items():
                        f.write(f"    '{key}': {val},\n")
                    f.write("}\n")
                
                if "flags" in field:
                    dict_name = f"{msg['name'].upper()}_FLAGS"
                    f.write(f"{dict_name} = {{\n")
                    for key, val in field["flags"].items():
                        f.write(f"    '{key}': (1 << {val}),\n")
                    f.write("}\n")
        f.write("\n")

        f.write("class Messages:\n")
        
        f.write("    ID_TO_NAME = {\n")
        for msg in data["messages"]:
            f.write(f"        {msg['id']}: '{msg['name']}',\n")
        f.write("    }\n\n")
        
        f.write("    NAME_TO_ID = {\n")
        for msg in data["messages"]:
            f.write(f"        '{msg['name']}': {msg['id']},\n")
        f.write("    }\n\n")
        
        f.write("    FORMATS = {\n")
        for msg in data["messages"]:
            fmt_string = endian_char 
            for field in msg['payload']:
                arr_len = str(field.get('array_length', ''))
                if field.get('encoding') == 'float32':
                    fmt_string += arr_len + 'f'
                else:
                    fmt_string += arr_len + TYPE_MAP[field['type']]['py']
            f.write(f"        {msg['id']}: '{fmt_string}', # {msg['name']}\n")
        f.write("    }\n\n")

        # Pack Method
        f.write("    @staticmethod\n")
        f.write("    def pack(msg_id, *args):\n")
        f.write("        if msg_id not in Messages.FORMATS:\n")
        f.write("            raise ValueError(f'Unknown Message ID: {msg_id}')\n")
        f.write("        fmt = Messages.FORMATS[msg_id]\n")
        f.write("        try:\n")
        f.write("            return struct.pack(fmt, *args)\n")
        f.write("        except struct.error as e:\n")
        f.write("            print(f'Packing error for ID {msg_id}: {e}')\n")
        f.write("            return None\n\n")

        # Unpack Method
        f.write("    @staticmethod\n")
        f.write("    def unpack(msg_id, data):\n")
        f.write("        if msg_id not in Messages.FORMATS:\n")
        f.write("            return None\n")
        f.write("        fmt = Messages.FORMATS[msg_id]\n")
        f.write("        try:\n")
        f.write("            return struct.unpack(fmt, data)\n")
        f.write("        except struct.error:\n")
        f.write("            return None\n")

    print(f"Generated Python File: {output_file}")

def load_protocol_data():
    base_dir = os.path.dirname(os.path.abspath(__file__))
    def_file = os.path.join(base_dir, 'ncom_def.json')
    if not os.path.exists(def_file):
        print(f"Error: {def_file} not found.")
        return None

    with open(def_file, 'r') as f:
        config = json.load(f)

    merged_messages = []
    
    # If "includes" exists, load messages from included files
    if "includes" in config:
        for section, info in config["includes"].items():
            if info.get("enabled", False):
                path = os.path.join(base_dir, info["path"])
                if os.path.exists(path):
                    try:
                        with open(path, 'r') as f:
                            # Check if file is empty
                            content = f.read().strip()
                            if not content:
                                print(f"Warning: Included file {path} is empty. Skipping.")
                                continue
                                
                            section_data = json.loads(content)
                            if "messages" in section_data:
                                merged_messages.extend(section_data["messages"])
                    except json.JSONDecodeError as e:
                        print(f"Error decoding JSON from {path}: {e}")
                else:
                    print(f"Warning: Included file {path} not found.")
    
    # Also include inline messages if any (backward compatibility)
    if "messages" in config:
         merged_messages.extend(config["messages"])

    # Sort messages by ID to ensure order
    merged_messages.sort(key=lambda x: x['id'])

    config["messages"] = merged_messages
    return config

if __name__ == "__main__":
    protocol_data = load_protocol_data()
    
    if protocol_data:
        generate_c_header(protocol_data, "c/ncom_protocol.h")
        generate_c_source(protocol_data, "c/ncom_protocol.c")
        generate_python_file(protocol_data, "py/ncom_protocol.py")