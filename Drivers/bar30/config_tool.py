import tkinter as tk
from tkinter import ttk, messagebox
import re
import os

CONFIG_FILE = os.path.join(os.path.dirname(__file__), "bar30_config.h")

class ConfigItem:
    def __init__(self, name, description, line_number, options=None, is_bitmask=False):
        self.name = name
        self.description = description
        self.line_number = line_number
        self.options = options if options else []  # List of (Label, Value)
        self.current_value = ""
        self.is_bitmask = is_bitmask
        self.define_line_idx = -1

class BitmaskSelector(ttk.Frame):
    def __init__(self, parent, config, callback):
        super().__init__(parent)
        self.config = config
        self.callback = callback
        self.vars = {}
        
        # Parse current value into a set of selected options
        # Assumes current_value is "OPT1 | OPT2 | ..." or "(OPT1 | OPT2)"
        val_str = config.current_value.strip("()")
        current_parts = [p.strip() for p in val_str.split('|')]
        
        # If value is "0", it might mean "None"
        
        for label, val in config.options:
            var = tk.BooleanVar(value=(val in current_parts))
            cb = ttk.Checkbutton(self, text=f"{label} ({val})", variable=var, command=self.update_value)
            cb.pack(anchor="w")
            self.vars[val] = var
            
    def update_value(self):
        selected = []
        for val, var in self.vars.items():
            if var.get():
                selected.append(val)
        
        if not selected:
            new_val = "0"
        else:
            new_val = " | ".join(selected)
            if len(selected) > 1:
                new_val = f"({new_val})"
            
        self.config.var.set(new_val)
        if self.callback:
            self.callback()  # Trigger any needed updates (optional)

class ConfigTool:
    def __init__(self, root):
        self.root = root
        self.root.title("BAR30 Configuration Tool")
        self.root.geometry("600x600")

        self.configs = []
        self.file_lines = []
        
        # UI Elements
        self.btn_frame = ttk.Frame(self.root, padding="10")
        self.btn_frame.pack(side=tk.BOTTOM, fill=tk.X)
        
        self.save_btn = ttk.Button(self.btn_frame, text="Save Changes", command=self.save_config)
        self.save_btn.pack(side=tk.RIGHT)
        
        self.reload_btn = ttk.Button(self.btn_frame, text="Reload File", command=self.load_config)
        self.reload_btn.pack(side=tk.LEFT)

        self.main_frame = ttk.Frame(self.root, padding="10")
        self.main_frame.pack(side=tk.TOP, fill=tk.BOTH, expand=True)

        self.canvas = tk.Canvas(self.main_frame)
        self.scrollbar = ttk.Scrollbar(self.main_frame, orient="vertical", command=self.canvas.yview)
        self.scrollable_frame = ttk.Frame(self.canvas)

        self.scrollable_frame.bind(
            "<Configure>",
            lambda e: self.canvas.configure(
                scrollregion=self.canvas.bbox("all")
            )
        )

        self.canvas.create_window((0, 0), window=self.scrollable_frame, anchor="nw")
        self.canvas.configure(yscrollcommand=self.scrollbar.set)

        self.canvas.pack(side="left", fill="both", expand=True)
        self.scrollbar.pack(side="right", fill="y")

        self.load_config()

    def parse_file(self):
        self.configs = []
        if not os.path.exists(CONFIG_FILE):
            messagebox.showerror("Error", f"Could not find {CONFIG_FILE}")
            return

        with open(CONFIG_FILE, 'r') as f:
            self.file_lines = f.readlines()

        current_context = None # (description, options, is_bitmask)
        in_comment_block = False
        
        # Regex patterns
        re_config_header = re.compile(r"^\s*\*\s+(BAR30_SETUP_[A-Za-z0-9_]+)\s+(.+)$")
        re_option = re.compile(r"^\s*\*\s+(.+?)\s{2,}(.+)$") 
        re_define = re.compile(r"^#define\s+(BAR30_SETUP_\w+)\s+(.+)$")
        
        # Context building
        ctx_desc = ""
        ctx_options = []
        ctx_is_bitmask = False
        
        for idx, line in enumerate(self.file_lines):
            line = line.strip()
            
            if line.startswith("/*"):
                in_comment_block = True
                # Reset context
                ctx_desc = ""
                ctx_options = []
                ctx_is_bitmask = False
                continue
            
            if line.endswith("*/"):
                in_comment_block = False
                current_context = (ctx_desc, ctx_options, ctx_is_bitmask)
                continue

            if in_comment_block:
                # remove leading *
                if line.startswith("*"):
                    content = line[1:].strip()
                else:
                    content = line
                
                # Check formatting
                # Header: * BAR30_SETUP_...   Description
                parts = re.split(r'\s{2,}', content)
                
                if len(parts) >= 2 and parts[0].startswith("BAR30_SETUP_"):
                    ctx_desc = parts[1]
                elif len(parts) >= 2 and ctx_desc:
                    # Option
                    val = parts[0]
                    label = " ".join(parts[1:])
                     # Heuristic: Value usually looks like a macro, number, or hex. 
                     # Ignore "See datasheet..." or similar instructions
                    if re.match(r'^(BAR30_|0x|\d|-|\d+\.\d+f|\d+f)', val):
                         ctx_options.append((label, val))
                
                if "bitwise OR" in content:
                    ctx_is_bitmask = True

            else:
                # definitions
                match_def = re_define.match(line)
                if match_def:
                    name = match_def.group(1).strip()
                    val = match_def.group(2).strip()
                    
                    if current_context:
                        cfg = ConfigItem(name, current_context[0], idx, list(current_context[1]), current_context[2])
                        cfg.current_value = val
                        cfg.define_line_idx = idx
                        cfg.var = tk.StringVar(value=val)
                        
                        self.configs.append(cfg)
                    else:
                        print(f"Orphaned define: {name}")

    def load_config(self):
        for widget in self.scrollable_frame.winfo_children():
            widget.destroy()
            
        self.parse_file()
        
        row = 0
        for cfg in self.configs:
            # Frame for each config
            f = ttk.LabelFrame(self.scrollable_frame, text=f"{cfg.name} - {cfg.description}", padding="5")
            f.grid(row=row, column=0, sticky="ew", padx=5, pady=5)
            
            # Ensure var is set if not already
            if not hasattr(cfg, 'var'):
                cfg.var = tk.StringVar(value=cfg.current_value)

            if cfg.is_bitmask and cfg.options:
                # Use Bitmask Selector
                bs = BitmaskSelector(f, cfg, None)
                bs.pack(fill=tk.X)
                
            elif cfg.options:
                # Create detailed dictionary for lookup
                val_map = {opt[1]: opt[0] for opt in cfg.options} # Val -> Label
                
                # Display nicely
                cb = ttk.Combobox(f, textvariable=cfg.var, width=50)
                cb['values'] = [f"{opt[1]} ({opt[0]})" for opt in cfg.options]
                
                # Set current selection formatted
                if cfg.current_value in val_map:
                    cb.set(f"{cfg.current_value} ({val_map[cfg.current_value]})")
                else:
                    cb.set(cfg.current_value) # Custom value
                    
                cb.pack(fill=tk.X)
                
            else:
                # Entry for free text
                e = ttk.Entry(f, textvariable=cfg.var, width=60)
                e.pack(fill=tk.X)
                if cfg.options:
                    options_text = "Options:\n" + "\n".join([f"{opt[1]}: {opt[0]}" for opt in cfg.options])
                    lbl = ttk.Label(f, text=options_text, font=("Consolas", 8), justify=tk.LEFT)
                    lbl.pack(anchor="w")

            row += 1

    def save_config(self):
        saved_count = 0
        for cfg in self.configs:
            if not hasattr(cfg, 'define_line_idx'):
                continue

            raw_val = cfg.var.get()
            # Clean up value from "VALUE (Label)" format
            if " (" in raw_val and ")" in raw_val:
                val = raw_val.split(" (")[0]
            else:
                val = raw_val
            
            orig_line = self.file_lines[cfg.define_line_idx]
            
            pattern = re.compile(fr"^(\s*#define\s+{re.escape(cfg.name)}\s+)(.*)$")
            match = pattern.match(orig_line.rstrip('\r\n')) 
            
            if match:
                prefix = match.group(1)
                # We overwrite the old value completely
                new_line = f"{prefix}{val}\n"
                
                if self.file_lines[cfg.define_line_idx] != new_line:
                    self.file_lines[cfg.define_line_idx] = new_line
                    saved_count += 1
            else:
                print(f"Warning: Could not match line for {cfg.name}")
            
        try:
            with open(CONFIG_FILE, 'w') as f:
                f.writelines(self.file_lines)
            messagebox.showinfo("Success", f"Configuration saved! ({saved_count} items updated)")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to save: {e}")

if __name__ == "__main__":
    root = tk.Tk()
    app = ConfigTool(root)
    root.mainloop()

